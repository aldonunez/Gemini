#include "pch.h"
#include "TestBase.h"

#include "../Gemini/LispyParser.h"
#include "../Gemini/AlgolyParser.h"
#include "../Gemini/Compiler.h"
#include "../Gemini/Disassembler.h"
#include "../Gemini/Machine.h"
#include <string.h>

#define ENABLE_DISASSEMBLY 0

using namespace Gemini;


int NatAdd( Machine* machine, U8 argc, CELL* args, UserContext context )
{
    if ( machine == nullptr || argc != 2 || args == nullptr )
        return ERR_BAD_ARG;

    machine->PushCell( args[0] + args[1] );
    return ERR_NONE;
}

int NatMul( Machine* machine, U8 argc, CELL* args, UserContext context )
{
    if ( machine == nullptr || argc != 2 || args == nullptr )
        return ERR_BAD_ARG;

    machine->PushCell( args[0] * args[1] );
    return ERR_NONE;
}

NativePair gNatives[] =
{
    { 0, NatAdd },
    { 0, NatMul },
    { 0, nullptr }
};


class CompilerEnv : public ICompilerEnv, public IEnvironment
{
    struct MachineFunc
    {
        ExternalKind    Kind;
        union
        {
            ::ByteCode      ByteCode;
            ::NativeCode    NativeCode;
        };
    };

    typedef std::map<std::string, ExternalFunc> FuncMap;
    typedef std::map<int, MachineFunc> IdMap;
    typedef std::map<std::string, int> GlobalMap;

    using ModVec = std::vector<Module>;
    using NativeMap = std::map<int32_t, NativeCode>;

    GlobalMap   mGlobalMap;
    FuncMap     mFuncMap;
    IdMap       mIdMap;
    Module*     mCurMod;
    ModVec      mMods;
    NativeMap   mNatives;

public:
    CompilerEnv();

    bool AddExternal( const std::string& name, ExternalKind kind, int address ) override;
    bool FindExternal( const std::string& name, ExternalFunc* func ) override;

    void SetCurrentModule( Module* mod );

    void SetNativeFuncs( NativePair* natives )
    {
        int32_t prevId = -1;

        for ( size_t i = 0; natives[i].Func != nullptr; i++ )
        {
            int32_t id = natives[i].Id != 0 ? natives[i].Id : prevId + 1;

            mNatives.insert( NativeMap::value_type( id, { natives[i].Func } ) );
            prevId = id;
        }
    }

    bool FindByteCode( U32 id, ByteCode* byteCode );
    bool FindNativeCode( U32 id, NativeCode* nativeCode ) override;

    bool AddGlobal( const std::string& name, int offset ) override;
    bool FindGlobal( const std::string& name, int& offset ) override;

    ModSize GetModuleCount()
    {
        return (ModSize) mMods.size();
    }

    Module* AddModule()
    {
        assert( mMods.size() < ModSizeMax );

        mMods.push_back( { } );
        mCurMod = &mMods.back();
        return &mMods.back();
    }

    virtual Module* FindModule( U8 index ) override
    {
        return &mMods[index];
    }
};

CompilerEnv::CompilerEnv()
    : mCurMod( nullptr )
{
}

bool CompilerEnv::AddExternal( const std::string& name, ExternalKind kind, int address )
{
    auto it = mFuncMap.find( name );
    if ( it != mFuncMap.end() )
        return false;

    using TId = decltype(ExternalFunc::Id);

    assert( mFuncMap.size() < static_cast<size_t>( std::numeric_limits<TId>::max() ) );

    ExternalFunc func;
    func.Id = static_cast<TId>( mFuncMap.size() );
    func.Kind = kind;
    func.Address = address;
    mFuncMap.insert( FuncMap::value_type( name, func ) );

    // A real CompilerEnv is free to declare native procs here.
    assert( kind == ExternalKind::Bytecode );

    MachineFunc macFunc;
    macFunc.Kind = kind;
    macFunc.ByteCode.Address = address;
    macFunc.ByteCode.Module = mCurMod;
    mIdMap.insert( IdMap::value_type( func.Id, macFunc ) );
    return true;
}

bool CompilerEnv::FindExternal( const std::string& name, ExternalFunc* func )
{
    auto it = mFuncMap.find( name );
    if ( it == mFuncMap.end() )
        return false;

    *func = it->second;
    return true;
}

void CompilerEnv::SetCurrentModule( Module* mod )
{
    mCurMod = mod;
}

bool CompilerEnv::FindByteCode( U32 id, ByteCode* byteCode )
{
    auto it = mIdMap.find( id );
    if ( it == mIdMap.end() )
        return false;

    if ( it->second.Kind != ExternalKind::Bytecode )
        return false;

    *byteCode = it->second.ByteCode;
    return true;
}

bool CompilerEnv::FindNativeCode( U32 id, NativeCode* nativeCode )
{
    auto it = mNatives.find( id );
    if ( it == mNatives.end() )
        return false;

    *nativeCode = it->second;
    return true;
}

bool CompilerEnv::AddGlobal( const std::string& name, int offset )
{
    auto it = mGlobalMap.find( name );
    if ( it != mGlobalMap.end() )
        return false;

    mGlobalMap.insert( GlobalMap::value_type( name, offset ) );
    return true;
}

bool CompilerEnv::FindGlobal( const std::string& name, int& offset )
{
    auto it = mGlobalMap.find( name );
    if ( it == mGlobalMap.end() )
        return false;

    offset = it->second;
    return true;
}


class CompilerLog : public ICompilerLog
{
    bool    mSuppressOutput;

public:
    CompilerLog( bool suppressOutput ) :
        mSuppressOutput( suppressOutput )
    {
    }

    virtual void Add( LogCategory category, const char* fileName, int line, int column, const char* message )
    {
        if ( !mSuppressOutput )
        {
            printf( "<%d>  ", static_cast<int>(category) );
            printf( "%s %4d %3d  ", (fileName != nullptr ? fileName : ""), line, column );
            printf( "%s\n", message );
        }
    }
};


void Disassemble( const uint8_t* program, int size )
{
    Disassembler disassembler( program );
    int totalBytesDisasm = 0;
    while ( totalBytesDisasm < size )
    {
        char disasm[256];
        int bytesDisasm = disassembler.Disassemble( disasm, std::size( disasm ) );
        if ( bytesDisasm <= 0 )
            break;
        totalBytesDisasm += bytesDisasm;
        printf( "%s\n", disasm );
    }
}


//-----------------------------------------

static CELL gStack[1024];


void TestCompileAndRun( Language lang, const char* code, ResultVariant result, const std::initializer_list<int>& params, int expectedStack );


void TestCompileAndRunAlgoly( const char* code, int result, int param, int expectedStack )
{
    TestCompileAndRun( Language::Gema, code, Emplace<ResultKind::Stack>( result ), { param }, expectedStack );
}

void TestCompileAndRunAlgoly( const char* code, int result, const std::initializer_list<int>& params, int expectedStack )
{
    TestCompileAndRun( Language::Gema, code, Emplace<ResultKind::Stack>( result ), params, expectedStack );
}

void TestCompileAndRunAlgoly( const char* code, CompilerErr result )
{
    TestCompileAndRun( Language::Gema, code, Emplace<ResultKind::Compiler>( result ), {}, 0 );
}

void TestCompileAndRunLispy( const char* code, int result, int param, int expectedStack )
{
    TestCompileAndRun( Language::Geml, code, Emplace<ResultKind::Stack>( result ), { param }, expectedStack );
}


void TestCompileAndRun( Language lang, const char* code, ResultVariant result, const std::initializer_list<int>& params, int expectedStack )
{
    const char* sources[] =
    {
        code,
        nullptr
    };

    const ModuleSource modSources[] =
    {
        {
            "Main",
            sources,
        },
        nullptr
    };

    TestCompileAndRun( lang, modSources, result, params, expectedStack );
}

void TestCompileAndRun(
    Language lang,
    const ModuleSource* moduleSources,
    int expectedResult,
    int param,
    int expectedStack,
    NativePair* natives
)
{
    TestCompileAndRun(
        lang,
        moduleSources,
        expectedResult,
        { param },
        expectedStack,
        natives );
}


void TestCompileAndRun(
    Language lang,
    const ModuleSource* moduleSources,
    int expectedResult,
    const std::initializer_list<int>& params,
    int expectedStack,
    NativePair* natives
)
{
    TestCompileAndRun(
        lang,
        moduleSources,
        Emplace<ResultKind::Stack>( expectedResult ),
        params,
        expectedStack,
        natives );
}


void TestCompileAndRun(
    Language lang,
    const ModuleSource* moduleSources,
    ResultVariant expectedResult,
    const std::initializer_list<int>& params,
    int expectedStack,
    NativePair* natives
)
{
    std::vector<U8> bin1;
    size_t      binSize = 0;
    std::vector<CELL> data1;
    size_t      dataSize = 0;
    uint32_t    maxStack = 0;

    CompilerEnv env;
    CompilerLog log{ GetKind( expectedResult ) == ResultKind::Compiler };

    std::vector<std::shared_ptr<ModuleDeclaration>> modDecls;

    for ( const ModuleSource* moduleSource = moduleSources;
        moduleSource->UnitTexts != nullptr;
        moduleSource++ )
    {
        Compiler compiler1( &env, &log, env.GetModuleCount() );

        for ( const char** unitSource = moduleSource->UnitTexts;
            *unitSource != nullptr;
            unitSource++ )
        {
            Unique<Unit> unit;

            const char* code = *unitSource;
            size_t codeLen = strlen( code );

            assert( codeLen < INT_MAX );

            if ( lang == Language::Gema )
            {
                AlgolyParser parser( code, (int) codeLen, moduleSource->Name, &log );
                unit = parser.Parse();
            }
            else if ( lang == Language::Geml )
            {
                LispyParser parser( code, (int) codeLen, moduleSource->Name, &log );
                unit = parser.Parse();
            }

            compiler1.AddUnit( std::move( unit ) );
        }

        Module* curMod = env.AddModule();

        for ( const auto& modDecl : modDecls )
        {
            compiler1.AddModule( modDecl );
        }

        CompilerErr compilerErr = compiler1.Compile();
        CompilerStats stats = { 0 };
        compiler1.GetStats( stats );

        maxStack = std::max( maxStack, stats.Static.MaxStackUsage );

        if ( GetKind( expectedResult ) == ResultKind::Compiler )
        {
            REQUIRE( compilerErr == Get<ResultKind::Compiler>( expectedResult ) );
            return;
        }
        else
        {
            REQUIRE( compilerErr == CompilerErr::OK );
        }

        if ( compiler1.GetDataSize() > 0 )
        {
            // Don't assign DataBase yet, because growing the buffer can reallocate it
            curMod->DataSize = (U16) compiler1.GetDataSize();

            data1.resize( data1.size() + compiler1.GetDataSize() );

            std::copy_n( compiler1.GetData(), compiler1.GetDataSize(), data1.data() + dataSize );

            dataSize += compiler1.GetDataSize();
        }

        if ( compiler1.GetCodeSize() > 0 )
        {
            // Don't assign CodeBase yet, because growing the buffer can reallocate it
            curMod->CodeSize = (U32) compiler1.GetCodeSize();

            bin1.resize( bin1.size() + compiler1.GetCodeSize() );

            std::copy_n( compiler1.GetCode(), compiler1.GetCodeSize(), bin1.data() + binSize );

            binSize += compiler1.GetCodeSize();
        }

        modDecls.push_back( compiler1.GetMetadata( moduleSource->Name ) );

#if ENABLE_DISASSEMBLY
        printf( "------------\n" );
        Disassemble( compiler1.GetCode(), compiler1.GetCodeSize() );
#endif
    }

    if ( expectedStack > 0 )
        REQUIRE( (size_t) expectedStack == maxStack );

    Machine machine;

    machine.Init( gStack, static_cast<U16>( std::size( gStack ) ), &env );

    binSize = 0;
    dataSize = 0;

    for ( ModSize i = 0; i < env.GetModuleCount(); i++ )
    {
        Module* mod = env.FindModule( i );

        if ( mod->DataSize > 0 )
        {
            mod->DataBase = data1.data() + dataSize;

            dataSize += mod->DataSize;
        }

        if ( mod->CodeSize > 0 )
        {
            mod->CodeBase = bin1.data() + binSize;

            binSize += mod->CodeSize;
        }
    }

    if ( natives != nullptr )
        env.SetNativeFuncs( natives );

    ExternalFunc external = { 0 };
    ByteCode byteCode = { 0 };
    bool b = false;

    b = env.FindExternal( "a", &external );
    REQUIRE( b );

    b = env.FindByteCode( external.Id, &byteCode );
    REQUIRE( b );

    CELL* args = machine.Start( (U8) (env.GetModuleCount() - 1), byteCode.Address, (U8) params.size() );

    REQUIRE( args != nullptr );

    std::copy( params.begin(), params.end(), args );

    int err = 0;

    do
    {
        err = machine.Run();
    } while ( err == ERR_YIELDED );

    if ( GetKind( expectedResult ) == ResultKind::Vm )
    {
        REQUIRE( err == Get<ResultKind::Vm>( expectedResult ) );
        return;
    }
    else
    {
        REQUIRE( err == 0 );
    }

    if ( GetKind( expectedResult ) == ResultKind::Stack )
        REQUIRE( gStack[std::size( gStack ) - 1] == Get<ResultKind::Stack>( expectedResult ) );
}

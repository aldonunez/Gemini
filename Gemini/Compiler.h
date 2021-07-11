// Gemini Languages and Virtual Machine
// Copyright 2019 Aldo Jose Nunez
//
// Licensed under the Apache License, Version 2.0.
// See the LICENSE.txt file for details.

#pragma once

#include "GeminiCommon.h"
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <optional>
#include <unordered_map>
#include "Syntax.h"


enum OpCode : uint8_t;


namespace Gemini
{

enum CompilerErr
{
    CERR_OK,
    CERR_INTERNAL,
    CERR_UNSUPPORTED,
    CERR_SYNTAX,
    CERR_SEMANTICS,
};

enum ExternalKind
{
    External_Bytecode,
    External_Native,
};

struct ExternalFunc
{
    int             Id;
    int             Address;
    ExternalKind    Kind;
};

class ICompilerEnv
{
public:
    virtual ~ICompilerEnv() { }

    virtual bool AddExternal( const std::string& name, ExternalKind kind, int address ) = 0;
    virtual bool FindExternal( const std::string& name, ExternalFunc* func ) = 0;
    virtual bool AddGlobal( const std::string& name, int offset ) = 0;
    virtual bool FindGlobal( const std::string& name, int& offset ) = 0;
};

enum LogCategory
{
    LOG_ERROR,
    LOG_WARNING,
};

class ICompilerLog
{
public:
    virtual ~ICompilerLog() { }

    virtual void Add( LogCategory category, const char* fileName, int line, int column, const char* message ) = 0;
};

struct CallStats
{
    uint32_t    MaxCallDepth;
    uint32_t    MaxStackUsage;
    bool        Recurses;
};

struct CompilerStats
{
    CodeSize    CodeBytesWritten;
    bool        CallsIndirectly;
    CallStats   Lambda;
    CallStats   Static;
};


class LocalScope;

class CompilerException : public std::exception
{
    CompilerErr     mError;

public:
    CompilerException( CompilerErr error )
        : mError( error )
    {
    }

    CompilerErr GetError() const
    {
        return mError;
    }
};

class Reporter
{
    ICompilerLog* mLog;

public:
    Reporter( ICompilerLog* log );

    ICompilerLog* GetLog();

    void Log( LogCategory category, const char* fileName, int line, int col, const char* format, va_list args );
    void LogWarning( const char* fileName, int line, int col, const char* format, ... );

    [[noreturn]] void ThrowError( CompilerErr exceptionCode, Syntax* elem, const char* format, ... );
    [[noreturn]] void ThrowError( CompilerErr exceptionCode, const char* fileName, int line, int col, const char* format, va_list args );
    [[noreturn]] void ThrowInternalError();
    [[noreturn]] void ThrowInternalError( const char* format, ... );
};


class Compiler final : public IVisitor
{
public:
    enum class CodeRefKind
    {
        Code,
        Data,
    };

    struct CodeRef
    {
        CodeRefKind     Kind;
        int32_t         Location;
    };

    template <typename TRef>
    struct BasicInstPatch
    {
        BasicInstPatch* Next;
        TRef            Ref;
    };

    template <typename TRef>
    struct BasicPatchChain
    {
        static constexpr int32_t UnpatchedIndex = -1;

        BasicInstPatch<TRef>*   First = nullptr;
        int32_t                 PatchedInstIndex = UnpatchedIndex;

        BasicPatchChain()
        {
        }

        ~BasicPatchChain()
        {
            while ( First != nullptr )
            {
                auto link = First;
                First = First->Next;
                delete link;
            }
        }

        BasicPatchChain( BasicPatchChain&& other ) noexcept
        {
            First = other.First;
            PatchedInstIndex = other.PatchedInstIndex;

            other.First = nullptr;
            other.PatchedInstIndex = UnpatchedIndex;
        }

        BasicPatchChain& operator=( BasicPatchChain&& other ) noexcept
        {
            std::swap( First, other.First );
            PatchedInstIndex = other.PatchedInstIndex;
            return *this;
        }
    };

    using InstPatch = BasicInstPatch<int32_t>;
    using FuncInstPatch = BasicInstPatch<CodeRef>;

    using PatchChain = BasicPatchChain<int32_t>;
    using FuncPatchChain = BasicPatchChain<CodeRef>;

    using FuncPatchMap = std::map<std::string, FuncPatchChain>;

private:
    enum class AddrRefKind
    {
        Inst,
    };

    struct AddrRef
    {
        AddrRefKind Kind;
        union
        {
            int32_t*    InstIndexPtr;
        };
    };

    enum class ExprKind
    {
        Other,
        Logical,
        Comparison,
    };

    struct GenStatus
    {
        ExprKind    kind;
        bool        discarded;
        bool        tailRet;

        Declaration*    baseDecl;
        DataSize        offset;
        bool            spilledAddr;
    };

    struct GenConfig
    {
        PatchChain* trueChain;
        PatchChain* falseChain;
        bool invert;
        bool discard;
        bool calcAddr;
        PatchChain* breakChain;
        PatchChain* nextChain;

        static GenConfig Discard()
        {
            GenConfig config = { nullptr, nullptr, false, true };
            return config;
        }

        static GenConfig Statement( bool discard = false )
        {
            GenConfig config = { nullptr, nullptr, false, discard };
            return config;
        }

        static GenConfig Expr( PatchChain* trueChain, PatchChain* falseChain, bool invert )
        {
            GenConfig config = { trueChain, falseChain, invert };
            return config;
        }

        GenConfig Invert() const
        {
            GenConfig config = { trueChain, falseChain, !invert, discard };
            return config;
        }

        GenConfig WithFalse( PatchChain* chain ) const
        {
            GenConfig config = { trueChain, chain, invert };
            return config;
        }

        GenConfig WithTrue( PatchChain* chain ) const
        {
            GenConfig config = { chain, falseChain, invert };
            return config;
        }

        GenConfig WithLoop( PatchChain* breakChain, PatchChain* nextChain ) const
        {
            GenConfig config = *this;
            config.breakChain = breakChain;
            config.nextChain = nextChain;
            return config;
        }

        GenConfig WithDiscard() const
        {
            GenConfig config = *this;
            config.discard = true;
            return config;
        }
    };

    typedef void (Compiler::*ConjClauseGenerator)( Syntax* elem, const GenConfig& config );

    struct ConjSpec
    {
        ConjClauseGenerator PositiveGenerator;
        ConjClauseGenerator NegativeGenerator;
    };

    typedef std::vector<AddrRef> AddrRefVec;
    typedef std::vector<Unique<Unit>> UnitVec;
    typedef std::vector<std::shared_ptr<ModuleDeclaration>> ModVec;

    using CodeVec = std::vector<U8>;
    using GlobalVec = std::vector<I32>;

    struct GenParams
    {
        const GenConfig& config;
        GenStatus& status;
    };

    CodeVec         mCodeBin;
    GlobalVec       mGlobals;

    SymTable        mGlobalTable;
    SymTable        mModuleTable;
    SymTable        mPublicTable;
    FuncPatchMap    mFuncPatchMap;
    AddrRefVec      mLocalAddrRefs;
    bool            mInFunc = false;
    Function*       mCurFunc = nullptr;
    LocalSize       mCurExprDepth = 0;
    LocalSize       mMaxExprDepth = 0;

    ICompilerEnv*   mEnv = nullptr;
    Reporter        mRep;
    ModSize         mModIndex = 0;

    std::vector<GenParams> mGenStack;

    bool            mCompiled = false;
    bool            mCalculatedStats = false;
    CompilerStats   mStats = {};
    UnitVec         mUnits;

    std::shared_ptr<Declaration> mLoadedAddrDecl;

public:
    Compiler( ICompilerEnv* env, ICompilerLog* log, ModSize modIndex = 0 );

    void AddUnit( Unique<Unit>&& unit );
    void AddModule( std::shared_ptr<ModuleDeclaration> moduleDecl );
    CompilerErr Compile();

    void GetStats( CompilerStats& stats );
    U8* GetCode();
    size_t GetCodeSize();
    I32* GetData();
    size_t GetDataSize();
    std::shared_ptr<ModuleDeclaration> GetMetadata( const char* modName );

private:
    void BindAttributes();
    void FoldConstants();
    void GenerateCode();

    // Code generation

    const GenConfig& Config() const;
    GenStatus& Status();

    // Level 1
    void Generate( Syntax* elem );
    void Generate( Syntax* elem, const GenConfig& config );
    void Generate( Syntax* elem, const GenConfig& config, GenStatus& status );
    void GenerateDiscard( Syntax* elem );
    void GenerateDiscard( Syntax* elem, const GenConfig& config );

    // Level 2 - S-expressions
    void GenerateNumber( NumberExpr* number, const GenConfig& config, GenStatus& status );
    void GenerateSymbol( NameExpr* symbol, const GenConfig& config, GenStatus& status );
    void GenerateValue( Syntax* node, Declaration* decl, const GenConfig& config, GenStatus& status );
    void GenerateEvalStar( CallOrSymbolExpr* callOrSymbol, const GenConfig& config, GenStatus& status );
    void GenerateArefAddr( IndexExpr* indexExpr, const GenConfig& config, GenStatus& status );
    void GenerateAref( IndexExpr* indexExpr, const GenConfig& config, GenStatus& status );
    void GenerateDefvar( VarDecl* varDecl, const GenConfig& config, GenStatus& status );
    void GenerateGlobalInit( GlobalSize offset, Syntax* initializer );

    void CalcAddress( Syntax* dotExpr, Declaration*& baseDecl, int32_t& offset );

    void AddGlobalData( GlobalSize offset, Syntax* valueElem );
    void AddGlobalDataArray( GlobalSize offset, Syntax* valueElem, size_t size );

    void EmitLoadConstant( int32_t value );
    void EmitLoadAddress( Syntax* node, Declaration* baseDecl, I32 offset );
    void EmitLoadFuncAddress( Function* func );
    void EmitFuncAddress( Function* func, CodeRef funcRef );
    void EmitLoadScalar( Syntax* node, Declaration* decl, int32_t offset );
    void EmitStoreScalar( Syntax* node, Declaration* decl, int32_t offset );
    void EmitSpilledAddrOffset( int32_t offset );

    // Level 3 - functions and special operators
    void GenerateArithmetic( BinaryExpr* binary, const GenConfig& config, GenStatus& status );
    void GenerateComparison( BinaryExpr* binary, const GenConfig& config, GenStatus& status );
    void GenerateAnd( BinaryExpr* binary, const GenConfig& config, GenStatus& status );
    void GenerateOr( BinaryExpr* binary, const GenConfig& config, GenStatus& status );
    void GenerateReturn( ReturnStatement* retStmt, const GenConfig& config, GenStatus& status );
    void GenerateCond( CondExpr* condExpr, const GenConfig& config, GenStatus& status );
    void GenerateSet( AssignmentExpr* assignment, const GenConfig& config, GenStatus& status );
    void GenerateFunction( AddrOfExpr* addrOf, const GenConfig& config, GenStatus& status );
    void GenerateFuncall( CallExpr* call, const GenConfig& config, GenStatus& status );
    void GenerateLet( LetStatement* letStmt, const GenConfig& config, GenStatus& status );
    void GenerateLetBinding( DataDecl* binding );
    void GenerateLocalInit( LocalSize offset, Syntax* initializer );
    void AddLocalDataArray( LocalSize offset, Syntax* valueElem, size_t size );

    void GenerateCall( CallExpr* call, const GenConfig& config, GenStatus& status );
    void GenerateCall( Declaration* decl, std::vector<Unique<Syntax>>& arguments, const GenConfig& config, GenStatus& status );
    void GenerateCallArgs( std::vector<Unique<Syntax>>& arguments );
    void GenerateFor( ForStatement* forStmt, const GenConfig& config, GenStatus& status );
    void GenerateSimpleLoop( LoopStatement* loopStmt, const GenConfig& config, GenStatus& status );
    void GenerateDo( WhileStatement* whileStmt, const GenConfig& config, GenStatus& status );
    void GenerateBreak( BreakStatement* breakStmt, const GenConfig& config, GenStatus& status );
    void GenerateNext( NextStatement* nextStmt, const GenConfig& config, GenStatus& status );
    void GenerateCase( CaseExpr* caseExpr, const GenConfig& config, GenStatus& status );
    void GenerateGeneralCase( CaseExpr* caseExpr, const GenConfig& config, GenStatus& status );

    void GenerateUnaryPrimitive( Syntax* elem, const GenConfig& config, GenStatus& status );
    void GenerateBinaryPrimitive( BinaryExpr* binary, U8 primitive, const GenConfig& config, GenStatus& status );

    void GenerateProc( ProcDecl* procDecl, Function* func );
    void GenerateImplicitProgn( StatementList* stmtList, const GenConfig& config, GenStatus& status );
    void GenerateStatements( StatementList* list, const GenConfig& config, GenStatus& status );
    void GenerateNilIfNeeded( const GenConfig& config, GenStatus& status );

    void GenerateSentinel();

    // And and Or
    void GenerateConj( ConjSpec* spec, BinaryExpr* binary, const GenConfig& config );
    void GenerateAndClause( Syntax* elem, const GenConfig& config );
    void GenerateOrClause( Syntax* elem, const GenConfig& config );
    void Atomize( ConjSpec* spec, BinaryExpr* binary, bool invert, bool discard );

    // And and Or plumbing
    void ElideTrue( PatchChain* trueChain, PatchChain* falseChain );
    void ElideFalse( PatchChain* trueChain, PatchChain* falseChain );
    U8 InvertJump( U8 opCode );

    // Backpatching
    void Patch( PatchChain* chain, int32_t targetIndex = -1 );
    template <typename TRef>
    void PushBasicPatch( BasicPatchChain<TRef>* chain, TRef patchLoc );
    void PushPatch( PatchChain* chain, int32_t patchLoc );
    void PushPatch( PatchChain* chain );
    void PopPatch( PatchChain* chain );
    void PatchCalls( FuncPatchChain* chain, U32 addr );
    void PushFuncPatch( const std::string& name, CodeRef ref );

    I32 GetSyntaxValue( Syntax* node, const char* message = nullptr );

    // Stack usage
    void IncreaseExprDepth();
    void DecreaseExprDepth( LocalSize amount = 1 );
    void CalculateStackDepth();
    void CalculateStackDepth( Function* func );

    // Emitting instructions
    size_t ReserveCode( size_t size );
    void DeleteCode( size_t size );
    void DeleteCode( size_t start, size_t size );
    void EmitBranch( OpCode opcode, PatchChain* chain );
    void Emit( OpCode opcode );
    void EmitU8( OpCode opcode, U8 operand );
    void EmitU16( OpCode opcode, U16 operand );
    void EmitU32( OpCode opcode, U32 operand );
    void EmitModAccess( OpCode opcode, U8 mod, U16 addr );


    // IVisitor
    virtual void VisitAddrOfExpr( AddrOfExpr* addrOf ) override;
    virtual void VisitAssignmentExpr( AssignmentExpr* assignment ) override;
    virtual void VisitBinaryExpr( BinaryExpr* binary ) override;
    virtual void VisitBreakStatement( BreakStatement* breakStmt ) override;
    virtual void VisitCallExpr( CallExpr* call ) override;
    virtual void VisitCallOrSymbolExpr( CallOrSymbolExpr* callOrSymbol ) override;
    virtual void VisitCaseExpr( CaseExpr* caseExpr ) override;
    virtual void VisitCondExpr( CondExpr* condExpr ) override;
    virtual void VisitCountofExpr( CountofExpr* countofExpr ) override;
    virtual void VisitDotExpr( DotExpr* dotExpr ) override;
    virtual void VisitForStatement( ForStatement* forStmt ) override;
    virtual void VisitIndexExpr( IndexExpr* indexExpr ) override;
    virtual void VisitLambdaExpr( LambdaExpr* lambdaExpr ) override;
    virtual void VisitLetStatement( LetStatement* letStmt ) override;
    virtual void VisitLoopStatement( LoopStatement* loopStmt ) override;
    virtual void VisitNameExpr( NameExpr* nameExpr ) override;
    virtual void VisitNextStatement( NextStatement* nextStmt ) override;
    virtual void VisitNumberExpr( NumberExpr* numberExpr ) override;
    virtual void VisitProcDecl( ProcDecl* procDecl ) override;
    virtual void VisitReturnStatement( ReturnStatement* retStmt ) override;
    virtual void VisitSliceExpr( SliceExpr* sliceExpr ) override;
    virtual void VisitStatementList( StatementList* stmtList ) override;
    virtual void VisitUnaryExpr( UnaryExpr* unary ) override;
    virtual void VisitUnit( Unit* unit ) override;
    virtual void VisitVarDecl( VarDecl* varDecl ) override;
    virtual void VisitWhileStatement( WhileStatement* whileStmt ) override;
};

}

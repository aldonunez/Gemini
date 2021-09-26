#include "pch.h"
#include "LangCommon.h"
#include "OpCodes.h"
#include "Syntax.h"
#include <stdarg.h>


namespace Gemini
{

//----------------------------------------------------------------------------
//  Log
//----------------------------------------------------------------------------

void Log( ICompilerLog* log, LogCategory category, const char* fileName, int line, int col, const char* format, va_list args )
{
    if ( log != nullptr )
    {
        char msg[256] = "";
        vsnprintf( msg, sizeof msg, format, args );
        log->Add( category, fileName, line, col, msg );
    }
}


Reporter::Reporter( ICompilerLog* log ) :
    mLog( log )
{
    assert( log != nullptr );
}

ICompilerLog* Reporter::GetLog()
{
    return mLog;
}

void Reporter::ThrowError( CompilerErr exceptionCode, Syntax* elem, const char* format, va_list args )
{
    const char* fileName = nullptr;
    int line = 0;
    int column = 0;
    if ( elem != nullptr )
    {
        fileName = elem->FileName;
        line = elem->Line;
        column = elem->Column;
    }
    ThrowError( exceptionCode, fileName, line, column, format, args );
}

void Reporter::ThrowError( CompilerErr exceptionCode, const char* fileName, int line, int col, const char* format, va_list args )
{
    Log( LogCategory::ERROR, fileName, line, col, format, args );
    throw CompilerException( exceptionCode );
}

void Reporter::ThrowSemanticsError( Syntax* node, const char* format, ... )
{
    va_list args;
    va_start( args, format );
    ThrowError( CompilerErr::SEMANTICS, node, format, args );
    // No need to run va_end( args ), since an exception was thrown
}

void Reporter::ThrowInternalError( const char* fileName, int line, const char* format, ... )
{
    va_list args;
    va_start( args, format );
    ThrowError( CompilerErr::INTERNAL, fileName, line, 1, format, args );
    // No need to run va_end( args ), since an exception was thrown
}

void Reporter::Log( LogCategory category, const char* fileName, int line, int col, const char* format, va_list args )
{
    Gemini::Log( mLog, category, fileName, line, col, format, args );
}

void Reporter::LogWarning( const char* fileName, int line, int col, const char* format, ... )
{
    va_list args;
    va_start( args, format );
    Log( LogCategory::WARNING, fileName, line, col, format, args );
    va_end( args );
}


//----------------------------------------------------------------------------
//  CompilerAttrs
//----------------------------------------------------------------------------

int32_t CompilerAttrs::AddFunctionByIndex( std::shared_ptr<Function> func )
{
    auto funcIt = mConstFuncIndexMap.find( func.get() );
    int32_t index;

    if ( funcIt == mConstFuncIndexMap.end() )
    {
        index = -static_cast<int32_t>(mConstFuncIndexMap.size()) - 1;

        mConstFuncIndexMap.insert( ConstFuncIndexMap::value_type( func.get(), index ) );
        mConstIndexFuncMap.insert( ConstIndexFuncMap::value_type( index, func ) );
    }
    else
    {
        index = funcIt->second;
    }

    return index;
}

void CompilerAttrs::AddFunctionByAddress( std::shared_ptr<Function> func )
{
    uint32_t address = CodeAddr::Build( func->Address, func->ModIndex );

    mAddressFuncMap.insert( AddressFuncMap::value_type( address, func ) );
}

std::shared_ptr<Function> CompilerAttrs::GetFunction( int32_t id ) const
{
    if ( id < 0 )
        return mConstIndexFuncMap.find( id )->second;
    else
        return mAddressFuncMap.find( id )->second;
}

void CompilerAttrs::AddModule( std::shared_ptr<ModuleAttrs> module )
{
    assert( module->GetIndex() < ModSizeMax );

    if ( module->GetIndex() >= mModules.size() )
    {
        mModules.resize( module->GetIndex() + 1 );
    }

    mModules[module->GetIndex()] = module;
}

std::shared_ptr<ModuleAttrs> CompilerAttrs::GetModule( int32_t index ) const
{
    return mModules[index];
}


//----------------------------------------------------------------------------
//  ModuleAttrs
//----------------------------------------------------------------------------

ModuleAttrs::ModuleAttrs( uint_least8_t modIndex, CompilerAttrs& globalAttrs ) :
    mModIndex( modIndex ),
    mGlobalAttrs( globalAttrs )
{
}

ModSize ModuleAttrs::GetIndex()
{
    return mModIndex;
}

CompilerAttrs& ModuleAttrs::GetGlobalAttrs()
{
    return mGlobalAttrs;
}

std::vector<int32_t>& ModuleAttrs::GetConsts()
{
    return mConsts;
}

GlobalSize ModuleAttrs::GrowConsts( GlobalSize amount )
{
    auto oldSize = mConsts.size();

    assert( amount <= (GlobalSizeMax - oldSize) );

    mConsts.resize( oldSize + amount );

    return static_cast<GlobalSize>(oldSize);
}

}

#include "pch.h"
#include "LangCommon.h"
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

}

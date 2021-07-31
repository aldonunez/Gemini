// Gemini Languages and Virtual Machine
// Copyright 2021 Aldo Jose Nunez
//
// Licensed under the Apache License, Version 2.0.
// See the LICENSE.txt file for details.

#pragma once


namespace Gemini
{

class Syntax;


enum class CompilerErr
{
    OK,
    INTERNAL,
    UNSUPPORTED,
    SYNTAX,
    SEMANTICS,
};


enum class LogCategory
{
    ERROR,
    WARNING,
};


class ICompilerLog
{
public:
    virtual void Add( LogCategory category, const char* fileName, int line, int column, const char* message ) = 0;
};


class Reporter
{
    ICompilerLog* mLog;

public:
    Reporter( ICompilerLog* log );

    ICompilerLog* GetLog();

    void Log( LogCategory category, const char* fileName, int line, int col, const char* format, va_list args );
    void LogWarning( const char* fileName, int line, int col, const char* format, ... );

    [[noreturn]] void ThrowError( CompilerErr exceptionCode, Syntax* elem, const char* format, va_list args );
    [[noreturn]] void ThrowError( CompilerErr exceptionCode, const char* fileName, int line, int col, const char* format, va_list args );
    [[noreturn]] void ThrowSemanticsError( Syntax* node, const char* format, ... );
    [[noreturn]] void ThrowInternalError( const char* fileName, int line, const char* format, ... );
};


#define THROW_INTERNAL_ERROR( ... ) \
    do { assert( false ); mRep.ThrowInternalError( __FILE__, __LINE__, __VA_ARGS__ ); } while ( 0 )

}

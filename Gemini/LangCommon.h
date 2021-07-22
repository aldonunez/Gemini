// Gemini Languages and Virtual Machine
// Copyright 2021 Aldo Jose Nunez
//
// Licensed under the Apache License, Version 2.0.
// See the LICENSE.txt file for details.

#pragma once


namespace Gemini
{

class Syntax;


enum CompilerErr
{
    CERR_OK,
    CERR_INTERNAL,
    CERR_UNSUPPORTED,
    CERR_SYNTAX,
    CERR_SEMANTICS,
};


enum LogCategory
{
    LOG_ERROR,
    LOG_WARNING,
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

    [[noreturn]] void ThrowError( CompilerErr exceptionCode, Syntax* elem, const char* format, ... );
    [[noreturn]] void ThrowError( CompilerErr exceptionCode, const char* fileName, int line, int col, const char* format, va_list args );
    [[noreturn]] void ThrowInternalError();
    [[noreturn]] void ThrowInternalError( const char* format, ... );
};

}

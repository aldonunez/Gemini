#include "stdafx.h"
#include "LispyParser.h"
#include <stdarg.h>


LispyParser::LispyParser( const char* codeText, int codeTextLen, ICompilerLog* log ) :
    mCodeTextPtr( codeText ),
    mCodeTextEnd( codeText + codeTextLen ),
    mLine( 1 ),
    mLineStart( codeText ),
    mCurToken( Token_Bof ),
    mCurNumber( 0 ),
    mTokLine( 0 ),
    mTokCol( 0 ),
    mLog( log )
{
}

LispyParser::TokenCode LispyParser::NextToken()
{
    SkipWhitespace();

    mCurString.clear();
    mCurNumber = 0;
    mTokLine = mLine;
    mTokCol = GetColumn();

    if ( mCodeTextPtr >= mCodeTextEnd )
    {
        mCurToken = Token_Eof;
    }
    else if ( *mCodeTextPtr == '(' )
    {
        mCurToken = Token_LParen;
        mCodeTextPtr++;
    }
    else if ( *mCodeTextPtr == ')' )
    {
        mCurToken = Token_RParen;
        mCodeTextPtr++;
    }
    else if ( isdigit( *mCodeTextPtr ) )
    {
        mCurToken = Token_Number;
        ReadNumber();
    }
    else if ( *mCodeTextPtr == '-' && isdigit( mCodeTextPtr[1] ) )
    {
        mCurToken = Token_Number;
        ReadNumber();
    }
    else
    {
        mCurToken = Token_Symbol;
        ReadSymbol();
    }

    return mCurToken;
}

void LispyParser::SkipWhitespace()
{
    while ( true )
    {
        while ( mCodeTextPtr < mCodeTextEnd
            && isspace( *mCodeTextPtr ) )
        {
            if ( *mCodeTextPtr == '\n' )
            {
                mLine++;
                mLineStart = mCodeTextPtr + 1;
            }

            mCodeTextPtr++;
        }

        if ( mCodeTextPtr == mCodeTextEnd
            || *mCodeTextPtr != ';' )
        {
            break;
        }

        while ( mCodeTextPtr < mCodeTextEnd
            && *mCodeTextPtr != '\n' )
        {
            mCodeTextPtr++;
        }
    }
}

void LispyParser::ReadNumber()
{
    bool negate = false;

    if ( *mCodeTextPtr == '-' )
    {
        negate = true;
        mCodeTextPtr++;
    }

    while ( isdigit( *mCodeTextPtr ) )
    {
        mCurString.append( 1, *mCodeTextPtr );
        mCodeTextPtr++;
    }

    if ( mCodeTextPtr < mCodeTextEnd
        && !isspace( *mCodeTextPtr )
        && (*mCodeTextPtr != '(')
        && (*mCodeTextPtr != ')')
        && (*mCodeTextPtr != ';') )
        ThrowSyntaxError( "syntax error : bad number" );

    mCurNumber = atoi( mCurString.c_str() );
    if ( negate )
        mCurNumber = -mCurNumber;
}

void LispyParser::ReadSymbol()
{
    while ( (*mCodeTextPtr != '(')
        && (*mCodeTextPtr != ')')
        && (*mCodeTextPtr != ';')
        && !isspace( *mCodeTextPtr ) )
    {
        mCurString.append( 1, *mCodeTextPtr );
        mCodeTextPtr++;
    }
}

int LispyParser::GetColumn()
{
    return mCodeTextPtr - mLineStart + 1;
}

Compiler::Slist* LispyParser::Parse()
{
    std::unique_ptr<Compiler::Slist> list( new Compiler::Slist() );
    list->Code = Compiler::Elem_Slist;

    while ( NextToken() != Token_Eof )
    {
        if ( mCurToken != Token_LParen )
            ThrowSyntaxError( "syntax error : expected list" );

        list->Elements.push_back( std::unique_ptr<Compiler::Slist>( ParseSlist() ) );
    }

    return list.release();
}

Compiler::Slist* LispyParser::ParseSlist()
{
    std::unique_ptr<Compiler::Slist> list( new Compiler::Slist() );
    list->Code = Compiler::Elem_Slist;
    list->Line = mTokLine;
    list->Column = mTokCol;

    for ( ; ; )
    {
        switch ( NextToken() )
        {
        case Token_LParen:
            list->Elements.push_back( std::unique_ptr<Compiler::Slist>( ParseSlist() ) );
            break;

        case Token_RParen:
            goto Done;

        case Token_Number:
            list->Elements.push_back( std::unique_ptr<Compiler::Number>( ParseNumber() ) );
            break;

        case Token_Symbol:
            list->Elements.push_back( std::unique_ptr<Compiler::Symbol>( ParseSymbol() ) );
            break;

        case Token_Eof:
            ThrowSyntaxError( "syntax error : unexpected end-of-file" );
            break;

        default:
            ThrowInternalError();
        }
    }
Done:

    return list.release();
}

Compiler::Number* LispyParser::ParseNumber()
{
    Compiler::Number* number = new Compiler::Number();
    number->Code = Compiler::Elem_Number;
    number->Value = mCurNumber;
    number->Line = mTokLine;
    number->Column = mTokCol;
    return number;
}

Compiler::Symbol* LispyParser::ParseSymbol()
{
    Compiler::Symbol* symbol = new Compiler::Symbol();
    symbol->Code = Compiler::Elem_Symbol;
    symbol->String = mCurString;
    symbol->Line = mTokLine;
    symbol->Column = mTokCol;
    return symbol;
}

void LispyParser::ThrowError( CompilerErr exceptionCode, int line, int col, const char* format, va_list args )
{
    Compiler::Log( mLog, LOG_ERROR, line, col, format, args );
    throw Compiler::CompilerException( exceptionCode );
}

void LispyParser::ThrowSyntaxError( const char* format, ... )
{
    va_list args;
    va_start( args, format );
    ThrowError( CERR_SYNTAX, mTokLine, mTokCol, format, args );
    va_end( args );
}

void LispyParser::ThrowInternalError( const char* format, ... )
{
    va_list args;
    va_start( args, format );
    ThrowError( CERR_INTERNAL, mLine, GetColumn(), format, args );
    va_end( args );
}

void LispyParser::ThrowInternalError()
{
    ThrowInternalError( "Internal error" );
}

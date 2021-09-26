// Gemini Languages and Virtual Machine
// Copyright 2021 Aldo Jose Nunez
//
// Licensed under the Apache License, Version 2.0.
// See the LICENSE.txt file for details.

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <optional>


namespace Gemini
{

class Syntax;


enum class CompilerErr
{
    NONE,
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


struct Function;
class InitList;
class RecordInitializer;


using ConstIndexFuncMap = std::map<int32_t, std::shared_ptr<Function>>;


class ModuleAttrs
{
    using ConstFuncIndexMap = std::map<Function*, int32_t>;

    ConstFuncIndexMap   mConstFuncIndexMap;
    ConstIndexFuncMap   mConstIndexFuncMap;

public:
    int32_t AddFunction( std::shared_ptr<Function> func );
    std::shared_ptr<Function> GetFunction( int32_t index ) const;
};


class GlobalDataGenerator
{
public:
    using GlobalSize = uint16_t;

    using EmitFuncAddressFunctor = std::function<void( std::optional<std::shared_ptr<Function>>, GlobalSize, int32_t*, Syntax* )>;
    using CopyAggregateFunctor = std::function<void( GlobalSize, int32_t*, Syntax* )>;

private:
    std::vector<int32_t>&   mGlobals;
    EmitFuncAddressFunctor  mEmitFuncAddressFunctor;
    CopyAggregateFunctor    mCopyAggregateFunctor;
    ModuleAttrs&            mModuleAttrs;
    Reporter&               mRep;

public:
    GlobalDataGenerator(
        std::vector<int32_t>& globals,
        EmitFuncAddressFunctor emitFuncAddressFunctor,
        CopyAggregateFunctor copyAggregateFunctor,
        ModuleAttrs& moduleAttrs,
        Reporter& reporter );

    void GenerateGlobalInit( GlobalSize offset, Syntax* initializer );

private:
    void EmitGlobalScalar( GlobalSize offset, Syntax* valueElem );
    void EmitGlobalArrayInitializer( GlobalSize offset, InitList* initList, size_t size );
    void EmitGlobalRecordInitializer( GlobalSize offset, RecordInitializer* recordInit );
};


enum class TypeKind;
class Type;

bool IsScalarType( TypeKind kind );
bool IsIntegralType( TypeKind kind );
bool IsClosedArrayType( Type& type );
bool IsOpenArrayType( Type& type );
bool IsPtrFuncType( Type& type );

}

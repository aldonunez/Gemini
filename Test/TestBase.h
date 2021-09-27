#pragma once

#include <stdint.h>
#include <variant>
#include "../Gemini/Machine.h"
#include "../Gemini/LangCommon.h"


enum class Language
{
    Gema,
    Geml,
};


template <typename T>
class Span
{
    T*          mFirst = nullptr;
    size_t      mSize = 0;

public:
    Span() = default;

    template <size_t N>
    Span( T (&array)[N] ) :
        mFirst( &array[0] ),
        mSize( N )
    {
    }

    Span( T* first, T* last ) :
        mFirst( first ),
        mSize( last - first )
    {
    }

    size_t size() const
    {
        return mSize;
    }

    T* begin() const
    {
        return mFirst;
    }

    T* end() const
    {
        return mFirst + mSize;
    }
};


struct ModuleSource
{
    const char*         Name = nullptr;
    Span<const char*>   Units;

    ModuleSource( const char* name, Span<const char*> unitTexts ) :
        Name( name ),
        Units( unitTexts )
    {
    }
};


struct NativePair
{
    int32_t     Id;
    NativeFunc  Func;
};


enum class ResultKind
{
    Compiler,
    Vm,
    Stack,
};

using ResultVariant = std::variant<Gemini::CompilerErr, int, int32_t>;

constexpr ResultKind GetKind( const ResultVariant& variant )
{
    return static_cast<ResultKind>(variant.index());
}

template <ResultKind kind>
constexpr auto Get( const ResultVariant& variant )
{
    return std::get<static_cast<size_t>(kind)>( variant );
}

template <ResultKind kind, typename T>
constexpr ResultVariant Emplace( T value )
{
    return ResultVariant{ std::in_place_index<static_cast<size_t>(kind)>, value };
}


using ParamSpan = Span<const int>;


constexpr int32_t DefaultParam = INT32_MAX;


struct TestConfig
{
    Language            lang = Language::Gema;
    Span<const ModuleSource> moduleSources;
    ParamSpan           params;
    ResultVariant       expectedResult = ResultVariant();
    int                 expectedStack = 0;
    NativePair*         natives = nullptr;

    TestConfig() = default;

    TestConfig( const ParamSpan& params ) :
        params( params )
    {
    }
};


void TestCompileAndRunAlgoly( const char* code, int result, int param = DefaultParam, int expectedStack = 0 );
void TestCompileAndRunAlgoly( const char* code, int result, const std::initializer_list<int>& params, int expectedStack = 0 );
void TestCompileAndRunAlgoly( const char* code, int result, const ParamSpan& params, int expectedStack = 0 );
void TestCompileAndRunAlgoly( const char* code, Gemini::CompilerErr result );
void TestCompileAndRunAlgoly( const char* code, VmError result );

void TestCompileAndRunLispy( const char* code, int result, int param = DefaultParam, int expectedStack = 0 );

void TestCompileAndRun(
    Language lang,
    Span<const ModuleSource> moduleSources,
    int expectedResult,
    int param,
    int expectedStack = 0,
    NativePair* natives = nullptr
);

void TestCompileAndRun(
    Language lang,
    Span<const ModuleSource> moduleSources,
    ResultVariant expectedResult,
    const ParamSpan& params,
    int expectedStack = 0,
    NativePair* natives = nullptr
);

void TestCompileAndRun( const TestConfig& config );


// Sample natives

int NatAdd( Machine* machine, U8 argc, CELL* args, UserContext context );
int NatMul( Machine* machine, U8 argc, CELL* args, UserContext context );

extern NativePair gNatives[];

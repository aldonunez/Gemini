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


struct ModuleSource
{
    const char* Name;
    const char** UnitTexts;
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


constexpr int32_t DefaultParam = INT32_MAX;


void TestCompileAndRunAlgoly( const char* code, int result, int param = DefaultParam, int expectedStack = 0 );
void TestCompileAndRunAlgoly( const char* code, int result, const std::initializer_list<int>& params, int expectedStack = 0 );
void TestCompileAndRunAlgoly( const char* code, Gemini::CompilerErr result );

void TestCompileAndRunLispy( const char* code, int result, int param = DefaultParam, int expectedStack = 0 );

void TestCompileAndRun(
    Language lang,
    const ModuleSource* moduleSources,
    int expectedResult,
    int param,
    int expectedStack = 0,
    NativePair* natives = nullptr
);

void TestCompileAndRun(
    Language lang,
    const ModuleSource* moduleSources,
    int expectedResult,
    const std::initializer_list<int>& params,
    int expectedStack = 0,
    NativePair* natives = nullptr
);

void TestCompileAndRun(
    Language lang,
    const ModuleSource* moduleSources,
    ResultVariant expectedResult,
    const std::initializer_list<int>& params,
    int expectedStack = 0,
    NativePair* natives = nullptr
);


// Sample natives

int NatAdd( Machine* machine, U8 argc, CELL* args, UserContext context );
int NatMul( Machine* machine, U8 argc, CELL* args, UserContext context );

extern NativePair gNatives[];

#pragma once

#include <stdint.h>
#include "../Gemini/Machine.h"


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


constexpr int32_t DefaultParam = INT32_MAX;


// TODO: get rid of codeLen parameters

void TestCompileAndRunAlgoly( const char* code, int codeLen, int result, int param = DefaultParam, int expectedStack = 0 );
void TestCompileAndRunAlgoly( const char* code, int codeLen, int result, const std::initializer_list<int>& params, int expectedStack = 0 );

void TestCompileAndRunLispy( const char* code, int codeLen, int result, int param = DefaultParam, int expectedStack = 0 );

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


// Sample natives

int NatAdd( Machine* machine, U8 argc, CELL* args, UserContext context );
int NatMul( Machine* machine, U8 argc, CELL* args, UserContext context );

extern NativePair gNatives[];

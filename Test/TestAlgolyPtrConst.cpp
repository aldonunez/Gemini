#include "pch.h"
#include "TestBase.h"

using namespace Gemini;


TEST_CASE( "Algoly: simple const fptr", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a const f = &C; (f)() end\n"
        "def C -> int 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 3, 0, 3 );
}

TEST_CASE( "Algoly: global const lambda", "[algoly][ptr-const]" )
{
    const char code[] =
        "const f = lambda 3 end\n"
        "def a\n"
        "  (f)()\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 3, 0, 3 );
}

TEST_CASE( "Algoly: local const lambda", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a\n"
        "  const f = lambda 3 end\n"
        "  (f)()\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 3, 0, 3 );
}

TEST_CASE( "Algoly: const &proc no typeref, &proc(int) typeref, copy const, call", "[algoly][ptr-const]" )
{
    const char code[] =
        // Output: 8
        "def a\n"
        "  const f = &C, g: &proc(int) = &B\n"
        "  const h = f\n"
        "  (f)() + (g)(2) + (h)() end\n"
        "def B(x) x end\n"
        "def C 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 8, 0, 5 );
}

TEST_CASE( "Algoly: complex ptr-const", "[algoly][ptr-const]" )
{
    const char code[] =
        //"def a 33 end"
        // Output: 22
        "const FB = &B\n"
        "const FA = FB\n"
        "var GA := FA\n"
        "var GB := &C\n"
        "var arr: [3] of &proc->int := [&B, &C, FA]\n"
        "def a\n"
        "  const FC = &C, FD = FC\n"
        "  var VD := FD\n"
        "  (FC)() + (FD)() + (VD)() +\n"
        "  (FB)() + (FA)() + (GA)() +\n"
        "  (GB)() + (arr[0])() + (arr[2])()\n"
        "end\n"
        "def B 2 end\n"
        "def C 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 22, 0, 5 );
}


//----------------------------------------------------------------------------
// Const array
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: simple const array", "[algoly][ptr-const][x]" )
{
    const char code[] =
        "const AR1 = [1, 2, 3]\n"
        "def a\n"
        "  AR1[2]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: const array, var index", "[algoly][ptr-const][x]" )
{
    const char code[] =
        "const AR1 = [1, 2, 3]\n"
        "var i := 2\n"
        "def a\n"
        "  AR1[i]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: simple const array of array", "[algoly][ptr-const][x]" )
{
    const char code[] =
        "const AR1 = [[1, 2], [3, 4]]\n"
        "def a\n"
        "  AR1[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 4 );
}

TEST_CASE( "Algoly: TODO1 simple const array of array", "[algoly][ptr-const][x]" )
{
    const char code[] =
        "const AR1 = [[1, 2], [3, 4]]\n"
        "def a\n"
        "  AR1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 0 );
}


//----------------------------------------------------------------------------
// Const record
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: const record", "[algoly][ptr-const][x]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "const R1: R = { a: 1, b: 2, c: 3 }\n"
        "def a\n"
        "  R1.c\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: const record in record", "[algoly][ptr-const][x]" )
{
    const char code[] =
        "type Q = record a: R, b: R end\n"
        "type R = record c, d end\n"
        "const Q1: Q = { a: { c: 1, d: 2 }, b: { c: 3, d: 4 } }\n"
        "def a\n"
        "  Q1.b.d\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 4 );
}


//----------------------------------------------------------------------------
// Const nested record and arrays
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: const record in array", "[algoly][ptr-const][x]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "const AR: [2] of R = [{ a: 1, b: 2 }, { a: 3, b: 4 }]\n"
        "def a\n"
        "  AR[1].b\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 4 );
}

TEST_CASE( "Algoly: const array in record", "[algoly][ptr-const][x]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "const R1: R = { a: [1, 2], b: [3, 4] }\n"
        "def a\n"
        "  R1.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 4 );
}

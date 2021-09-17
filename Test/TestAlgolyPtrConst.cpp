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

TEST_CASE( "Algoly: const array of int, const index", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [1, 2, 3]\n"
        "def a\n"
        "  AR1[2]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: const array of int, var index", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [1, 2, 3]\n"
        "var i := 2\n"
        "def a\n"
        "  AR1[i]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: const array of fptr, const index", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [&B, &C, &D]\n"
        "def a\n"
        "  (AR1[2])()\n"
        "end\n"
        "def B 1 end\n"
        "def C 2 end\n"
        "def D 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: const array of fptr, var index", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [&B, &C, &D]\n"
        "var i := 2\n"
        "def a\n"
        "  (AR1[i])()\n"
        "end\n"
        "def B 1 end\n"
        "def C 2 end\n"
        "def D 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: const array of array", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [[1, 2], [3, 4]]\n"
        "def a\n"
        "  AR1[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 4 );
}

TEST_CASE( "Algoly: assign to const array, const index", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "const AR1 = [1, 2]\n"
        "def a\n"
        "  AR1[1] := 10\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: assign to const array, var index", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "const AR1 = [1, 2]\n"
        "var i := 1\n"
        "def a\n"
        "  AR1[i] := 10\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: assign var array to const", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "const AR1 = [1, 2]\n"
        "var ar1: [2] := []\n"
        "def a\n"
        "  AR1 := ar1\n"
        "  ar1[0] + ar1[1] + AR1[0] + AR1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: assign var array in array to const", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "const AR1 = [1, 2]\n"
        "var ar1: [2] of [2] := []\n"
        "def a\n"
        "  AR1 := ar1[1]\n"
        "  ar1[0] + ar1[1] + AR1[0] + AR1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: assign var array in array to const, var index", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "const AR1 = [1, 2]\n"
        "var ar1: [2] of [2] := []\n"
        "var i := 1\n"
        "def a\n"
        "  AR1 := ar1[i]\n"
        "  ar1[0] + ar1[1] + AR1[0] + AR1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: assign const array to var", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [1, 2]\n"
        "var ar1: [2] := []\n"
        "def a\n"
        "  ar1 := AR1\n"
        "  ar1[0] + ar1[1] + AR1[0] + AR1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 );
}

TEST_CASE( "Algoly: assign const array in array to var", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [[1, 2], [3, 4]]\n"
        "var ar1: [2] := []\n"
        "def a\n"
        "  ar1 := AR1[1]\n"
        "  ar1[0] + ar1[1] + AR1[0][0] + AR1[0][1] + AR1[1][0] + AR1[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17 );
}

TEST_CASE( "Algoly: assign const array in array to var, var index", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [[1, 2], [3, 4]]\n"
        "var ar1: [2] := []\n"
        "var i := 1\n"
        "def a\n"
        "  ar1 := AR1[i]\n"
        "  ar1[0] + ar1[1] + AR1[0][0] + AR1[0][1] + AR1[1][0] + AR1[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17 );
}


//----------------------------------------------------------------------------
// Const record
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: const record", "[algoly][ptr-const]" )
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

TEST_CASE( "Algoly: const record of fptr", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a: @proc, b: @proc, c: @proc end\n"
        "const R1: R = { a: @A, b: @B, c: @C }\n"
        "def a\n"
        "  (R1.c)()\n"
        "end\n"
        "def A 1 end\n"
        "def B 2 end\n"
        "def C 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: const record in record", "[algoly][ptr-const]" )
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

TEST_CASE( "Algoly: assign to const record field", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "const R1: R = { a: 1, b: 2, c: 3 }\n"
        "def a\n"
        "  R1.c := 10\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: assign const record to var record", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "const R1: R = { a: 1, b: 2, c: 3 }\n"
        "var r1: R := {}\n"
        "def a\n"
        "  r1 := R1\n"
        "  r1.a + r1.b + r1.c +\n"
        "  R1.a + R1.b + R1.c\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 12 );
}

TEST_CASE( "Algoly: assign var record to const record", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "const R1: R = { a: 1, b: 2, c: 3 }\n"
        "var r1: R := {}\n"
        "def a\n"
        "  R1 := r1\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}


//----------------------------------------------------------------------------
// Const nested record and arrays
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: const record in array", "[algoly][ptr-const]" )
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

TEST_CASE( "Algoly: const array in record", "[algoly][ptr-const]" )
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

TEST_CASE( "Algoly: const record of int and fptr in array, const index", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b, f: @proc end\n"
        "const AR: [2] of R = [{ a: 1, b: 2, f: @B }, { a: 3, b: 4, f: @C }]\n"
        "def a\n"
        "  AR[0].a + AR[0].b + (AR[0].f)() + \n"
        "  AR[1].a + AR[1].b + (AR[1].f)() \n"
        "end\n"
        "def B 5 end\n"
        "def C 10 end\n"
        ;

    TestCompileAndRunAlgoly( code, 25 );
}

TEST_CASE( "Algoly: const record of int and fptr in array, var index", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b, f: @proc end\n"
        "const AR: [2] of R = [{ a: 1, b: 2, f: @B }, { a: 3, b: 4, f: @C }]\n"
        "var i := 0, j := 1\n"
        "def a\n"
        "  AR[i].a + AR[i].b + (AR[i].f)() + \n"
        "  AR[j].a + AR[j].b + (AR[j].f)() \n"
        "end\n"
        "def B 5 end\n"
        "def C 10 end\n"
        ;

    TestCompileAndRunAlgoly( code, 25 );
}


//----------------------------------------------------------------------------
// Const arguments to var parameters
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: pass const int to var param", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "const N = 3\n"
        "def a\n"
        "  B(N)\n"
        "end\n"
        "def B(var n) end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: pass const int in record to var param", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "const R1: R = { a: 1, b: 2, c: 3 }\n"
        "def a\n"
        "  B(R1.c)\n"
        "end\n"
        "def B(var n) end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: pass const int in array to var param, const index", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "const AR1 = [ 1, 2, 3 ]\n"
        "def a\n"
        "  B(AR1[2])\n"
        "end\n"
        "def B(var n) end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: pass const int in array to var param, var index", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "const AR1 = [ 1, 2, 3 ]\n"
        "var i := 2\n"
        "def a\n"
        "  B(AR1[i])\n"
        "end\n"
        "def B(var n) end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: pass const record to var param", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "const R1: R = { a: 1, b: 2, c: 3 }\n"
        "def a\n"
        "  B(R1)\n"
        "end\n"
        "def B(var r: R) end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: pass const array to var param", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "const AR1 = [ 1, 2, 3 ]\n"
        "def a\n"
        "  B(AR1)\n"
        "end\n"
        "def B(var array: [2]) end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: pass const array to var param open array", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "const AR1 = [ 1, 2, 3 ]\n"
        "def a\n"
        "  B(AR1)\n"
        "end\n"
        "def B(var array: []) end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: pass closed slice of const array to var param open array", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "const AR1 = [ 1, 2, 3 ]\n"
        "def a\n"
        "  B(AR1[1..3])\n"
        "end\n"
        "def B(var array: []) end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: pass const array in array to var param", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "const AR1 = [[1, 2], [3, 4]]\n"
        "def a\n"
        "  B(AR1[1])\n"
        "end\n"
        "def B(var array: [2]) end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: pass const array in array to var param open array", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "const AR1 = [[1, 2], [3, 4]]\n"
        "def a\n"
        "  B(AR1[1])\n"
        "end\n"
        "def B(var array: []) end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: pass closed slice of const array in array to var param open array", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "const AR1 = [[1, 2, 3], [4, 5, 6]]\n"
        "def a\n"
        "  B(AR1[1][1..3])\n"
        "end\n"
        "def B(var array: []) end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: pass const array in array to var param, var index", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "const AR1 = [[1, 2], [3, 4]]\n"
        "var i := 1\n"
        "def a\n"
        "  B(AR1[i])\n"
        "end\n"
        "def B(var array: [2]) end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: pass const array in array to var param open array, var index", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "const AR1 = [[1, 2], [3, 4]]\n"
        "var i := 1\n"
        "def a\n"
        "  B(AR1[i])\n"
        "end\n"
        "def B(var array: []) end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: pass closed slice of const array in array to var param open array, var index", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "const AR1 = [[1, 2, 3], [4, 5, 6]]\n"
        "var i := 1\n"
        "def a\n"
        "  B(AR1[i][1..3])\n"
        "end\n"
        "def B(var array: []) end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

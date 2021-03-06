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
// Init const with const
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: init const int by copying", "[algoly][ptr-const]" )
{
    const char code[] =
        "const A = 3\n"
        "const B = A\n"
        "def a\n"
        "  B\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: init const fptr by copying", "[algoly][ptr-const]" )
{
    const char code[] =
        "const f = &C\n"
        "const g = f\n"
        "def a\n"
        "  (g)()\n"
        "end\n"
        "def C 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: init const int by copying from record field", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b end\n"
        "const R1: R = { a: 2, b: 3 }\n"
        "const B = R1.b\n"
        "def a\n"
        "  B\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: init const fptr by copying from record field", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, f: @proc end\n"
        "const R1: R = { a: 2, f: @C }\n"
        "const g = R1.f\n"
        "def a\n"
        "  (g)()\n"
        "end\n"
        "def C 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: init const fptr field from const", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, f: @proc end\n"
        "const F = @C\n"
        "const R1: R = { a: 2, f: F }\n"
        "def a\n"
        "  (R1.f)()\n"
        "end\n"
        "def C 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: init const int field from const field", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b end\n"
        "const R1: R = { a: 1, b: 3 }\n"
        "const R2: R = { a: 2, b: R1.b }\n"
        "def a\n"
        "  R2.a + R2.b\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 5 );
}

TEST_CASE( "Algoly: init const int field from var field", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "type R = record a, b end\n"
        "var R1: R := { a: 1, b: 3 }\n"
        "const R2: R = { a: 2, b: R1.b }\n"
        "def a\n"
        "  R2.a + R2.b\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: init var int field from const field", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b end\n"
        "const R1: R = { a: 1, b: 3 }\n"
        "var R2: R := { a: 2, b: R1.b }\n"
        "def a\n"
        "  R2.a + R2.b\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 5 );
}

TEST_CASE( "Algoly: init const record by copying", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "const R1: R = { a: 1, b: 2, c: 3 }\n"
        "const R2: R = R1\n"
        "def a\n"
        "  R2.a + R2.b + R2.c\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 );
}

TEST_CASE( "Algoly: init const record by copying var", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "var R1: R := { a: 1, b: 2, c: 3 }\n"
        "const R2: R = R1\n"
        "def a\n"
        "  R2.a + R2.b + R2.c\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: init var record by copying const", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "const R0 = [10, 20]\n"
        "const R1: R = { a: 1, b: 2, c: 3 }\n"
        "var n0 := 30\n"
        "var R2: R := R1\n"
        "def a\n"
        "  R2.a + R2.b + R2.c\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 );
}

TEST_CASE( "Algoly: init const record by record initializer and copying", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b end\n"
        "type S = record c: R, r: R end\n"
        "const R1: R = { a: 3, b: 4 }\n"
        "const S1: S = { c: { a: 1, b: 2 }, r: R1 }\n"
        "def a\n"
        "  S1.c.a + S1.c.b + S1.r.a + S1.r.b\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}

TEST_CASE( "Algoly: init const record by record initializer and copying var", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "type R = record a, b end\n"
        "type S = record c: R, r: R end\n"
        "var R1: R := { a: 3, b: 4 }\n"
        "const S1: S = { c: { a: 1, b: 2 }, r: R1 }\n"
        "def a\n"
        "  S1.c.a + S1.c.b + S1.r.a + S1.r.b\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: init var record by record initializer and copying const", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b end\n"
        "type S = record c: R, r: R end\n"
        "const R1: R = { a: 3, b: 4 }\n"
        "var S1: S := { c: { a: 1, b: 2 }, r: R1 }\n"
        "def a\n"
        "  S1.c.a + S1.c.b + S1.r.a + S1.r.b\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}

TEST_CASE( "Algoly: init const record field by record initializer and copying const", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b end\n"
        "type S = record c: R, r: R end\n"
        "const S0: S = { c: { a: 5, b: 6 }, r: { a: 3, b: 4 } }\n"
        "const S1: S = { c: { a: 1, b: 2 }, r: S0.r }\n"
        "def a\n"
        "  S1.c.a + S1.c.b + S1.r.a + S1.r.b\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}

TEST_CASE( "Algoly: init const record field by record initializer and copying var", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b end\n"
        "type S = record c: R, r: R end\n"
        "var S0: S := { c: { a: 5, b: 6 }, r: { a: 3, b: 4 } }\n"
        "const S1: S = { c: { a: 1, b: 2 }, r: S0.r }\n"
        "def a\n"
        "  S1.c.a + S1.c.b + S1.r.a + S1.r.b\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: init var record field by record initializer and copying const", "[algoly][ptr-const]" )
{
    const char code[] =
        "type Q = record a, b end\n"
        "type R = record a, f: @proc end\n"
        "type S = record c: Q, r: R end\n"
        "const C0 = [10, 20]\n"
        "const S0: S = { c: { a: 5, b: 6 }, r: { a: 3, f: @C } }\n"
        "var N0 := 30\n"
        "var S1: S := { c: { a: 1, b: 2 }, r: S0.r }\n"
        "def a\n"
        "  S1.c.a + S1.c.b + S1.r.a + (S1.r.f)()\n"
        "end\n"
        "def C 4 end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}

TEST_CASE( "Algoly: init const int by copying from array element", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [2, 3]\n"
        "const B = AR1[1]\n"
        "def a\n"
        "  B\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: init const array by copying", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [1, 2, 3]\n"
        "const AR2 = AR1\n"
        "def a\n"
        "  AR2[0] + AR2[1] + AR2[2]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 );
}

TEST_CASE( "Algoly: init const int field from const", "[algoly][ptr-const]" )
{
    const char code[] =
        "const X = 3\n"
        "const AR1 = [2, X]\n"
        "def a\n"
        "  AR1[0] + AR1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 5 );
}

TEST_CASE( "Algoly: init const int element from const field", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b end\n"
        "const R1: R = { a: 1, b: 3 }\n"
        "const AR1 = [2, R1.b]\n"
        "def a\n"
        "  AR1[0] + AR1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 5 );
}

TEST_CASE( "Algoly: init const int element from var field", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "type R = record a, b end\n"
        "var R1: R := { a: 1, b: 3 }\n"
        "const AR1 = [2, R1.b]\n"
        "def a\n"
        "  AR1[0] + AR1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: init const array by array initializer and copying", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [3, 4]\n"
        "const AR2 = [[1, 2], AR1]\n"
        "def a\n"
        "  AR2[0][0] + AR2[0][1] + AR2[1][0] + AR2[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}


//----------------------------------------------------------------------------
// Assignment
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: assign const int from const", "[algoly][ptr-const]" )
{
    const char code[] =
        "const A = 3\n"
        "const B = 1\n"
        "def a\n"
        "  B := A\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: assign const int from const record field", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b end\n"
        "const R1: R = { a: 2, b: 3 }\n"
        "const B = 1\n"
        "def a\n"
        "  B := R1.b\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: assign const record from const", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "const R1: R = { a: 1, b: 2, c: 3 }\n"
        "const R2: R = { }\n"
        "def a\n"
        "  R2 := R1\n"
        "  R2.a + R2.b + R2.c\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

//-------------

TEST_CASE( "Algoly: assign const record from var", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "var R1: R := { a: 1, b: 2, c: 3 }\n"
        "const R2: R = { }\n"
        "def a\n"
        "  R2 := R1\n"
        "  R2.a + R2.b + R2.c\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

//-----------

TEST_CASE( "Algoly: assign var record from const", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "const R0 = [10, 20]\n"
        "const R1: R = { a: 1, b: 2, c: 3 }\n"
        "var n0 := 30\n"
        "var R2: R := {}\n"
        "def a\n"
        "  R2 := R1\n"
        "  R2.a + R2.b + R2.c\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 );
}

TEST_CASE( "Algoly: assign var record in record from const", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b end\n"
        "type S = record c: R, r: R end\n"
        "const R1: R = { a: 3, b: 4 }\n"
        "var S1: S := { c: { a: 1, b: 2 }, r: {} }\n"
        "def a\n"
        "  S1.r := R1\n"
        "  S1.c.a + S1.c.b + S1.r.a + S1.r.b\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}

TEST_CASE( "Algoly: assign var record field from const record field", "[algoly][ptr-const]" )
{
    const char code[] =
        "type Q = record a, b end\n"
        "type R = record a, f: @proc end\n"
        "type S = record c: Q, r: R end\n"
        "const C0 = [10, 20]\n"
        "const S0: S = { c: { a: 5, b: 6 }, r: { a: 3, f: @C } }\n"
        "var N0 := 30\n"
        "var S1: S := { c: { a: 1, b: 2 }, r: { a: 7, f: @D } }\n"
        "def a\n"
        "  S1.r := S0.r\n"
        "  S1.c.a + S1.c.b + S1.r.a + (S1.r.f)()\n"
        "end\n"
        "def C 4 end\n"
        "def D 99 end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}

TEST_CASE( "Algoly: assign var array from const", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [1, 2, 3]\n"
        "var AR2: [3] := []\n"
        "def a\n"
        "  AR2 := AR1\n"
        "  AR2[0] + AR2[1] + AR2[2]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 );
}

TEST_CASE( "Algoly: assign copy var open array param from const closed array", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [1, 2, 3]\n"
        "var AR2: [3] := []\n"
        "def a\n"
        "  B(AR2, AR1)\n"
        "  AR2[0] + AR2[1] + AR2[2]\n"
        "end\n"
        "def B(var array1: [], const array2: [3])\n"
        "  array1 := array2\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 );
}

TEST_CASE( "Algoly: assign copy var open array param from const open array", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [1, 2, 3]\n"
        "var AR2: [3] := []\n"
        "def a\n"
        "  B(AR2, AR1)\n"
        "  AR2[0] + AR2[1] + AR2[2]\n"
        "end\n"
        "def B(var array1: [], const array2: [])\n"
        "  array1 := array2\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 );
}

TEST_CASE( "Algoly: assign var array element array from const array element array", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [[5, 6], [3, 4]]\n"
        "var AR2 := [[1, 2], []]\n"
        "def a\n"
        "  AR2[1] := AR1[1]\n"
        "  AR2[0][0] + AR2[0][1] + AR2[1][0] + AR2[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
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


//----------------------------------------------------------------------------
// Var arguments to const parameters
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: pass var int to const param", "[algoly][ptr-const]" )
{
    const char code[] =
        "var n := 3\n"
        "def a\n"
        "  B(n)\n"
        "end\n"
        "def B(const N) N end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: pass var int in record to const param", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "var r1: R := { a: 1, b: 2, c: 3 }\n"
        "def a\n"
        "  B(r1.c)\n"
        "end\n"
        "def B(const N) N end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: pass var int in array to const param, const index", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [ 1, 2, 3 ]\n"
        "def a\n"
        "  B(ar1[2])\n"
        "end\n"
        "def B(const N) N end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: pass var int in array to const param, var index", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [ 1, 2, 3 ]\n"
        "var i := 2\n"
        "def a\n"
        "  B(ar1[i])\n"
        "end\n"
        "def B(const N) N end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: pass var record to const param", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "var r1: R := { a: 1, b: 2, c: 3 }\n"
        "def a\n"
        "  B(r1)\n"
        "end\n"
        "def B(const RR: R) RR.a + RR.b + RR.c end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 );
}

TEST_CASE( "Algoly: pass var array to const param", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [ 1, 2, 3 ]\n"
        "def a\n"
        "  B(ar1)\n"
        "end\n"
        "def B(const ARRAY: [2]) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: pass var array to const param open array", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [ 1, 2, 3 ]\n"
        "def a\n"
        "  B(ar1)\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[0] + ARRAY[1] + ARRAY[2] end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 );
}

TEST_CASE( "Algoly: pass closed slice of var array to const param open array", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [ 1, 2, 3 ]\n"
        "def a\n"
        "  B(ar1[1..3])\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 5 );
}

TEST_CASE( "Algoly: pass var array in array to const param", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  B(ar1[1])\n"
        "end\n"
        "def B(const ARRAY: [2]) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 7 );
}

TEST_CASE( "Algoly: pass var array in array to const param open array", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  B(ar1[1])\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 7 );
}

TEST_CASE( "Algoly: pass closed slice of var array in array to const param open array", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [[1, 2, 3], [4, 5, 6]]\n"
        "def a\n"
        "  B(ar1[1][1..3])\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 11 );
}

TEST_CASE( "Algoly: pass var array in array to const param, var index", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [[1, 2], [3, 4]]\n"
        "var i := 1\n"
        "def a\n"
        "  B(ar1[i])\n"
        "end\n"
        "def B(const ARRAY: [2]) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 7 );
}

TEST_CASE( "Algoly: pass var array in array to const param open array, var index", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [[1, 2], [3, 4]]\n"
        "var i := 1\n"
        "def a\n"
        "  B(ar1[i])\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 7 );
}

TEST_CASE( "Algoly: pass closed slice of var array in array to const param open array, var index", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [[1, 2, 3], [4, 5, 6]]\n"
        "var i := 1\n"
        "def a\n"
        "  B(ar1[i][1..3])\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 11 );
}

TEST_CASE( "Algoly: pass var array in array to const param open array, var index array and param", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [[1, 2], [3, 4]]\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  B(ar1[one])\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[zero] + ARRAY[one] + countof(ARRAY) end\n"
        ;

    TestCompileAndRunAlgoly( code, 9 );
}


//----------------------------------------------------------------------------
// Modify const parameters
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: fail to modify var int passed to const param", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "var n := 3\n"
        "def a\n"
        "  B(n)\n"
        "end\n"
        "def B(const N) N := 10 end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: fail to modify var int in record passed to const param", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "var r1: R := { a: 1, b: 2, c: 3 }\n"
        "def a\n"
        "  B(r1.c)\n"
        "end\n"
        "def B(const N) N := 10 end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: fail to modify var record passed to const param", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "var r1: R := { a: 1, b: 2, c: 3 }\n"
        "var r2: R := { a: 4, b: 5, c: 6 }\n"
        "def a\n"
        "  B(r1)\n"
        "end\n"
        "def B(const RR: R) RR := r2 end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: fail to modify field of var record passed to const param", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "var r1: R := { a: 1, b: 2, c: 3 }\n"
        "def a\n"
        "  B(r1)\n"
        "end\n"
        "def B(const RR: R) RR.c := 10; RR.a + RR.b + RR.c end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: fail to modify var array passed to const param", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "var ar1 := [ 1, 2, 3 ]\n"
        "var ar2 := [ 4, 5, 6 ]\n"
        "def a\n"
        "  B(ar1)\n"
        "end\n"
        "def B(const ARRAY: [3]) ARRAY := ar2 end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: fail to modify elem of var array passed to const param", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "var ar1 := [ 1, 2, 3 ]\n"
        "def a\n"
        "  B(ar1)\n"
        "end\n"
        "def B(const ARRAY: [2]) ARRAY[1] := 10 end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: fail to modify var array passed to const param open array", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "var ar1 := [ 1, 2, 3 ]\n"
        "var ar2 := [ 4, 5, 6 ]\n"
        "def a\n"
        "  B(ar1)\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY := ar2 end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: fail to modify elem of var array passed to const param open array", "[algoly][ptr-const][negative]" )
{
    const char code[] =
        "var ar1 := [ 1, 2, 3 ]\n"
        "def a\n"
        "  B(ar1)\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[1] := 10 end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}


//----------------------------------------------------------------------------
// Const arguments to const parameters - global
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: pass const int to const param", "[algoly][ptr-const]" )
{
    const char code[] =
        "const n = 3\n"
        "def a\n"
        "  B(n)\n"
        "end\n"
        "def B(const N) N end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: pass const int in record to const param", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "const r1: R = { a: 1, b: 2, c: 3 }\n"
        "def a\n"
        "  B(r1.c)\n"
        "end\n"
        "def B(const N) N end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: pass const int in array to const param, const index", "[algoly][ptr-const]" )
{
    const char code[] =
        "const ar1 = [ 1, 2, 3 ]\n"
        "def a\n"
        "  B(ar1[2])\n"
        "end\n"
        "def B(const N) N end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: pass const int in array to const param, var index", "[algoly][ptr-const]" )
{
    const char code[] =
        "const ar1 = [ 1, 2, 3 ]\n"
        "var i := 2\n"
        "def a\n"
        "  B(ar1[i])\n"
        "end\n"
        "def B(const N) N end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: pass const record to const param", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "const r1: R = { a: 1, b: 2, c: 3 }\n"
        "def a\n"
        "  B(r1)\n"
        "end\n"
        "def B(const RR: R) RR.a + RR.b + RR.c end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 );
}

TEST_CASE( "Algoly: const var array to const param", "[algoly][ptr-const]" )
{
    const char code[] =
        "const ar1 = [ 1, 2, 3 ]\n"
        "def a\n"
        "  B(ar1)\n"
        "end\n"
        "def B(const ARRAY: [2]) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: pass const array to const param open array", "[algoly][ptr-const]" )
{
    const char code[] =
        "const ar1 = [ 1, 2, 3 ]\n"
        "def a\n"
        "  B(ar1)\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[0] + ARRAY[1] + ARRAY[2] end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 );
}

TEST_CASE( "Algoly: pass closed slice of const array to const param open array", "[algoly][ptr-const]" )
{
    const char code[] =
        "const ar1 = [ 1, 2, 3 ]\n"
        "def a\n"
        "  B(ar1[1..3])\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 5 );
}

TEST_CASE( "Algoly: pass const array in array to const param", "[algoly][ptr-const]" )
{
    const char code[] =
        "const ar1 = [[1, 2], [3, 4]]\n"
        "def a\n"
        "  B(ar1[1])\n"
        "end\n"
        "def B(const ARRAY: [2]) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 7 );
}

TEST_CASE( "Algoly: pass const array in array to const param open array", "[algoly][ptr-const]" )
{
    const char code[] =
        "const ar1 = [[1, 2], [3, 4]]\n"
        "def a\n"
        "  B(ar1[1])\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 7 );
}

TEST_CASE( "Algoly: pass closed slice of const array in array to const param open array", "[algoly][ptr-const]" )
{
    const char code[] =
        "const ar1 = [[1, 2, 3], [4, 5, 6]]\n"
        "def a\n"
        "  B(ar1[1][1..3])\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 11 );
}

TEST_CASE( "Algoly: pass const array in array to const param, var index", "[algoly][ptr-const]" )
{
    const char code[] =
        "const ar1 = [[1, 2], [3, 4]]\n"
        "var i := 1\n"
        "def a\n"
        "  B(ar1[i])\n"
        "end\n"
        "def B(const ARRAY: [2]) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 7 );
}

TEST_CASE( "Algoly: pass const array in array to const param open array, var index", "[algoly][ptr-const]" )
{
    const char code[] =
        "const ar1 = [[1, 2], [3, 4]]\n"
        "var i := 1\n"
        "def a\n"
        "  B(ar1[i])\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 7 );
}

TEST_CASE( "Algoly: pass closed slice of const array in array to const param open array, var index", "[algoly][ptr-const]" )
{
    const char code[] =
        "const ar1 = [[1, 2, 3], [4, 5, 6]]\n"
        "var i := 1\n"
        "def a\n"
        "  B(ar1[i][1..3])\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 11 );
}

TEST_CASE( "Algoly: pass const array in array to const param open array, var index array and param", "[algoly][ptr-const]" )
{
    const char code[] =
        "const ar1 = [[1, 2], [3, 4]]\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  B(ar1[one])\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[zero] + ARRAY[one] + countof(ARRAY) end\n"
        ;

    TestCompileAndRunAlgoly( code, 9 );
}


//----------------------------------------------------------------------------
// Const arguments to const parameters - local
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: pass local const int to const param", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a\n"
        "  const n = 3\n"
        "  B(n)\n"
        "end\n"
        "def B(const N) N end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: pass local const int in record to const param", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "def a\n"
        "  const r1: R = { a: 1, b: 2, c: 3 }\n"
        "  B(r1.c)\n"
        "end\n"
        "def B(const N) N end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: pass local const int in array to const param, const index", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a\n"
        "  const ar1 = [ 1, 2, 3 ]\n"
        "  B(ar1[2])\n"
        "end\n"
        "def B(const N) N end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: pass local const int in array to const param, var index", "[algoly][ptr-const]" )
{
    const char code[] =
        "var i := 2\n"
        "def a\n"
        "  const ar1 = [ 1, 2, 3 ]\n"
        "  B(ar1[i])\n"
        "end\n"
        "def B(const N) N end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: pass local const record to const param", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "def a\n"
        "  const r1: R = { a: 1, b: 2, c: 3 }\n"
        "  B(r1)\n"
        "end\n"
        "def B(const RR: R) RR.a + RR.b + RR.c end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 );
}

TEST_CASE( "Algoly: local const var array to const param", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a\n"
        "  const ar1 = [ 1, 2, 3 ]\n"
        "  B(ar1)\n"
        "end\n"
        "def B(const ARRAY: [2]) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: pass local const array to const param open array", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a\n"
        "  const ar1 = [ 1, 2, 3 ]\n"
        "  B(ar1)\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[0] + ARRAY[1] + ARRAY[2] end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 );
}

TEST_CASE( "Algoly: pass closed slice of local const array to const param open array", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a\n"
        "  const ar1 = [ 1, 2, 3 ]\n"
        "  B(ar1[1..3])\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 5 );
}

TEST_CASE( "Algoly: pass local const array in array to const param", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a\n"
        "  const ar1 = [[1, 2], [3, 4]]\n"
        "  B(ar1[1])\n"
        "end\n"
        "def B(const ARRAY: [2]) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 7 );
}

TEST_CASE( "Algoly: pass local const array in array to const param open array", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a\n"
        "  const ar1 = [[1, 2], [3, 4]]\n"
        "  B(ar1[1])\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 7 );
}

TEST_CASE( "Algoly: pass closed slice of local const array in array to const param open array", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a\n"
        "  const ar1 = [[1, 2, 3], [4, 5, 6]]\n"
        "  B(ar1[1][1..3])\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 11 );
}

TEST_CASE( "Algoly: pass local const array in array to const param, var index", "[algoly][ptr-const]" )
{
    const char code[] =
        "var i := 1\n"
        "def a\n"
        "  const ar1 = [[1, 2], [3, 4]]\n"
        "  B(ar1[i])\n"
        "end\n"
        "def B(const ARRAY: [2]) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 7 );
}

TEST_CASE( "Algoly: pass local const array in array to const param open array, var index", "[algoly][ptr-const]" )
{
    const char code[] =
        "var i := 1\n"
        "def a\n"
        "  const ar1 = [[1, 2], [3, 4]]\n"
        "  B(ar1[i])\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 7 );
}

TEST_CASE( "Algoly: pass closed slice of local const array in array to const param open array, var index", "[algoly][ptr-const]" )
{
    const char code[] =
        "var i := 1\n"
        "def a\n"
        "  const ar1 = [[1, 2, 3], [4, 5, 6]]\n"
        "  B(ar1[i][1..3])\n"
        "end\n"
        "def B(const ARRAY: []) ARRAY[0] + ARRAY[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 11 );
}


//----------------------------------------------------------------------------
// Assign const parameters to var locals
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: pass const int in array to const param, var index, assign to local", "[algoly][ptr-const]" )
{
    const char code[] =
        "const ar1 = [ 1, 2, 3 ]\n"
        "var i := 2\n"
        "def a\n"
        "  B(ar1[i])\n"
        "end\n"
        "def B(const N) var n := N; n end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: pass const record to const param, assign to local", "[algoly][ptr-const]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "const r1: R = { a: 1, b: 2, c: 3 }\n"
        "def a\n"
        "  B(r1)\n"
        "end\n"
        "def B(const RR: R) var r := RR; r.a + r.b + r.c end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 );
}

TEST_CASE( "Algoly: const var array to const param, assign to local", "[algoly][ptr-const]" )
{
    const char code[] =
        "const ar1 = [ 1, 2, 3 ]\n"
        "def a\n"
        "  B(ar1)\n"
        "end\n"
        "def B(const ARRAY: [2]) var array := ARRAY; array[0] + array[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: pass const array to const param open array, assign slice to local", "[algoly][ptr-const]" )
{
    const char code[] =
        "const ar1 = [ 1, 2, 3 ]\n"
        "def a\n"
        "  B(ar1)\n"
        "end\n"
        "def B(const ARRAY: []) var array := ARRAY[0..3]; array[0] + array[1] + array[2] end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 );
}


//----------------------------------------------------------------------------
// Pass enum by const reference
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: pass enum member to const param", "[algoly][ptr-const]" )
{
    const char code[] =
        "type E = enum E1=11, E2, E3 end\n"
        "def a\n"
        "  B(E.E3)\n"
        "end\n"
        "def B(const N: E) N as int end\n"
        ;

    TestCompileAndRunAlgoly( code, 13 );
}

// TODO: test assign var param to const param

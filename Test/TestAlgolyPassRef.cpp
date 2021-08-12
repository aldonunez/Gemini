#include "pch.h"
#include "TestBase.h"
#include "../Gemini/LangCommon.h"

using namespace Gemini;


TEST_CASE( "Algoly: PassRef: global pass int by ref", "[algoly][pass-ref]" )
{
    const char code[] =
        "var x := 1\n"
        "def a\n"
        "  B(x)\n"
        "  x\n"
        "end\n"
        "def B(var n) n := n + 3; n end\n"
        ;

    TestCompileAndRunAlgoly( code, 4, 0, 7 );
}

TEST_CASE( "Algoly: PassRef: local pass int by ref", "[algoly][pass-ref]" )
{
    const char code[] =
        "def a\n"
        "  var x := 1\n"
        "  B(x)\n"
        "  x\n"
        "end\n"
        "def B(var n) n := n + 3; n end\n"
        ;

    TestCompileAndRunAlgoly( code, 4, 0, 8 );
}

TEST_CASE( "Algoly: PassRef: local pass enum by ref", "[algoly][pass-ref][enum]" )
{
    const char code[] =
        "type E = enum A=1, B, C end\n"
        "def a -> E\n"
        "  var x := E.B\n"
        "  B(x)\n"
        "  x\n"
        "end\n"
        "def B(var e: E) e := E.C; 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: PassRef: global pass scalar record field by ref", "[algoly][pass-ref][record]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "var r: R := { a: 1, b: 2, c: 3 }\n"
        "def a\n"
        "  B(r.c)\n"
        "  r.c\n"
        "end\n"
        "def B(var p) p := p + 30; 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, 33 );
}

TEST_CASE( "Algoly: PassRef: local pass scalar record field by ref", "[algoly][pass-ref][record]" )
{
    const char code[] =
        "type R = record a, b, c end\n"
        "def a\n"
        "  var r: R := { a: 1, b: 2, c: 3 }\n"
        "  B(r.c)\n"
        "  r.c\n"
        "end\n"
        "def B(var p) p := p + 30; 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, 33 );
}

//----

TEST_CASE( "Algoly: PassRef: literal int by ref", "[algoly][pass-ref][negative]" )
{
    const char code[] =
        "def a\n"
        "  B(5)\n"
        "end\n"
        "def B(var p) 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: PassRef: pass ref param int by ref", "[algoly][pass-ref]" )
{
    const char code[] =
        "def a\n"
        "  var n := 5\n"
        "  B(n)\n"
        "  n\n"
        "end\n"
        "def B(var p) C(p); p end\n"
        "def C(var q) q := 10; 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 10, 0, 11 );
}

TEST_CASE( "Algoly: PassRef: pass value param int by ref", "[algoly][pass-ref]" )
{
    const char code[] =
        "def a\n"
        "  var n := 5\n"
        "  B(n)\n"
        "end\n"
        "def B(p) C(p); p end\n"
        "def C(var q) q := 10; 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 10, 0, 11 );
}

TEST_CASE( "Algoly: PassRef: global pass &proc by ref", "[algoly][pass-ref]" )
{
    const char code[] =
        "var f := &C\n"
        "def a\n"
        "  B(f)\n"
        "  (f)()\n"
        "end\n"
        "def B(var f: &proc) f := &D; 0 end\n"
        "def C 3 end\n"
        "def D 4 end\n"
        ;

    TestCompileAndRunAlgoly( code, 4, 0, 7 );
}

TEST_CASE( "Algoly: PassRef: local pass &proc by ref", "[algoly][pass-ref]" )
{
    const char code[] =
        "def a\n"
        "  var f := &C\n"
        "  B(f)\n"
        "  (f)()\n"
        "end\n"
        "def B(var f: &proc) f := &D; 0 end\n"
        "def C 3 end\n"
        "def D 4 end\n"
        ;

    TestCompileAndRunAlgoly( code, 4, 0, 8 );
}

TEST_CASE( "Algoly: PassRef: global pass array elem, const index", "[algoly][pass-ref]" )
{
    const char code[] =
        "var ar: [2] := [1, 2]\n"
        "def a\n"
        "  B(ar[1])\n"
        "  ar[1]\n"
        "end\n"
        "def B(var n) n := n + 3; 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, 5, 0, 7 );
}

TEST_CASE( "Algoly: PassRef: local pass array elem, const index", "[algoly][pass-ref]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [2] := [1, 2]\n"
        "  B(ar[1])\n"
        "  ar[1]\n"
        "end\n"
        "def B(var n) n := n + 3; 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, 5, 0, 9 );
}

TEST_CASE( "Algoly: PassRef: global pass array elem, var index", "[algoly][pass-ref]" )
{
    const char code[] =
        "var i := 1\n"
        "var ar: [2] := [1, 2]\n"
        "def a\n"
        "  B(ar[i])\n"
        "  ar[i]\n"
        "end\n"
        "def B(var n) n := n + 3; 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, 5, 0, 7 );
}

TEST_CASE( "Algoly: PassRef: local pass array elem, var index", "[algoly][pass-ref]" )
{
    const char code[] =
        "var i := 1\n"
        "def a\n"
        "  var ar: [2] := [1, 2]\n"
        "  B(ar[i])\n"
        "  ar[i]\n"
        "end\n"
        "def B(var n) n := n + 3; 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, 5, 0, 9 );
}

//-------

TEST_CASE( "Algoly: PassRef: global countof closed array by ref", "[algoly][pass-ref]" )
{
    const char code[] =
        "var ar: [2] := [3, 4]\n"
        "def a\n"
        "  B(ar)\n"
        "end\n"
        "def B(var arr: [2]) countof( arr ) end\n"
        ;

    TestCompileAndRunAlgoly( code, 2, 0, 6 );
}

TEST_CASE( "Algoly: PassRef: local countof closed array by ref", "[algoly][pass-ref]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [2] := [3, 4]\n"
        "  B(ar)\n"
        "end\n"
        "def B(var arr: [2]) countof( arr ) end\n"
        ;

    TestCompileAndRunAlgoly( code, 2, 0, 8 );
}

TEST_CASE( "Algoly: PassRef: global modify closed array by ref", "[algoly][pass-ref]" )
{
    const char code[] =
        "var ar: [2] := [3, 4]\n"
        "def a\n"
        "  B(ar)\n"
        "  ar[1]\n"
        "end\n"
        "def B(var arr: [2]) arr[1] := arr[0]; 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, 3, 0, 8 );
}

TEST_CASE( "Algoly: PassRef: local modify closed array by ref", "[algoly][pass-ref]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [2] := [3, 4]\n"
        "  B(ar)\n"
        "  ar[1]\n"
        "end\n"
        "def B(var arr: [2]) arr[1] := arr[0]; 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, 3, 0, 10 );
}

TEST_CASE( "Algoly: PassRef: global modify closed sliced array by ref", "[algoly][pass-ref]" )
{
    const char code[] =
        "var ar: [3] := [3, 4, 5]\n"
        "def a\n"
        "  B(ar[1..3])\n"
        "  ar[2]\n"
        "end\n"
        "def B(var arr: [2]) arr[1] := arr[0]; 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, 4, 0, 8 );
}

TEST_CASE( "Algoly: PassRef: local modify closed sliced array by ref", "[algoly][pass-ref]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [3] := [3, 4, 5]\n"
        "  B(ar[1..3])\n"
        "  ar[2]\n"
        "end\n"
        "def B(var arr: [2]) arr[1] := arr[0]; 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, 4, 0, 11 );
}

//----

TEST_CASE( "Algoly: PassRef: global array call chain by ref", "[algoly][pass-ref]" )
{
    const char code[] =
        "var ar: [3] := [3, 4, 5]\n"
        "def a\n"
        "  B(ar)\n"
        "  ar[0] + ar[1] + ar[2]\n"
        "end\n"
        "def B(var arr: [3]) C(arr); arr[0] := arr[1]; 0 end\n"
        "def C(var arr: [3]) arr[1] := arr[2]; 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, 15, 0, 11 );
}

TEST_CASE( "Algoly: PassRef: local array call chain by ref", "[algoly][pass-ref]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [3] := [3, 4, 5]\n"
        "  B(ar)\n"
        "  ar[0] + ar[1] + ar[2]\n"
        "end\n"
        "def B(var arr: [3]) C(arr); arr[0] := arr[1]; 0 end\n"
        "def C(var arr: [3]) arr[1] := arr[2]; 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, 15, 0, 11+3 );
}

TEST_CASE( "Algoly: PassRef: global slice array call chain by ref", "[algoly][pass-ref]" )
{
    const char code[] =
        "var ar: [4] := [3, 4, 5, 6]\n"
        "def a\n"
        "  B(ar[1..4])\n"
        "  ar[0] + ar[1] + ar[2] + ar[3]\n"
        "end\n"
        "def B(var arr: [3]) arr[1] := arr[0]; C(arr[1..3]); 0 end\n"
        "def C(var arr: [2]) arr[1] := arr[0]; 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, 15, 0, 11 );
}

TEST_CASE( "Algoly: PassRef: local slice array call chain by ref", "[algoly][pass-ref]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [4] := [3, 4, 5, 6]\n"
        "  B(ar[1..4])\n"
        "  ar[0] + ar[1] + ar[2] + ar[3]\n"
        "end\n"
        "def B(var arr: [3]) arr[1] := arr[0]; C(arr[1..3]); 0 end\n"
        "def C(var arr: [2]) arr[1] := arr[0]; 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, 15, 0, 11+4 );
}

TEST_CASE( "Algoly: PassRef: global slice array call chain by ref, var indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "var zero := 0, one := 1\n"
        "var ar: [4] := [3, 4, 5, 6]\n"
        "def a\n"
        "  B(ar[1..4])\n"
        "  ar[0] + ar[1] + ar[2] + ar[3]\n"
        "end\n"
        "def B(var arr: [3])\n"
        "  arr[one] := arr[zero]; C(arr[1..3]); 0\n"
        "end\n"
        "def C(var arr: [2])\n"
        "  arr[one] := arr[zero]; 0\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 15, 0, 11 );
}

TEST_CASE( "Algoly: PassRef: local slice array call chain by ref, var indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "var zero := 0, one := 1\n"
        "def a\n"
        "  var ar: [4] := [3, 4, 5, 6]\n"
        "  B(ar[1..4])\n"
        "  ar[0] + ar[1] + ar[2] + ar[3]\n"
        "end\n"
        "def B(var arr: [3])\n"
        "  arr[one] := arr[zero]; C(arr[1..3]); 0\n"
        "end\n"
        "def C(var arr: [2])\n"
        "  arr[one] := arr[zero]; 0\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 15, 0, 11 + 4 );
}

TEST_CASE( "Algoly: PassRef: global array to fptr taking ref", "[algoly][pass-ref]" )
{
    // The called function has a deeper expr depth
    const char code[] =
        "var ar: [2] := [3, 4]\n"
        "var f: &proc(var [2]) := &B\n"
        "def a\n"
        "  (f)(ar)\n"
        "  ar[1] + ar[0]\n"
        "end\n"
        "def B(var arr: [2]) arr[1] := arr[0]; 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, 6, 0, 6 );
}

TEST_CASE( "Algoly: PassRef: local array to fptr taking ref", "[algoly][pass-ref]" )
{
    // Both functions have he same expr depth
    const char code[] =
        "var f: &proc(var [2]) := &B\n"
        "def a\n"
        "  var ar: [2] := [3, 4]\n"
        "  (f)(ar)\n"
        "  ar[1] + ar[0]\n"
        "end\n"
        "def B(var arr: [2]) arr[1] := arr[0]; 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, 6, 0, 6 );
}

//---

TEST_CASE( "Algoly: PassRef: global pass whole part of multidim by ref, const indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  B(ar[1])\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        "def B(var array: [2])\n"
        "  array[1] := 99\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 105, 0, 9 );
}

TEST_CASE( "Algoly: PassRef: local pass whole part of multidim by ref, const indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  B(ar[1])\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        "def B(var array: [2])\n"
        "  array[1] := 99\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 105, 0, 9+4 );
}

TEST_CASE( "Algoly: PassRef: global pass whole part of multidim by ref, var indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "var n := 1\n"
        "def a\n"
        "  B(ar[n])\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        "def B(var array: [2])\n"
        "  array[n] := 99\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 105, 0, 9 );
}

TEST_CASE( "Algoly: PassRef: local pass whole part of multidim by ref, var indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "var n := 1\n"
        "def a\n"
        "  var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  B(ar[n])\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        "def B(var array: [2])\n"
        "  array[n] := 99\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 105, 0, 9 + 4 );
}

//---

TEST_CASE( "Algoly: PassRef: global pass slice part of multidim by ref, const indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "var ar: [3] of [2] := [[1, 2], [3, 4], [5, 6]]\n"
        "def a\n"
        "  B(ar[1..3])\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1] + ar[2][0] + ar[2][1]\n"
        "end\n"
        "def B(var array: [2] of [2])\n"
        "  array[1][1] := 99\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 114, 0, 9 );
}

TEST_CASE( "Algoly: PassRef: local pass slice part of multidim by ref, const indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [3] of [2] := [[1, 2], [3, 4], [5, 6]]\n"
        "  B(ar[1..3])\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1] + ar[2][0] + ar[2][1]\n"
        "end\n"
        "def B(var array: [2] of [2])\n"
        "  array[1][1] := 99\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 114, 0, 9 + 6 );
}

TEST_CASE( "Algoly: PassRef: global pass slice part of multidim by ref, var indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "var n := 1\n"
        "var ar: [3] of [2] := [[1, 2], [3, 4], [5, 6]]\n"
        "def a\n"
        "  B(ar[1..3])\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1] + ar[2][0] + ar[2][1]\n"
        "end\n"
        "def B(var array: [2] of [2])\n"
        "  array[n][n] := 99\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 114, 0, 9 );
}

TEST_CASE( "Algoly: PassRef: local pass slice part of multidim by ref, var indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "var n := 1\n"
        "def a\n"
        "  var ar: [3] of [2] := [[1, 2], [3, 4], [5, 6]]\n"
        "  B(ar[1..3])\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1] + ar[2][0] + ar[2][1]\n"
        "end\n"
        "def B(var array: [2] of [2])\n"
        "  array[n][n] := 99\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 114, 0, 9 + 6 );
}

//---

TEST_CASE( "Algoly: PassRef: global pass whole part of slice part of multidim by ref, const indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "var ar: [3] of [2] := [[1, 2], [3, 4], [5, 6]]\n"
        "def a\n"
        "  B(ar[1..3][1])\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1] + ar[2][0] + ar[2][1]\n"
        "end\n"
        "def B(var array: [2])\n"
        "  array[1] := 99\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 114, 0, 9 );
}

TEST_CASE( "Algoly: PassRef: local pass whole part of slice part of multidim by ref, const indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [3] of [2] := [[1, 2], [3, 4], [5, 6]]\n"
        "  B(ar[1..3][1])\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1] + ar[2][0] + ar[2][1]\n"
        "end\n"
        "def B(var array: [2])\n"
        "  array[1] := 99\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 114, 0, 9 + 6 );
}

TEST_CASE( "Algoly: PassRef: global pass whole part of slice part of multidim by ref, var indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "var n := 1\n"
        "var ar: [3] of [2] := [[1, 2], [3, 4], [5, 6]]\n"
        "def a\n"
        "  B(ar[1..3][n])\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1] + ar[2][0] + ar[2][1]\n"
        "end\n"
        "def B(var array: [2])\n"
        "  array[n] := 99\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 114, 0, 9 );
}

TEST_CASE( "Algoly: PassRef: local pass whole part of slice part of multidim by ref, var indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "var n := 1\n"
        "def a\n"
        "  var ar: [3] of [2] := [[1, 2], [3, 4], [5, 6]]\n"
        "  B(ar[1..3][n])\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1] + ar[2][0] + ar[2][1]\n"
        "end\n"
        "def B(var array: [2])\n"
        "  array[n] := 99\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 114, 0, 9 + 6 );
}

//---

TEST_CASE( "Algoly: PassRef: global pass slice part of whole part of multidim by ref, const indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "var ar: [2] of [3] := [[1, 2, 3], [4, 5, 6]]\n"
        "def a\n"
        "  B(ar[1][1..3])\n"
        "  ar[0][0] + ar[0][1] + ar[0][2] + ar[1][0] + ar[1][1] + ar[1][2]\n"
        "end\n"
        "def B(var array: [2])\n"
        "  array[1] := 99\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 114, 0, 9 );
}

TEST_CASE( "Algoly: PassRef: local pass slice part of whole part of multidim by ref, const indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [2] of [3] := [[1, 2, 3], [4, 5, 6]]\n"
        "  B(ar[1][1..3])\n"
        "  ar[0][0] + ar[0][1] + ar[0][2] + ar[1][0] + ar[1][1] + ar[1][2]\n"
        "end\n"
        "def B(var array: [2])\n"
        "  array[1] := 99\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 114, 0, 9 + 6 );
}

TEST_CASE( "Algoly: PassRef: global pass slice part of whole part of multidim by ref, var indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "var n := 1\n"
        "var ar: [2] of [3] := [[1, 2, 3], [4, 5, 6]]\n"
        "def a\n"
        "  B(ar[n][1..3])\n"
        "  ar[0][0] + ar[0][1] + ar[0][2] + ar[1][0] + ar[1][1] + ar[1][2]\n"
        "end\n"
        "def B(var array: [2])\n"
        "  array[n] := 99\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 114, 0, 9 );
}

TEST_CASE( "Algoly: PassRef: local pass slice part of whole part of multidim by ref, var indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "var n := 1\n"
        "def a\n"
        "  var ar: [2] of [3] := [[1, 2, 3], [4, 5, 6]]\n"
        "  B(ar[n][1..3])\n"
        "  ar[0][0] + ar[0][1] + ar[0][2] + ar[1][0] + ar[1][1] + ar[1][2]\n"
        "end\n"
        "def B(var array: [2])\n"
        "  array[n] := 99\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 114, 0, 9 + 6 );
}

//---

TEST_CASE( "Algoly: PassRef: global pass element of slice part of whole part of multidim by ref, var indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "var n := 1\n"
        "var ar: [2] of [3] := [[1, 2, 3], [4, 5, 6]]\n"
        "def a\n"
        "  B(ar[n][1..3][n])\n"
        "  ar[0][0] + ar[0][1] + ar[0][2] + ar[1][0] + ar[1][1] + ar[1][2]\n"
        "end\n"
        "def B(var x)\n"
        "  x := 99\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 114, 0, 8 );
}

TEST_CASE( "Algoly: PassRef: local pass element of slice part of whole part of multidim by ref, var indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "var n := 1\n"
        "def a\n"
        "  var ar: [2] of [3] := [[1, 2, 3], [4, 5, 6]]\n"
        "  B(ar[n][1..3][n])\n"
        "  ar[0][0] + ar[0][1] + ar[0][2] + ar[1][0] + ar[1][1] + ar[1][2]\n"
        "end\n"
        "def B(var x)\n"
        "  x := 99\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 114, 0, 8 + 6 );
}

//---

TEST_CASE( "Algoly: PassRef: global pass whole part of 2x2x2 array by ref, var indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "var i := 1\n"
        "var ar: [2] of [2] of [2] :=\n"
        "    [\n"
        "      [[1, 2], [3, 4]],\n"
        "      [[5, 6], [7, 8]]\n"
        "    ]\n"
        "def a\n"
        "  B(ar[i])\n"
        "  ar[1][1][1]\n"
        "end\n"
        "def B(var array: [2] of [2])\n"
        "  array[1][1] := 99\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 99, 0, 9 );
}

TEST_CASE( "Algoly: PassRef: global pass int element of 2x2x2 array by ref, var indexes", "[algoly][pass-ref]" )
{
    const char code[] =
        "var i := 1\n"
        "var ar: [2] of [2] of [2] :=\n"
        "    [\n"
        "      [[1, 2], [3, 4]],\n"
        "      [[5, 6], [7, 8]]\n"
        "    ]\n"
        "def a\n"
        "  B(ar[i][i][i])\n"
        "  ar[1][1][1]\n"
        "end\n"
        "def B(var x)\n"
        "  x := 99\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 99, 0, 8 );
}


//----------------------------------------------------------------------------
//  Open array
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: PassRef: global countof open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var ar: [3] := [1, 2, 3]\n"
        "def a\n"
        "  B( ar )\n"
        "end\n"
        "def B(var array: [])\n"
        "  countof( array )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 3, 0, 7 );
}

TEST_CASE( "Algoly: PassRef: local countof open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [3] := [1, 2, 3]\n"
        "  B( ar )\n"
        "end\n"
        "def B(var array: [])\n"
        "  countof( array )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 3, 0, 7+3 );
}

TEST_CASE( "Algoly: PassRef: global modify open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var ar: [3] := [1, 2, 3]\n"
        "def a\n"
        "  B( ar )\n"
        "  ar[0] + ar[1] + ar[2]\n"
        "end\n"
        "def B(var array: [])\n"
        "  array[2] := 10\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 13, 0, 10 );
}

TEST_CASE( "Algoly: PassRef: local modify open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [3] := [1, 2, 3]\n"
        "  B( ar )\n"
        "  ar[0] + ar[1] + ar[2]\n"
        "end\n"
        "def B(var array: [])\n"
        "  array[2] := 10\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 13, 0, 13 );
}

TEST_CASE( "Algoly: PassRef: global modify open array, var index", "[algoly][pass-ref]" )
{
    const char code[] =
        "var two := 2\n"
        "var ar: [3] := [1, 2, 3]\n"
        "def a\n"
        "  B( ar )\n"
        "  ar[0] + ar[1] + ar[2]\n"
        "end\n"
        "def B(var array: [])\n"
        "  array[two] := 10\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 13, 0, 10 );
}

TEST_CASE( "Algoly: PassRef: local modify open array, var index", "[algoly][pass-ref]" )
{
    const char code[] =
        "var two := 2\n"
        "def a\n"
        "  var ar: [3] := [1, 2, 3]\n"
        "  B( ar )\n"
        "  ar[0] + ar[1] + ar[2]\n"
        "end\n"
        "def B(var array: [])\n"
        "  array[two] := 10\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 13, 0, 13 );
}

TEST_CASE( "Algoly: PassRef: global modify open array passed to fptr, var index", "[algoly][pass-ref]" )
{
    // Main: 6, B: 9
    const char code[] =
        "var f: &proc(int, var []) := &B\n"
        "var ar: [3] := [1, 2, 3]\n"
        "def a\n"
        "  (f)( 2, ar )\n"
        "  ar[0] + ar[1] + ar[2]\n"
        "end\n"
        "def B(n, var array: [])\n"
        "  array[n] := 10\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 13, 0, 9 );
}

TEST_CASE( "Algoly: PassRef: global modify open array of slice", "[algoly][pass-ref]" )
{
    const char code[] =
        "var ar: [3] := [1, 2, 3]\n"
        "def a\n"
        "  B( ar[1..3] )\n"
        "  ar[0] + ar[1] + ar[2]\n"
        "end\n"
        "def B(var array: [])\n"
        "  array[1] := 10\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 13, 0, 10 );
}

// TODO: change all of these so that they read and write the reference array in the callees

TEST_CASE( "Algoly: PassRef: local modify open array of slice", "[algoly][pass-ref]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [3] := [1, 2, 3]\n"
        "  B( ar[1..3] )\n"
        "  ar[0] + ar[1] + ar[2]\n"
        "end\n"
        "def B(var array: [])\n"
        "  array[1] := 10\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 13, 0, 13 );
}

TEST_CASE( "Algoly: PassRef: global modify open array of 2x2", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  c := B(ar)\n"
        "  ar[1][1] + c\n"
        "end\n"
        "def B( var arr: [] of [2] )\n" // 6
        "  arr[1][1] := 99\n"
        "  countof( arr )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 99+2, 0, 9 );
}

TEST_CASE( "Algoly: PassRef: local modify open array of 2x2", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "def a\n"
        "  var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  c := B(ar)\n"
        "  ar[1][1] + c\n"
        "end\n"
        "def B( var arr: [] of [2] )\n"
        "  arr[1][1] := 99\n"
        "  countof( arr )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 99+2, 0, 9 + 4 );
}


//----------------------------------------------------------------------------
//  Chain of calls passing open arrays
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: PassRef: global pass closed array open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "var ar: [3] := [3, 4, 5]\n"
        "def a\n"
        "  c := B(ar)\n"
        "  ar[0] + ar[1] + ar[2] + c\n"
        "end\n"
        "def B( var arr: [3]) \n"    // 5
        "  arr[1] := arr[0]\n"
        "  C( arr )\n"
        "end\n"
        "def C( var array: [] )\n"  // 9
        "  array[2] := array[1]\n"
        "  countof( array )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 12, 0, 12 );
}

TEST_CASE( "Algoly: PassRef: local pass closed array open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "def a\n"
        "  var ar: [3] := [3, 4, 5]\n"
        "  c := B(ar)\n"
        "  ar[0] + ar[1] + ar[2] + c\n"
        "end\n"
        "def B( var arr: [3]) \n"    // 5
        "  arr[1] := arr[0]\n"
        "  C( arr )\n"
        "end\n"
        "def C( var array: [] )\n"  // 9
        "  array[2] := array[1]\n"
        "  countof( array )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 12, 0, 12 + 3 );
}

TEST_CASE( "Algoly: PassRef: global pass open array open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "var ar: [3] := [3, 4, 5]\n"
        "def a\n"
        "  c := B(ar)\n"
        "  ar[0] + ar[1] + ar[2] + c\n"
        "end\n"
        "def B( var arr: []) \n"    // 6
        "  arr[1] := arr[0]\n"
        "  C( arr )\n"
        "end\n"
        "def C( var array: [] )\n"  // 10
        "  array[2] := array[1]\n"
        "  countof( array )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 12, 0, 13 );
}

TEST_CASE( "Algoly: PassRef: local pass open array open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "def a\n"
        "  var ar: [3] := [3, 4, 5]\n"
        "  c := B(ar)\n"
        "  ar[0] + ar[1] + ar[2] + c\n"
        "end\n"
        "def B( var arr: []) \n"    // 6
        "  arr[1] := arr[0]\n"
        "  C( arr )\n"
        "end\n"
        "def C( var array: [] )\n"  // 10
        "  array[2] := array[1]\n"
        "  countof( array )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 12, 0, 13+3 );
}

TEST_CASE( "Algoly: PassRef: global pass open array slice open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "var ar: [3] := [3, 4, 5]\n"
        "def a\n"
        "  c := B(ar)\n"
        "  ar[0] + ar[1] + ar[2] + c\n"
        "end\n"
        "def B( var arr: []) \n"    // 6
        "  arr[1] := arr[0]\n"
        "  C( arr[1..3] )\n"
        "end\n"
        "def C( var array: [] )\n"  // 10
        "  array[1] := array[0]\n"
        "  countof( array )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 11, 0, 13 );
}

TEST_CASE( "Algoly: PassRef: local pass open array slice open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "def a\n"
        "  var ar: [3] := [3, 4, 5]\n"
        "  c := B(ar)\n"
        "  ar[0] + ar[1] + ar[2] + c\n"
        "end\n"
        "def B( var arr: []) \n"    // 6
        "  arr[1] := arr[0]\n"
        "  C( arr[1..3] )\n"
        "end\n"
        "def C( var array: [] )\n"  // 10
        "  array[1] := array[0]\n"
        "  countof( array )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 11, 0, 13 + 3 );
}


//----------------------------------------------------------------------------
//  Chain of calls passing open arrays of arrays: 1 dim ref array
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: PassRef: global pass whole part to closed array open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "var ar: [3] of [3] := [[3, 4, 5], [6, 7, 8], [9, 10, 11]]\n"
        "def a\n"
        "  c := B(ar[2])\n"
        "  ar[2][0] + ar[2][1] + ar[2][2] + c\n"
        "end\n"
        "def B( var arr: [3]) \n"    // 5
        "  arr[1] := arr[0]\n"
        "  C( arr )\n"
        "end\n"
        "def C( var array: [] )\n"  // 9
        "  array[2] := array[1]\n"
        "  countof( array )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30, 0, 12 );
}

TEST_CASE( "Algoly: PassRef: local pass whole part to closed array open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "def a\n"
        "  var ar: [3] of [3] := [[3, 4, 5], [6, 7, 8], [9, 10, 11]]\n"
        "  c := B(ar[2])\n"
        "  ar[2][0] + ar[2][1] + ar[2][2] + c\n"
        "end\n"
        "def B( var arr: [3]) \n"    // 5
        "  arr[1] := arr[0]\n"
        "  C( arr )\n"
        "end\n"
        "def C( var array: [] )\n"  // 9
        "  array[2] := array[1]\n"
        "  countof( array )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30, 0, 18 + 3 );
}

TEST_CASE( "Algoly: PassRef: global pass whole part to open array open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "var ar: [3] of [3] := [[3, 4, 5], [6, 7, 8], [9, 10, 11]]\n"
        "def a\n"
        "  c := B(ar[2])\n"
        "  ar[2][0] + ar[2][1] + ar[2][2] + c\n"
        "end\n"
        "def B( var arr: []) \n"    // 6
        "  arr[1] := arr[0]\n"
        "  C( arr )\n"
        "end\n"
        "def C( var array: [] )\n"  // 10
        "  array[2] := array[1]\n"
        "  countof( array )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30, 0, 13 );
}

TEST_CASE( "Algoly: PassRef: local pass whole part to open array open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "def a\n"
        "  var ar: [3] of [3] := [[3, 4, 5], [6, 7, 8], [9, 10, 11]]\n"
        "  c := B(ar[2])\n"
        "  ar[2][0] + ar[2][1] + ar[2][2] + c\n"
        "end\n"
        "def B( var arr: []) \n"    // 6
        "  arr[1] := arr[0]\n"
        "  C( arr )\n"
        "end\n"
        "def C( var array: [] )\n"  // 10
        "  array[2] := array[1]\n"
        "  countof( array )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30, 0, 19 + 3 );
}

TEST_CASE( "Algoly: PassRef: global pass whole part to open array slice open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "var ar: [3] of [3] := [[3, 4, 5], [6, 7, 8], [9, 10, 11]]\n"
        "def a\n"
        "  c := B(ar[2])\n"
        "  ar[2][0] + ar[2][1] + ar[2][2] + c\n"
        "end\n"
        "def B( var arr: []) \n"    // 6
        "  arr[1] := arr[0]\n"
        "  C( arr[1..3] )\n"
        "end\n"
        "def C( var array: [] )\n"  // 10
        "  array[1] := array[0]\n"
        "  countof( array )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 29, 0, 13 );
}

TEST_CASE( "Algoly: PassRef: local pass whole part to open array slice open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "def a\n"
        "  var ar: [3] of [3] := [[3, 4, 5], [6, 7, 8], [9, 10, 11]]\n"
        "  c := B(ar[2])\n"
        "  ar[2][0] + ar[2][1] + ar[2][2] + c\n"
        "end\n"
        "def B( var arr: []) \n"    // 6
        "  arr[1] := arr[0]\n"
        "  C( arr[1..3] )\n"
        "end\n"
        "def C( var array: [] )\n"  // 10
        "  array[1] := array[0]\n"
        "  countof( array )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 29, 0, 13 + 9 );
}


//----------------------------------------------------------------------------
//  Chain of calls passing open arrays of arrays: 2 dim ref array
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: PassRef: global pass whole to closed array open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "var ar: [3] of [3] := [[3, 4, 5], [6, 7, 8], [9, 10, 11]]\n"
        "def a\n"
        "  c := B(ar)\n"
        "  ar[2][0] + ar[2][1] + ar[2][2] + c\n"
        "end\n"
        "def B( var arr: [3] of [3]) \n"    // 5
        "  arr[2][1] := arr[2][0]\n"
        "  C( arr )\n"
        "end\n"
        "def C( var array: [] of [3])\n"  // 9
        "  array[2][2] := array[2][1]\n"
        "  countof( array )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30, 0, 12 );
}

TEST_CASE( "Algoly: PassRef: local pass whole to closed array open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "def a\n"
        "  var ar: [3] of [3] := [[3, 4, 5], [6, 7, 8], [9, 10, 11]]\n"
        "  c := B(ar)\n"
        "  ar[2][0] + ar[2][1] + ar[2][2] + c\n"
        "end\n"
        "def B( var arr: [3] of [3]) \n"    // 5
        "  arr[2][1] := arr[2][0]\n"
        "  C( arr )\n"
        "end\n"
        "def C( var array: [] of [3])\n"  // 9
        "  array[2][2] := array[2][1]\n"
        "  countof( array )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30, 0, 18 + 3 );
}

TEST_CASE( "Algoly: PassRef: global pass whole to open array open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "var ar: [3] of [3] := [[3, 4, 5], [6, 7, 8], [9, 10, 11]]\n"
        "def a\n"
        "  c := B(ar)\n"
        "  ar[2][0] + ar[2][1] + ar[2][2] + c\n"
        "end\n"
        "def B( var arr: [] of [3]) \n"    // 6
        "  arr[2][1] := arr[2][0]\n"
        "  C( arr )\n"
        "end\n"
        "def C( var array: [] of [3])\n"  // 10
        "  array[2][2] := array[2][1]\n"
        "  countof( array )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30, 0, 13 );
}

TEST_CASE( "Algoly: PassRef: local pass whole to open array open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "def a\n"
        "  var ar: [3] of [3] := [[3, 4, 5], [6, 7, 8], [9, 10, 11]]\n"
        "  c := B(ar)\n"
        "  ar[2][0] + ar[2][1] + ar[2][2] + c\n"
        "end\n"
        "def B( var arr: [] of [3]) \n"    // 6
        "  arr[2][1] := arr[2][0]\n"
        "  C( arr )\n"
        "end\n"
        "def C( var array: [] of [3])\n"  // 10
        "  array[2][2] := array[2][1]\n"
        "  countof( array )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30, 0, 19 + 3 );
}

TEST_CASE( "Algoly: PassRef: global pass whole to open array slice open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "var ar: [3] of [3] := [[3, 4, 5], [6, 7, 8], [9, 10, 11]]\n"
        "def a\n"
        "  c := B(ar)\n"
        "  ar[2][0] + ar[2][1] + ar[2][2] + c\n"
        "end\n"
        "def B( var arr: [] of [3]) \n"    // 6
        "  arr[2][1] := arr[2][0]\n"
        "  C( arr[1..3] )\n"
        "end\n"
        "def C( var array: [] of [3])\n"  // 10
        "  array[1][2] := array[1][1]\n"
        "  countof( array )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 29, 0, 13 );
}

TEST_CASE( "Algoly: PassRef: local pass whole to open array slice open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "def a\n"
        "  var ar: [3] of [3] := [[3, 4, 5], [6, 7, 8], [9, 10, 11]]\n"
        "  c := B(ar)\n"
        "  ar[2][0] + ar[2][1] + ar[2][2] + c\n"
        "end\n"
        "def B( var arr: [] of [3]) \n"    // 6
        "  arr[2][1] := arr[2][0]\n"
        "  C( arr[1..3] )\n"
        "end\n"
        "def C( var array: [] of [3])\n"  // 10
        "  array[1][2] := array[1][1]\n"
        "  countof( array )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 29, 0, 13 + 9 );
}

TEST_CASE( "Algoly: PassRef: countof global pass whole to open array slice open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "var ar: [3] of [3] := [[3, 4, 5], [6, 7, 8], [9, 10, 11]]\n"
        "def a\n"
        "  c := B(ar)\n"
        "  ar[2][0] + ar[2][1] + ar[2][2] + c\n"
        "end\n"
        "def B( var arr: [] of [3]) \n"    // 6
        "  C( arr[1..3] ) + \n"
        "  countof( arr )*100 + countof( arr )*1000\n"
        "end\n"
        "def C( var array: [] of [3])\n"  // 10
        "  countof( array ) + countof( array[0] )*10\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30+300+3000+2+30, 0, 12 );
}

TEST_CASE( "Algoly: PassRef: countof local pass whole to open array slice open array", "[algoly][pass-ref]" )
{
    const char code[] =
        "var c := 0\n"
        "def a\n"
        "  var ar: [3] of [3] := [[3, 4, 5], [6, 7, 8], [9, 10, 11]]\n"
        "  c := B(ar)\n"
        "  ar[2][0] + ar[2][1] + ar[2][2] + c\n"
        "end\n"
        "def B( var arr: [] of [3]) \n"    // 6
        "  C( arr[1..3] ) + \n"
        "  countof( arr )*100 + countof( arr )*1000\n"
        "end\n"
        "def C( var array: [] of [3])\n"  // 10
        "  countof( array ) + countof( array[0] )*10\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30+300+3000+2+30, 0, 12 + 9 );
}

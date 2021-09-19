#include "pch.h"
#include "TestBase.h"


/*
* TODO: test:
*   locals initialized to zero
*   multi dim arrays
*   slice
*   countof
*
* look at b3fd9ae09d3da3cfcae76c095a000547ce71fdfe
*   If a statement list doesn't end in an expr with statement type, then add 0
* 
* negative tests
*   for example for "Don't allow uninitialized pointers even in a complex initializer"
*/


TEST_CASE( "Algoly: little number", "[algoly]" )
{
    const char code[] =
        "def a 33 end\n"
        ;

    TestCompileAndRunAlgoly( code, 33 );
}

TEST_CASE( "Algoly: infer number", "[algoly]" )
{
    const char code[] =
        "var x := 33\n"
        "def a x end\n"
        ;

    TestCompileAndRunAlgoly( code, 33 );
}

TEST_CASE( "Algoly: infer fptr", "[algoly]" )
{
    const char code[] =
        "var f := &B\n"
        "def a; (f)() end\n"
        "def B 99 end\n"
        ;

    TestCompileAndRunAlgoly( code, 99 );
}

TEST_CASE( "Algoly: infer array", "[algoly]" )
{
    const char code[] =
        "var ar := [1, 2, 3]\n"
        "def a ar[0] + ar[1] + ar[2] + countof(ar) end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 + 3 );
}

TEST_CASE( "Algoly: infer array repeat", "[algoly]" )
{
    const char code[] =
        "var ar := [1, 2, 3...]\n"
        "def a ar[0] + ar[1] + ar[2] + countof(ar) end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 + 3 );
}

TEST_CASE( "Algoly: infer array of array", "[algoly]" )
{
    const char code[] =
        "var ar := [[1, 2], [3, 4], [5, 6]]\n"
        "def a\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1] + ar[2][0] + ar[2][1] + \n"
        "  countof(ar) + countof(ar[0])*10\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 21 + 3 + 20 );
}

TEST_CASE( "Algoly: infer array of array repeat", "[algoly]" )
{
    const char code[] =
        "var ar := [[1, 2], [3, 4], [5, 6]...]\n"
        "def a\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1] + ar[2][0] + ar[2][1] + \n"
        "  countof(ar) + countof(ar[0])*10\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 21 + 3 + 20 );
}

TEST_CASE( "Algoy: 2 funcs, call predefine", "[algoly]" )
{
    const char code[] =
        "def B 33 end\n"
        "def a B() end\n"
        ;

    TestCompileAndRunAlgoly( code, 33 );
}

TEST_CASE( "Algoy: 2 funcs, call postdefine", "[algoly]" )
{
    const char code[] =
        "def a B() end\n"
        "def B 33 end\n"
        ;

    TestCompileAndRunAlgoly( code, 33 );
}

TEST_CASE( "Algoy: func with param", "[algoly]" )
{
    const char code[] =
        "def a B(30) end\n"
        "def B(n) n+3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 33 );
}

TEST_CASE( "Algoy: func with param, assign to param", "[algoly]" )
{
    const char code[] =
        "def a B(30) end\n"
        "def B(n) n := 90; n+3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 93 );
}

TEST_CASE( "Global array ref in binary expr", "[algoly]" )
{
    const char code[] =
        "var b := 2\n"
        "var c := 3\n"
        "var d: [3] := [10, 20, 30]\n"
        "def a() 9; b + c + d[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 25 );
}

TEST_CASE( "Global array implicit", "[algoly]" )
{
    const char code[] =
        "var b := 2\n"
        "var c := 3\n"
        "var d := [10, 20, 30]\n"
        "def a() 9; b + c + d[1] end\n"
        ;

    TestCompileAndRunAlgoly( code, 25 );
}

TEST_CASE( "Local array refs in binary exprs, init array with mixed calls and number", "[algoly]" )
{
    const char code[] =
        "def B( x ) x + 1 end\n"
        "def a()\n"
        //"  var d: [3] := [ 1, 2, 3 ]\n"
        "  var d: [3] := [ B(0), 2, B(2) ]\n"
        "  d[0] + d[1] + d[2]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 );
}

TEST_CASE( "Binary expr with 2 local ints", "[algoly]" )
{
    const char code[] =
        "def a var x := 3, y := 4; y - x end\n"
        ;

    TestCompileAndRunAlgoly( code, 1 );
}

TEST_CASE( "Case, when 1 key, 2 keys, else", "[algoly]" )
{
    const char code[] =
        "def a(x) case x when 1 then 10 when 2 then 20 when 5, 6 then 99 else 40 end end\n"
        ;

    SECTION( "First when" )
    {
        TestCompileAndRunAlgoly( code, 10, 1 );
    }

    SECTION( "Second when" )
    {
        TestCompileAndRunAlgoly( code, 20, 2 );
    }

    SECTION( "Third when, first key" )
    {
        TestCompileAndRunAlgoly( code, 99, 5 );
    }

    SECTION( "Third when, second key" )
    {
        TestCompileAndRunAlgoly( code, 99, 6 );
    }

    SECTION( "Else" )
    {
        TestCompileAndRunAlgoly( code, 40, 7 );
    }
}

TEST_CASE( "For-to, inside increment arg", "[algoly]" )
{
    const char code[] =
        "def a B(1) end\n"
        "def B(x) for i := 1 to 3 do x := x + 1; next; x:=x+1 end x end\n"
        ;

    TestCompileAndRunAlgoly( code, 4 );
}

TEST_CASE( "While, inside inc arg", "[algoly]" )
{
    const char code[] =
        "def a B(0) end\n"
        "def B(x)"
        "  x := 1; while x < 2 do x := x + 1 end\n"
        "  x\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 2 );
}

TEST_CASE( "Loop, inside inc arg, break", "[algoly]" )
{
    const char code[] =
        "def a B(0) end\n"
        "def B(x)"
        "  x := 1; loop x := x + 1; break end\n"
        "  x\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 2 );
}

TEST_CASE( "Loop-while, inside inc arg, break", "[algoly]" )
{
    const char code[] =
        "def a B(0) end\n"
        "def B(x)"
        "  x := 1; loop x := x + 1 do while x < 2 end\n"
        "  x\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 2 );
}

TEST_CASE( "Loop, inside inc local, break", "[algoly]" )
{
    const char code[] =
        "def a() var x := 1; loop x := x + 1; if x >= 2 then break end end x end\n"
        ;

    TestCompileAndRunAlgoly( code, 2 );
}

TEST_CASE( "For, if-then, break", "[algoly]" )
{
    const char code[] =
        "def a() for i := 1 to 3 do if i >= 2 then break end end end\n"
        ;

    TestCompileAndRunAlgoly( code, 0 );
}

TEST_CASE( "For, if-elsif, break", "[algoly]" )
{
    const char code[] =
        "def a() for i := 1 to 3 do if i >= 3 then elsif i >= 2 then break end end end\n"
        ;

    TestCompileAndRunAlgoly( code, 0 );
}

TEST_CASE( "For, if-else, break", "[algoly]" )
{
    const char code[] =
        "def a() for i := 1 to 3 do if i < 2 then elsif i >= 3 then else break end end end\n"
        ;

    TestCompileAndRunAlgoly( code, 0 );
}

TEST_CASE( "For, case-when, break", "[algoly]" )
{
    const char code[] =
        "def a() for i := 1 to 3 do case 1 when 1 then break end end end\n"
        ;

    TestCompileAndRunAlgoly( code, 0 );
}

TEST_CASE( "For, case-else, break", "[algoly]" )
{
    const char code[] =
        "def a(x) for i := 1 to 3 do case x when 1 then break else break end end end\n"
        ;

    WHEN( "1" )
        TestCompileAndRunAlgoly( code, 0, 1 );

    WHEN( "Else" )
        TestCompileAndRunAlgoly( code, 0, 2 );
}

// ptr->func, int

#if 1

TEST_CASE( "Algoly: pfunc(int)->int", "[algoly]" )
{
    const char code[] =
        "def a()\n"
        "  var f: &proc(int\n"
        ") := &B\n"
        "  (f)(3)\n"
        "end\n"
        "def B(x) x * 11 end\n"
        ;

    TestCompileAndRunAlgoly( code, 33 );
}

TEST_CASE( "Algoly: pfunc(int, int)->int", "[algoly]" )
{
    const char code[] =
        "def a()\n"
        "  var f: &proc(int, int) := &B\n"
        "  f := &C\n"
        "  (f)(3, 5)\n"
        "end\n"
        "def B(x, y) x + y end\n"
        "def C(x, y) x * y end\n"
        ;

    TestCompileAndRunAlgoly( code, 15 );
}

TEST_CASE( "Algoly: pfunc(int, pfunc(int)->int)->int", "[algoly]" )
{
    const char code[] =
        "def a()\n"
        "  var f: &proc(int, &proc(int)) := &B\n"
        //"  f := &C\n"
        "  (f)(1, &C)\n"
        "end\n"
        "def B(x, y: &proc(int)) x + (y)(2) end\n"
        "def C(x) x * 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 7 );
}

TEST_CASE( "Algoly: pfunc(int, pfunc(int)->int)->int with lambda", "[algoly]" )
{
    const char code[] =
        "def a()\n"
        "  var f: &proc(int, &proc(int)) := &B\n"
        "  f := lambda (x, y: &proc(int)) x + (y)(4) end\n"
        //"  f := &C\n"
        "  (f)(1, &C)\n"
        "end\n"
        "def B(x, y: &proc(int)) x + (y)(2) end\n"
        "def C(x) x * 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 13 );
}

TEST_CASE( "Algoly: pfunc(int)->int replace", "[algoly]" )
{
    const char code[] =
        "def a()\n"
        "  var f: &proc := &B\n"
        "  f := &C\n"
        "  (f)()\n"
        "end\n"
        "def B 10 end\n"
        "def C 20 end\n"
        ;

    TestCompileAndRunAlgoly( code, 20 );
}

TEST_CASE( "Algoly: const local", "[algoly]" )
{
    const char code[] =
        "def a()\n"
        "  var i := 1\n"
        "  const J = 2\n"
        "  var k := 3\n"
        "  i + J + k\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 );
}

TEST_CASE( "Algoly: explicit int return type no params", "[algoly]" )
{
    const char code[] =
        "def a()\n"
        "  var f: &proc->int := &B\n"
        "  (f)()\n"
        "end\n"
        "def B -> int 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: explicit int return type params", "[algoly]" )
{
    const char code[] =
        "def a()\n"
        "  var f: &proc(int)->int := &B\n"
        "  (f)(2)\n"
        "end\n"
        "def B(x) -> int x + 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 5 );
}

TEST_CASE( "Algoly: explicit fptr return type no params", "[algoly]" )
{
    const char code[] =
        "def a()\n"
        "  var f: &proc->&proc := &B\n"
        "  ((f)())()\n"
        "end\n"
        "def B -> &proc &C end\n"
        "def C 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: explicit fptr return type params", "[algoly]" )
{
    const char code[] =
        "def a()\n"
        "  var f: &proc(&proc)->&proc := &B\n"
        "  ((f)(&C))()\n"
        "end\n"
        "def B(f: &proc) -> &proc f end\n"
        "def C 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: explicit fptr chain of 2 return types", "[algoly]" )
{
    const char code[] =
        "def a()\n"
        "  var f: &proc->&proc->int := &B\n"
        "  ((f)())()\n"
        "end\n"
        "def B -> &proc &C end\n"
        "def C 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: global array of fptr", "[algoly]" )
{
    const char code[] =
        "var arr: [2] of &proc := [&B, &C]\n"
        "def a\n"
        "  (arr[0])() + (arr[1])() end\n"
        "def B 2 end\n"
        "def C 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 5 );
}

TEST_CASE( "Algoly: local array of fptr", "[algoly]" )
{
    const char code[] =
        "def a\n"
        "  var arr: [2] of &proc := [&B, &C]\n"
        "  (arr[0])() + (arr[1])() end\n"
        "def B 2 end\n"
        "def C 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 5 );
}

TEST_CASE( "Algoly: fptr type alias", "[algoly]" )
{
    const char code[] =
        "type P = &proc(int)->int, I=int\n"
        "const i: I = 3\n"
        "def a var p: P := &B; (p)(i) end\n"
        "def B( x ) x end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: Named int type", "[algoly]" )
{
    const char code[] =
        "def a() var t: int := 2; t := t+1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

#endif

TEST_CASE( "Algoly: New line after param and arg", "[algoly]" )
{
    const char code[] =
        "def a; B(33\n"
        ") end\n"
        "def B(x\n"
        ") x end\n"
        ;

    TestCompileAndRunAlgoly( code, 33 );
}

TEST_CASE( "Binary expr of lambda->call and addrOf->call", "[algoly]" )
{
    const char code[] =
        "def a; (lambda (x) x end)(66) + (&B)(33) end\n"
        "def B(x) x end\n"
        ;

    TestCompileAndRunAlgoly( code, 99 );
}

TEST_CASE( "If-Else, complex-condition with literals", "[algoly]" )
{
    WHEN( "true" )
    {
        const char code[] =
            "def a\n"
            "  if (1+1) < (2+2) and (4+0) < (3+2) then 3\n"
            "  else 4 end\n"
            "end\n"
            ;

        TestCompileAndRunAlgoly( code, 3 );
    }

    WHEN( "false" )
    {
        const char code[] =
            "def a\n"
            "  if (1+1) < (2+2) and (4+0) < (3+1) then 3\n"
            "  else 4 end\n"
            "end\n"
            ;

        TestCompileAndRunAlgoly( code, 4 );
    }
}

TEST_CASE( "Complex arithmetic", "[algoly]" )
{
    const char code[] =
        "def a; (1 + (32/4*2) - 2) end\n"
        ;

    TestCompileAndRunAlgoly( code, 15 );
}

TEST_CASE( "Call without parens, 1 arg", "[algoly]" )
{
    const char code[] =
        "def a B 93 end\n def B(x) -x end\n"
        ;

    TestCompileAndRunAlgoly( code, -93 );
}

TEST_CASE( "Call with parens, 1 arg", "[algoly]" )
{
    const char code[] =
        "def a B(97) end\n def B(x) -x end\n"
        ;

    TestCompileAndRunAlgoly( code, -97 );
}

TEST_CASE( "If", "[algoly]" )
{
    const char code[] =
        "def a(x) if not x then 20 end end\n"
        ;

    WHEN( "true" )
    {
        TestCompileAndRunAlgoly( code, 0, 1 );
    }

    WHEN( "false" )
    {
        TestCompileAndRunAlgoly( code, 20, 0 );
    }
}

TEST_CASE( "Return, not last expr", "[algoly]" )
{
    const char code[] =
        "def a 9; 3; return 99; not false; end\n"
        ;

    TestCompileAndRunAlgoly( code, 99 );
}

TEST_CASE( "Return, last expr", "[algoly]" )
{
    const char code[] =
        "def a 9; 3; not false; return 99; end\n"
        ;

    TestCompileAndRunAlgoly( code, 99 );
}

//-------------------------------

TEST_CASE( "Call, line breaks, 2 args", "[algoly][syntax]" )
{
    const char code[] =
        "var t := 0\n"
        "def a B( \n"
        "  1\n"
        "  ,\n"
        "  2\n"
        "  )\n"
        "  B 4,\n"
        "    5\n"
        "  B 2, 6\n"
        "  t + 3\n"
        "end\n"
        "def B(x, y) t := t + x + y end\n"
        ;

    TestCompileAndRunAlgoly( code, 23 );
}

TEST_CASE( "Call, line breaks, 1 arg", "[algoly][syntax]" )
{
    const char code[] =
        "var t := 0\n"
        "def a B( \n"
        "  1\n"
        "  )\n"
        "  B 4\n"
        "  t + 3\n"
        "end\n"
        "def B(x) t := t + x end\n"
        ;

    TestCompileAndRunAlgoly( code, 8 );
}

TEST_CASE( "Call, line breaks, 0 args", "[algoly][syntax]" )
{
    const char code[] =
        "var t := 0\n"
        "def a B( \n"
        "  )\n"
        "  B\n"
        "  t + 3\n"
        "end\n"
        "def B() t := t + 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 9 );
}

TEST_CASE( "Negative number", "[algoly][syntax]" )
{
    const char code[] =
        "def a -3 end\n"
        ;

    TestCompileAndRunAlgoly( code, -3 );
}

TEST_CASE( "Const, multi, dependent", "[algoly]" )
{
    const char code[] =
        "const C1 = 1\n"
        "const C2 = 2,\n"
        "      C3 = 4\n"
        "const C4 = C1 + C2 + C3\n"
        "def a C4 end\n"
        ;

    TestCompileAndRunAlgoly( code, 7 );
}

TEST_CASE( "Case-when-else, complex test", "[algoly]" )
{
    const char code[] =
        "def a(x)\n"
        "  case 1+x when 3, 4 then 5*2 else 5*3 end\n"
        "end\n"
        ;

    WHEN( "Passed when" )
    {
        TestCompileAndRunAlgoly( code, 10, 2 );
    }

    WHEN( "Else" )
    {
        TestCompileAndRunAlgoly( code, 15, 10 );
    }
}

TEST_CASE( "Case-when, no else", "[algoly]" )
{
    const char code[] =
        "def a(x)\n"
        "  case 11+2 when 3, 4 then 5*2 end\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 0 );
}

TEST_CASE( "Case-when, complex const key", "[algoly]" )
{
    const char code[] =
        "def a(x)\n"
        "  case x when 1+1+1, 2+2 then 5*2 end\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10, 4 );
}

TEST_CASE( "Case-when, complex var key", "[algoly]" )
{
    const char code[] =
        "var m := 3\n"
        "def a(x)\n"
        "  case x when m, m+1 then 5*2 end\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10, 4 );
}

TEST_CASE( "Algoly: Case-when, mod dotted key", "[algoly]" )
{
    const char* modeCodeA[] =
    {
        "const N = 2\n"
        ,
        nullptr
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "def a\n"
        "  case 2 when ModA.N then 3 end\n"
        "end\n"
        ,
        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "ModA",   modeCodeA },
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 3, 0 );
}

TEST_CASE( "For-to, last expr", "[algoly]" )
{
    const char code[] =
        "def a(x)\n"
        "  for i := 1 to 3 do 9 end\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 0 );
}

TEST_CASE( "Loop, while, break", "[algoly]" )
{
    const char code[] =
        "def a var x := 0\n"
        "  loop x:=x+1; break do while 13+13 end\n"
        "  while 2+2 do x:=x+1; break end\n"
        "  x\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 2 );
}

TEST_CASE( "Loop, while, complex exprs", "[algoly]" )
{
    const char code[] =
        "def a var x := 0\n"
        "  loop x := x + (2+3) do while x < 5*2 end\n"
        "  while x < 20*5 do x := x + (20- 7) end\n"
        "  x\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 101 );
}

TEST_CASE( "Algoly: Subtraction, no spaces", "[algoly]" )
{
    const char code[] =
        "def a 5-8 end"
        ;

    TestCompileAndRunAlgoly( code, -3 );
}

TEST_CASE( "Algoly: For-to-step, complex bounds and step, call lambda with loop var", "[algoly]" )
{
    // 1, 4, 7, 10, 13 = 35
    const char code[] =
        "def a\n"
        "  var x:=0, f := lambda (x) x*(1+2+3+4) end\n"
        "  for i := 4-3 to 16-3 by 1+2 do\n"
        "    x := x + (f)(i)\n"
        "  end\n"
        "  x\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 350 );
}

TEST_CASE( "Algoly: Let, call, complex exprs", "[algoly]" )
{
    const char code[] =
        "def a\n"
        "  var x:=9*2;  x := x-B(-(33*3));\n"
        "  x := x+(1+2*3-4)\n"
        "end\n"
        "def B(x) x end\n"
        ;

    TestCompileAndRunAlgoly( code, 120 );
}

TEST_CASE( "Algoly: Global array, complex elem with fptr elem and repeat init", "[algoly]" )
{
    const char code[] =
        "var ar: [2] of [2] of &proc := [[&B...]...]\n"
        "def a \n"
        "  var total := 0\n"
        "  for i := 0 to 1 do\n"
        "    for j := 0 to 1 do\n"
        "      total := total + (ar[i][j])()\n"
        "    end\n"
        "  end\n"
        //"  total := (ar[0])() + (ar[1])() + (ar[2])()\n"
        "  total\n"
        "end\n"
        "def B 3 end\n"
        "def C 4 end\n"
        ;

    TestCompileAndRunAlgoly( code, 12 );
}

TEST_CASE( "Algoly: Local array, complex elem with fptr elem and repeat init", "[algoly]" )
{
    const char code[] =
        "def a \n"
        "  var ar: [2] of [2] of &proc := [[&B...]...]\n"
        "  var total := 0\n"
        "  for i := 0 to 1 do\n"
        "    for j := 0 to 1 do\n"
        "      total := total + (ar[i][j])()\n"
        "    end\n"
        "  end\n"
        //"  total := (ar[0])() + (ar[1])() + (ar[2])()\n"
        "  total\n"
        "end\n"
        "def B 3 end\n"
        "def C 4 end\n"
        ;

    TestCompileAndRunAlgoly( code, 12 );
}

TEST_CASE( "Algoly: Global array, fptr elem and repeat init", "[algoly]" )
{
    const char code[] =
        "var ar: [3] of &proc := [&B, &C...]\n"
        "def a \n"
        "  var total := 0\n"
        "  for i := 0 to 2 do total := total + (ar[i])() end\n"
        //"  total := (ar[0])() + (ar[1])() + (ar[2])()\n"
        "  total\n"
        "end\n"
        "def B 3 end\n"
        "def C 4 end\n"
        ;

    TestCompileAndRunAlgoly( code, 11 );
}

TEST_CASE( "Algoly: Local array, fptr elem and repeat init", "[algoly]" )
{
    const char code[] =
        "def a \n"
        "  var ar: [3] of &proc := [&B, &C...]\n"
        "  var total := 0\n"
        "  for i := 0 to 2 do total := total + (ar[i])() end\n"
        //"  total := (ar[0])() + (ar[1])() + (ar[2])()\n"
        "  total\n"
        "end\n"
        "def B 3 end\n"
        "def C 4 end\n"
        ;

    TestCompileAndRunAlgoly( code, 11 );
}

TEST_CASE( "Algoly: Local array repeat init with call and binary", "[algoly]" )
{
    const char code[] =
        "var m := 2, n := 3, p := 4\n"
        "def a\n"
        "  var ar: [2] of [2] := [[B(p), m*n]...]\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        "def B(x) x*x end\n"
        ;

    TestCompileAndRunAlgoly( code, 44, 0, 11 );
}

TEST_CASE( "Algoly: Global array, int complex elem and repeat init, complex indexing", "[algoly]" )
{
    const char code[] =
        "var bb := 2\n"
        "var ar: [3] := [1+2, 2+3...]\n"
        "def a \n"
        "  ar[3-3]+ar[bb/2]+ar[6/3] end\n"
        ;

    TestCompileAndRunAlgoly( code, 13 );
}

TEST_CASE( "Algoly: Global array, int complex elem and extra init, complex indexing", "[algoly]" )
{
    const char code[] =
        "var bb := 2\n"
        "var ar: [3] := [1+2, 2+3...+]\n"
        "def a \n"
        "  ar[3-3]+ar[bb/2]+ar[6/3] end\n"
        ;

    TestCompileAndRunAlgoly( code, 15 );
}

TEST_CASE( "Algoly: Local array, int complex elem and repeat init, complex indexing", "[algoly]" )
{
    const char code[] =
        "def a \n"
        "  var bb := 2\n"
        "  var ar: [3] := [1+2, 2+3...]\n"
        "  ar[3-3]+ar[bb/2]+ar[6/3] end\n"
        ;

    TestCompileAndRunAlgoly( code, 13 );
}

TEST_CASE( "Algoly: Local array, int complex elem and extra init, complex indexing", "[algoly]" )
{
    const char code[] =
        "def a \n"
        "  var bb := 2\n"
        "  var ar: [3] := [1+2, 2+3...+]\n"
        "  ar[3-3]+ar[bb/2]+ar[6/3] end\n"
        ;

    TestCompileAndRunAlgoly( code, 15 );
}

TEST_CASE( "Algoly: Assignment, chained", "[algoly]" )
{
    const char code[] =
        "def a\n"
        "  var x:=0, y:=0, z:=1\n"
        "  x := y := z := B(3);\n"
        "  x*y*z\n"
        "end\n"
        "def B(x) x end\n"
        ;

    TestCompileAndRunAlgoly( code, 27 );
}

TEST_CASE( "Algoly: Assignment, return chained", "[algoly]" )
{
    const char code[] =
        "def a\n"
        "  var x:=0, y:=0, z:=1\n"
        "  x := y := z := B(3);\n"
        "end\n"
        "def B(x) x end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: Local array, empty init, chained assignment", "[algoly]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [3] := []\n"
        "  ar[0] := ar[1] := ar[2] := B(3+ar[0]+ar[1]+ar[2])\n"
        "  ar[0]*ar[1]*ar[2] end\n"
        "def B(x) x end\n"
        ;

    TestCompileAndRunAlgoly( code, 27 );
}

TEST_CASE( "Algoly: Lambda, call directly, local binding", "[algoly]" )
{
    const char code[] =
        "def a(x)\n"
        "  (lambda (a) a+5 end)(x)\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 7, 2 );
}

TEST_CASE( "Algoly: Lambda, local binding", "[algoly]" )
{
    const char code[] =
        "def a\n"
        "  var f := &B, g := 0; g := (f)(1, 3)\n"
        "  f := lambda (a, b) a*b end\n"
        "  (f)(2, 3) - g\n"
        "end\n"
        "def B(x, y) x+y end\n"
        ;

    TestCompileAndRunAlgoly( code, 2 );
}

TEST_CASE( "Globals, mixed", "[algoly]" )
{
    const char code[] =
        "var b := 2, c := 3\n"
        "var d: [3] := [10, 20, 30],\n"
        "  e := 2\n"
        "def a() b + c + d[1] + e end\n"
        ;

    TestCompileAndRunAlgoly( code, 27 );
}

TEST_CASE( "Global array, indexing", "[algoly]" )
{
    const char code[] =
        "var array: [3] := [0, 2, 3]\n"
        "def a if array[0] or (array[1] and array[2]) then 99 else 101 end end\n"
        ;

    TestCompileAndRunAlgoly( code, 99 );
}


//----------------------------------------------------------------------------
//  (new)
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: local const", "[algoly]" )
{
    const char code[] =
        "def a const C = 3, D = 2; C + D end\n"
        ;

    TestCompileAndRunAlgoly( code, 5 );
}

TEST_CASE( "Algoly: const with type denoter", "[algoly]" )
{
    const char code[] =
        "const C: int = 3, D: int = 2\n"
        "def a C + D end\n"
        ;

    TestCompileAndRunAlgoly( code, 5 );
}

TEST_CASE( "Algoly: func return with type denoter int", "[algoly]" )
{
    const char code[] =
        "var f: &proc -> int := &B\n"
        "def a\n"
        "  var t := (f)() + B()\n"
        "  f := &C\n"
        "  t := t + (f)() + C()\n"
        "end\n"
        "def B -> int 3 end\n"
        "def C -> int 2 end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}

TEST_CASE( "Algoly: func return with type denoter proc->int", "[algoly]" )
{
    const char code[] =
        "def a\n"
        "  var f: &proc(int) -> &proc := &X\n"
        "  var t := ((f)(0))() + (X(0))()\n"
        "  t := t + ((f)(1))() + (X(1))()\n"
        "end\n"
        "def B -> int 3 end\n"
        "def C -> int 2 end\n"
        "def X(x)->&proc if x then &C else &B end end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}

TEST_CASE( "Algoly: func addrs in global", "[algoly]" )
{
    const char code[] =
        "var f := &B\n"
        "def a\n"
        "  var t := (f)()\n"
        "  f := &C\n"
        "  t + (f)()\n"
        "end\n"
        "def B -> int 3 end\n"
        "def C -> int 2 end\n"
        ;

    TestCompileAndRunAlgoly( code, 5 );
}

TEST_CASE( "Algoly: local fptr array", "[algoly]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [2] of &proc->int := [&B, &C]\n"
        "  var t := (ar[0])() + (ar[1])()\n"
        "  for i := 0 to 1 do\n"
        "    t := t + (ar[i])()\n"
        "  end\n"
        "  ar[1] := &D\n"
        "  t + (ar[1])()\n"
        "end\n"
        "def B -> int 3 end\n"
        "def C -> int 2 end\n"
        "def D -> int 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 11 );
}

TEST_CASE( "Algoly: global fptr array", "[algoly]" )
{
    const char code[] =
        "var ar: [2] of &proc->int := [&B, &C]\n"
        "def a\n"
        "  var t := (ar[0])() + (ar[1])()\n"
        "  for i := 0 to 1 do\n"
        "    t := t + (ar[i])()\n"
        "  end\n"
        "  ar[1] := &D\n"
        "  t + (ar[1])()\n"
        "end\n"
        "def B -> int 3 end\n"
        "def C -> int 2 end\n"
        "def D -> int 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 11 );
}

TEST_CASE( "Algoly: type alias int", "[algoly]" )
{
    const char code[] =
        "type T = int\n"
        "type U = T\n"
        "def a\n"
        "  var t: T := 1, u: U := 2\n"
        "  t + u\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: type alias fptr", "[algoly]" )
{
    const char code[] =
        "type F = &proc(int)->int\n"
        "var f1: F := &B\n"
        "def a\n"
        "  var f2: F := &C\n"
        "  (f1)(10) + (f2)(10)\n"
        "end\n"
        "def B(x) x + 1 end\n"
        "def C(x) x + 2 end\n"
        ;

    TestCompileAndRunAlgoly( code, 23 );
}

TEST_CASE( "Algoly: type alias array", "[algoly]" )
{
    const char code[] =
        "type AI = [2] of int\n"
        "type AF = [2] of &proc->int\n"
        "var af: AF := [&B, &C]\n"
        "def a\n"
        "  var ai: AI := [1, 2]\n"
        "  ai[0] + ai[1] + (af[0])() + (af[1])()\n"
        "end\n"
        "def B 3 end\n"
        "def C 4 end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}

TEST_CASE( "Algoly: multi type alias int", "[algoly]" )
{
    const char code[] =
        "type T = int, F = &proc\n"
        "def a\n"
        "  var t: T := 1, f: F := &B\n"
        "  t + (f)()\n"
        "end\n"
        "def B 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 4 );
}

TEST_CASE( "Algoly: elide branch before discarded catch all", "[algoly]" )
{
    // See the elision of B instruction at end of Cond generation

    const char code[] =
        "def a\n"
        "  var n := 1\n"
        "  if n then B() else 3 end\n"
        "  0\n"
        "end\n"
        "def B end\n"
        ;

    TestCompileAndRunAlgoly( code, 0 );
}


//----------------------------------------------------------------------------
// Arrays again
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: countof", "[algoly]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [3] := [1, 2, 3]\n"
        "  countof( ar )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: slice index literal", "[algoly]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [3] := [1, 2, 3]\n"
        "  ar[1..3][1] := 30\n"
        "  ar[2]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30 );
}

TEST_CASE( "Algoly: slice index complex expr", "[algoly]" )
{
    const char code[] =
        "def a\n"
        "  const I = 1\n"
        "  var ar: [3] := [1, 2, 3]\n"
        "  ar[I+0..3][1] := 30\n"
        "  ar[2]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30 );
}

TEST_CASE( "Algoly: default array", "[algoly]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [3] := []\n"
        "  ar[0] + ar[1] + ar[2]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 0 );
}

TEST_CASE( "Algoly: default array elements", "[algoly]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [3] := [5]\n"
        "  ar[0] + ar[1]*10 + ar[2]*100\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 5 );
}

TEST_CASE( "Algoly: assign countof to const", "[algoly]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [3] := [5]\n"
        "  const N = countof( ar1 )\n"
        "  var ar2: [N] := []\n"
        "  countof( ar2 )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 3 );
}

TEST_CASE( "Algoly: big array 32768 extrapolate", "[algoly]" )
{
    const char code[] =
        "var ar: [32768] := [1, 2...+]\n"
        "def a\n"
        "  var total := 0\n"
        "  for i := 0 to countof(ar)-1 do\n"
        "    total := ar[i] + total\n"
        "  end"
        "  total\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 536887296 );
}

TEST_CASE( "Algoly: big array 65534 extrapolate", "[algoly]" )
{
    const char code[] =
        "var ar: [65534] := [1, 2...+]\n"
        "def a\n"
        "  var total := 0\n"
        "  for i := 0 to countof(ar)-1 do\n"
        "    total := ar[i] + total\n"
        "  end"
        "  total\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 2147385345 );
}

TEST_CASE( "Algoly: big array 63x1024 extrapolate repeat", "[algoly]" )
{
    const char code[] =
        "var ar: [63] of [1024] := [[1, 2...+]...]\n"
        "def a\n"
        "  var total := 0\n"
        "  for i := 0 to countof(ar)-1 do\n"
        "    for j := 0 to countof(ar[0])-1 do\n"
        "      total := ar[i][j] + total\n"
        "    end\n"
        "  end"
        "  total\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 33062400 );
}

#define EXTRAPOLATE_MATCHES_FUNC_3 \
    "def Matches(var ar: [3])\n" \
    "  ar[2] = ar[1] + (ar[1] - ar[0])\n" \
    "end\n"

#define EXTRAPOLATE_MATCHES_FUNC_4 \
    "def Matches(var ar: [4])\n" \
    "  ar[3] = ar[1] + (ar[1] - ar[0])*2\n" \
    "end\n"

#define EXTRAPOLATE_MATCHES_FUNC_5 \
    "def Matches(var ar: [5])\n" \
    "  ar[4] = ar[1] + (ar[1] - ar[0])*3\n" \
    "end\n"

TEST_CASE( "Algoly: array extrapolate limit positive 1", "[algoly]" )
{
    // 2,147,483,648
    const char code[] =
        "var gar: [3] := [2, 1073741825...+]\n"
        "def a\n"
        "  var lar: [3] := [2, 1073741825...+]\n"
        "  Matches(gar) and Matches(lar) and (gar[2] = lar[2])\n"
        "end\n"
        EXTRAPOLATE_MATCHES_FUNC_3
        ;

    TestCompileAndRunAlgoly( code, 1 );
}

TEST_CASE( "Algoly: array extrapolate limit positive 2", "[algoly]" )
{
    const char code[] =
        "var gar: [4] := [2, 1073741825...+]\n"
        "def a\n"
        "  var lar: [4] := [2, 1073741825...+]\n"
        "  Matches(gar) and Matches(lar) and (gar[3] = lar[3])\n"
        "end\n"
        EXTRAPOLATE_MATCHES_FUNC_4
        ;

    TestCompileAndRunAlgoly( code, 1 );
}

TEST_CASE( "Algoly: array extrapolate limit positive 3", "[algoly]" )
{
    // 4,294,967,296
    const char code[] =
        "var gar: [5] := [0, 1073741824...+]\n"
        "def a\n"
        "  var lar: [5] := [0, 1073741824...+]\n"
        "  Matches(gar) and Matches(lar) and (gar[4] = lar[4])\n"
        "end\n"
        EXTRAPOLATE_MATCHES_FUNC_5
        ;

    TestCompileAndRunAlgoly( code, 1 );
}

TEST_CASE( "Algoly: array extrapolate limit negative in range", "[algoly]" )
{
    // -2,147,483,648
    const char code[] =
        "var gar: [3] := [-2, -1073741825...+]\n"
        "def a\n"
        "  var lar: [3] := [-2, -1073741825...+]\n"
        "  Matches(gar) and Matches(lar) and (gar[2] = lar[2])\n"
        "end\n"
        EXTRAPOLATE_MATCHES_FUNC_3
        ;

    TestCompileAndRunAlgoly( code, 1 );
}

TEST_CASE( "Algoly: array extrapolate limit negative 1", "[algoly]" )
{
    // -2,147,483,649
    const char code[] =
        "var gar: [3] := [-1, -1073741825...+]\n"
        "def a\n"
        "  var lar: [3] := [-1, -1073741825...+]\n"
        "  Matches(gar) and Matches(lar) and (gar[2] = lar[2])\n"
        "end\n"
        EXTRAPOLATE_MATCHES_FUNC_3
        ;

    TestCompileAndRunAlgoly( code, 1 );
}

TEST_CASE( "Algoly: array extrapolate limit negative 2", "[algoly]" )
{
    const char code[] =
        "var gar: [4] := [-1, -1073741825...+]\n"
        "def a\n"
        "  var lar: [4] := [-1, -1073741825...+]\n"
        "  Matches(gar) and Matches(lar) and (gar[3] = lar[3])\n"
        "end\n"
        EXTRAPOLATE_MATCHES_FUNC_4
        ;

    TestCompileAndRunAlgoly( code, 1 );
}

TEST_CASE( "Algoly: array extrapolate limit negative 3", "[algoly]" )
{
    // -4,294,967,297
    const char code[] =
        "var gar: [5] := [-1, -1073741825...+]\n"
        "def a\n"
        "  var lar: [5] := [-1, -1073741825...+]\n"
        "  Matches(gar) and Matches(lar) and (gar[4] = lar[4])\n"
        "end\n"
        EXTRAPOLATE_MATCHES_FUNC_5
        ;

    TestCompileAndRunAlgoly( code, 1 );
}


//----------------------------------------------------------------------------
//  Natives
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: native basic", "[algoly]" )
{
    const char* mainCode[] =
    {
        "def a\n"
        "  Add(20, 30) + Mul(2, 3)\n"
        "end\n"
        "native Add(a, b)\n"
        "native Mul(a, b)\n"
        ,
        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 56, 0, 0, gNatives );
}

int NatNoReturnValue( Machine* machine, U8 argc, CELL* args, UserContext context )
{
    REQUIRE( (machine != nullptr && argc == 2 && args != nullptr) );

    return ERR_NONE;
}

static NativePair gNoRetNatives[] =
{
    { 0, NatNoReturnValue },
    { 0, nullptr }
};

TEST_CASE( "Algoly: native no return value", "[algoly]" )
{
    const char* mainCode[] =
    {
        "def a\n"
        "  NoReturnValue(20, 30)\n"
        "end\n"
        "native NoReturnValue(a, b)\n"
        ,
        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 0, 0, 0, gNoRetNatives );
}

static NativePair gHighNatives[] =
{
    { INT32_MAX-1, NatAdd },
    { INT32_MAX-0, NatMul },
    { 0, nullptr }
};

TEST_CASE( "Algoly: native high", "[algoly]" )
{
    const char* mainCode[] =
    {
        "def a\n"
        "  Add(20, 30) + Mul(2, 3)\n"
        "end\n"
        "native Add(a, b) = 2147483646\n"
        "native Mul(a, b)\n"
        ,
        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 56, 0, 0, gHighNatives );
}

static NativePair gLowHighNatives[] =
{
    { INT32_MAX - 0, NatAdd },
    { 1, NatMul },
    { 0, nullptr }
};

TEST_CASE( "Algoly: native low and high", "[algoly]" )
{
    const char* mainCode[] =
    {
        "def a\n"
        "  Add(20, 30) + Mul(2, 3)\n"
        "end\n"
        "native Add(a, b) = 2147483647\n"
        "native Mul(a, b) = 1\n"
        ,
        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 56, 0, 0, gLowHighNatives );
}

TEST_CASE( "Algoly: native reuse ID", "[algoly]" )
{
    const char* mainCode[] =
    {
        "def a\n"
        "  Add(20, 30) + Mul(2, 3) +\n"
        "  AddEx(40, 50) + MulEx(4, 5)\n"
        "end\n"
        "native Add(a, b)\n"
        "native Mul(a, b)\n"
        "native AddEx(a, b) = 0\n"
        "native MulEx(a, b) = 1\n"
        ,
        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 56+110, 0, 0, gNatives );
}

TEST_CASE( "Algoly: native refer other", "[algoly]" )
{
    const char* mainCode[] =
    {
        "def a\n"
        "  Add(20, 30) + Mul(2, 3) +\n"
        "  AddEx(40, 50) + MulEx(4, 5)\n"
        "end\n"
        "native Add(a, b)\n"
        "native Mul(a, b)\n"
        "native AddEx(a, b) = Add\n"
        "native MulEx(a, b) = Mul\n"
        ,
        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 56 + 110, 0, 0, gNatives );
}

TEST_CASE( "Algoly: native separate units, out of order", "[algoly]" )
{
    const char* mainCode[] =
    {
        "def a\n"
        "  Mul(2, 3) + Add(20, 30)\n"
        "end\n"
        ,
        "native Add(a, b)\n"
        "native Mul(a, b)\n"
        ,
        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 56, 0, 0, gNatives );
}

int NatYielderContinuation( Machine* machine, U8 argc, CELL* args, UserContext context )
{
    REQUIRE( (machine != nullptr && argc == 2 && args != nullptr) );

    machine->PushCell( args[0] + args[1] + static_cast<CELL>( context ) );
    return ERR_NONE;
}

int NatYielder( Machine* machine, U8 argc, CELL* args, UserContext context )
{
    REQUIRE( (machine != nullptr && argc == 2 && args != nullptr) );

    return machine->Yield( NatYielderContinuation, 100 );
}

static NativePair gYieldNatives[] =
{
    { 0, NatYielder },
    { 0, nullptr }
};

TEST_CASE( "Algoly: native yield", "[algoly]" )
{
    const char* mainCode[] =
    {
        "def a\n"
        "  Yielder(20, 30)\n"
        "end\n"
        "native Yielder(a, b)\n"
        ,
        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 150, 0, 0, gYieldNatives );
}


int NatCallA( Machine* machine, U8 argc, CELL* args, UserContext context )
{
    REQUIRE( argc == 2 );

    CELL* subArgs = machine->Start( args[0], 1 );

    subArgs[0] = args[1] + 4;

    int err = machine->Run();
    if ( err != ERR_NONE )
        return err;

    // Leave the return value from the managed call on the stack

    return ERR_NONE;
}

int NatCallB( Machine* machine, U8 argc, CELL* args, UserContext context )
{
    REQUIRE( argc == 2 );

    CELL* subArgs = machine->Start( args[0], 1 );

    subArgs[0] = args[1] + 5;

    int err = machine->Run();
    if ( err != ERR_NONE )
        return err;

    CELL retVal;

    err = machine->PopCell( retVal );
    if ( err != ERR_NONE )
        return err;

    err = machine->PushCell( retVal );
    if ( err != ERR_NONE )
        return err;

    return ERR_NONE;
}

static NativePair gNestedNatives[] =
{
    { 0, NatCallA },
    { 1, NatCallB },
    { 0, nullptr }
};

TEST_CASE( "Algoly: native call back nested", "[algoly]" )
{
    const char* mainCode[] =
    {
        "def a\n"
        "  CallA(&A, 1)\n"
        "end\n"
        "def A(x) CallB(&B, 10+x) end\n"
        "def B(x) 20+x end\n"
        "native CallA(callback: &proc(int), addend: int)\n"
        "native CallB(callback: &proc(int), addend: int)\n"
        ,
        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 40, 0, 0, gNestedNatives );
}


int NatCallFail( Machine* machine, U8 argc, CELL* args, UserContext context )
{
    return ERR_NATIVE_ERROR;
}

int NatCallYield( Machine* machine, U8 argc, CELL* args, UserContext context )
{
    REQUIRE( argc == 2 );

    return machine->Yield( NatCallFail, 0 );
}

static NativePair gNestedYieldNatives[] =
{
    { 0, NatCallA },
    { 1, NatCallYield },
    { 0, nullptr }
};

TEST_CASE( "Algoly: native call back nested yield", "[algoly][negative]" )
{
    const char* mainCode[] =
    {
        "def a\n"
        "  CallA(&A, 1)\n"
        "end\n"
        "def A(x) CallB(&B, 10+x) end\n"
        "def B(x) 20+x end\n"
        "native CallA(callback: &proc(int), addend: int)\n"
        "native CallB(callback: &proc(int), addend: int)\n"
        ,
        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, Emplace<ResultKind::Vm>( ERR_NATIVE_ERROR ), ParamSpan(), 0, gNestedYieldNatives );
}


//----------------------------------------------------------------------------
//  Modules, units
//----------------------------------------------------------------------------

// What happens when you init a global in 1 mod with a global in another mod?
//  Globals must be initialized with constant data

TEST_CASE( "Algoly: mod dotted slice index", "[algoly]" )
{
    const char* modeCodeA[] =
    {
        "const N = 1\n"
        ,

        nullptr
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "def a\n"
        "  var ar: [3] := [1, 2, 3]\n"
        "  ar[ModA.N..3][1] := 30\n"
        "  ar[2]\n"
        "end\n"
        ,

        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "ModA",   modeCodeA },
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 30, 0 );
}

TEST_CASE( "Algoly: mod dotted array count", "[algoly]" )
{
    const char* modeCodeA[] =
    {
        "const N = 3\n"
        ,

        nullptr
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "def a\n"
        "  var ar: [ModA.N] := [1, 2, 3]\n"
        "end\n"
        ,

        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "ModA",   modeCodeA },
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 0, 0 );
}

TEST_CASE( "Algoly: mod dotted type alias", "[algoly]" )
{
    const char* modeCodeA[] =
    {
        "type T = int\n"
        ,

        nullptr
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "def a var i: ModA.T := 3; i end\n"
        ,

        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "ModA",   modeCodeA },
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 3, 0 );
}

TEST_CASE( "Algoly: mod dotted callOrSymbol", "[algoly]" )
{
    const char* modeCodeA[] =
    {
        "var I := 3\n"
        "def X 4 end\n"
        ,

        nullptr
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "def a B() + C() end\n"
        "def B ModA.I end\n"
        "def C ModA.X end\n"
        ,

        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "ModA",   modeCodeA },
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 7, 0 );
}

TEST_CASE( "Algoly: mod dotted callOrSymbol, 1 arg", "[algoly]" )
{
    const char* modeCodeA[] =
    {
        "def X(n) n+2 end\n"
        ,
        nullptr
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "def a ModA.X 3 end\n"
        ,
        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "ModA",   modeCodeA },
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 5, 0 );
}

TEST_CASE( "Algoly: multi unit and mod", "[algoly]" )
{
    const char* modeCodeM2[] =
    {
        "var X := 1"
        ,

        "def B(x) X := x + 1 end\n"
        ,

        nullptr
    };

    const char* mainCode[] =
    {
        "import M2\n"
        "const CA1 = 3\n"
        ,

        "def a\n"
        "  var x := M2.X\n"
        "  M2.B(2)\n"
        "  x + M2.X + CC1\n"
        "end\n"
        ,

        "const CC1 = CA1 + 4\n"
        ,

        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "M2",     modeCodeM2 },
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 11, 0 );
}

TEST_CASE( "Algoly: multi unit", "[algoly]" )
{
    const char* mainCode[] =
    {
        "const CA1 = 3\n"
        //"def FC1 9 end\n"
        "def FA1(x) x + VB1 + FC1() end\n"
        ,

        //"def a CC1 + FC1() end\n"

        "def a\n"
        "  FA1(CC1) + C1\n"
        "end\n"
        "const C1 = 100 + C2\n"
        "const C2 = 200\n"
        "var VB1 := 5\n"
        ,

        "const CC1 = CA1 + 4\n"
        "def FC1 2 end\n"
        ,

        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 314, 0 );
}

TEST_CASE( "Algoly: multi unit, lambdas", "[algoly]" )
{
    const char* mainCode[] =
    {
        "def a\n"
        "  var f := lambda 1 end\n"
        "  var g := lambda 2 end\n"
        "  (f)() + (g)() + B()\n"
        "end\n"
        ,
        "def B\n"
        "  var h := lambda 3 end\n"
        "  var i := lambda 4 end\n"
        "  (h)() + (i)()\n"
        "end\n"
        ,
        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 10, 0 );
}

TEST_CASE( "Algoly: import as", "[algoly]" )
{
    const char* modCodeM2[] =
    {
        "const C1 = 99\n"
        "var V1 := 2\n"
        ,

        nullptr
    };

    // Output: 12
    const char* mainCode[] =
    {
        "import M2 as M1\n"
        "def a\n"
        "  M1.V1 := M1.V1 + 2\n"
        "  M1.C1 + M1.V1\n"
        "end\n"
        ,

        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "M2",     modCodeM2 },
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 103, 0 );
}

TEST_CASE( "Algoly: mod basic", "[algoly]" )
{
    const char* modCodeA[] =
    {
        "type P = &proc(int)\n"
        "const I = 3\n"
        "def C(x) x+2 end\n"
        ,

        nullptr
    };

    // Output: 12
    const char* mainCode[] =
    {
        "import ModA\n"
        "type Q = ModA.P\n"
        "const J = ModA.I\n"
        "def a\n"
        "  var p: ModA.P := &B, q: Q := &C, r := &ModA.C\n"
        "  (p)(3) + (q)(3) + (r)(3) +\n"
        "  ModA.C(3) + J + ModA.I\n"
        "end\n"
        "def B( x ) x end\n"
        "def C( x ) x+1 end\n"
        ,

        nullptr
    };

    const ModuleSource modSources[] =
    {
        {
            "ModA",
            modCodeA,
        },
        {
            "Main",
            mainCode,
        },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 17 + 6, 0 );
}

TEST_CASE( "Algoly: mod var proc exchange", "[algoly]" )
{
    const char* modCodeA[] =
    {
        "var d1 := 3, d2 := &C\n"
        "def C(x) x+2 end\n"
        "def D() d1 + (d2)(1) end\n"
        ,

        nullptr
    };

    // Output: 12
    const char* mainCode[] =
    {
        "import ModA\n"
        "def a\n"
        "  var e1 := ModA.d1, e2 := ModA.d2\n"
        "  var n :=\n"
        "    ModA.d1 + (ModA.d2)(1) +\n"
        "    e1 + (e2)(1)\n"
        "\n"
        "  ModA.d1 := 10\n"
        "  ModA.d2 := &C\n"
        "  n + ModA.D()\n"
        "end\n"
        "def C(x) x+20 end\n"
        ,

        nullptr
    };

    const ModuleSource modSources[] =
    {
        {
            "ModA",
            modCodeA,
        },
        {
            "Main",
            mainCode,
        },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 12 + 31, 0 );
}

TEST_CASE( "Algoly: mod array 1", "[algoly]" )
{
    const char* modCodeA[] =
    {
        "var ar1 := [[1, 2], [3, 4]]\n"
        "def B(i, j) ar1[i][j] end\n"
        ,

        nullptr
    };

    // Output: 12
    const char* mainCode[] =
    {
        "import ModA\n"
        "def a\n"
        "  ModA.ar1[0][0] := 10\n"
        "  ModA.ar1[1][1] := 20\n"
        "\n"
        "  ModA.ar1[0][1] + ModA.B(1, 0) +\n"
        "  ModA.B(0, 0) + ModA.ar1[1][1]\n"
        "end\n"
        ,

        nullptr
    };

    const ModuleSource modSources[] =
    {
        {
            "ModA",
            modCodeA,
        },
        {
            "Main",
            mainCode,
        },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 5 + 30, 0 );
}


//----------------------------------------------------------------------------
//  Yield
//----------------------------------------------------------------------------

// TODO: prove that it yielded the right number of times; maybe also the value at each point

TEST_CASE( "Algoly: yield discard", "[algoly]" )
{
    const char code[] =
        "var n := 0\n"
        "def a\n"
        "  B(1)\n"
        "  yield\n"
        "  B(2)\n"
        "  yield\n"
        "  B(3)\n"
        "  n\n"
        "end\n"
        "def B(x) n := n + x end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 );
}

TEST_CASE( "Algoly: yield end", "[algoly]" )
{
    const char code[] =
        "var n := 0\n"
        "def a\n"
        "  B(1)\n"
        "  yield\n"
        "  B(2)\n"
        "  yield\n"
        "end\n"
        "def B(x) n := n + x end\n"
        ;

    TestCompileAndRunAlgoly( code, 0 );
}


//------------------------

int NatCallFailManagedYield( Machine* machine, U8 argc, CELL* args, UserContext context )
{
    REQUIRE( argc == 1 );

    machine->Start( args[0], 0 );

    return machine->Run();
}

static NativePair gNestedManagedYieldNatives[] =
{
    { 0, NatCallFailManagedYield },
    { 0, nullptr }
};

TEST_CASE( "Algoly: fail nested managed yield", "[algoly][negative]" )
{
    const char* mainCode[] =
    {
        "def a\n"
        "  Call(&A)\n"
        "end\n"
        "def A() yield end\n"
        "native Call(callback: &proc)\n"
        ,
        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, Emplace<ResultKind::Vm>( ERR_BAD_STATE ), ParamSpan(), 0, gNestedManagedYieldNatives );
}


// TODO:
//TEST_CASE( "For ??? 1", "[algoly]" )
//{
//    const char code[] =
//        "def a var total := 0\n"
//        "  var array: [8] := [1, 2, 3, 4, 5, 6, 7, 8]\n"
//        "  for i := 0 below 8 do total := total + array[i] end\n"
//        "  for j := 0 to 3 do array[j] := 100 + j end\n"
//        "  total end\n"
//        ;
//
//    TestCompileAndRunAlgoly( code, 36 );
//}

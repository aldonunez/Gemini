#include "pch.h"
#include "TestBase.h"


//----------------------------------------------------------------------------
//  Stack usage
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: StackUse: default return", "[algoly][stack]" )
{
    const char code[] =
        "def a end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 0, 0, 3 );
}

TEST_CASE( "Algoly: StackUse: little number", "[algoly][stack]" )
{
    const char code[] =
        "def a 33 end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 33, 0, 3 );
}

TEST_CASE( "Algoly: StackUse: big number", "[algoly][stack]" )
{
    const char code[] =
        "def a 2147483647 end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 2147483647, 0, 3 );
}

TEST_CASE( "Algoly: StackUse: negate negative min number", "[algoly][stack]" )
{
    const char code[] =
        "def a - -2147483648 end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, INT32_MIN, 0, 3 );
}

TEST_CASE( "Algoly: StackUse: negative min number", "[algoly][stack]" )
{
    const char code[] =
        "def a -2147483648 end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, INT32_MIN, 0, 3 );
}

TEST_CASE( "Algoly: StackUse: unary not", "[algoly][stack]" )
{
    const char code[] =
        "var n := 0\n"
        "def a not n end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 1, 0, 3 );
}

TEST_CASE( "Algoly: StackUse: unary minus", "[algoly][stack]" )
{
    // Implemented with (0 - x)
    const char code[] =
        "var n := 1\n"
        "def a -n end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, -1, 0, 4 );
}

TEST_CASE( "Algoly: StackUse: binary multiply", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3\n"
        "def a m*n end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 6, 0, 4 );
}

TEST_CASE( "Algoly: StackUse: binary multiply with discard", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3\n"
        "def a m*n; m*n end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 6, 0, 4 );
}

TEST_CASE( "Algoly: StackUse: binary multiply with discard deeper", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3\n"
        "def a n-(m-(m-(m-n))); m*n end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 6, 0, 4 );
}

TEST_CASE( "? Algoly: StackUse: binary comparison", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3\n"
        "def a m < n end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 1, 0, 4 );
}

TEST_CASE( "Algoly: StackUse: atomized AND with comparisons", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3, p := 0\n"
        "def a\n"
        "  p := m < n and m <> n\n"
        "  p\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 1, 0, 4 );
}

TEST_CASE( "Algoly: StackUse: atomized OR with comparisons", "[algoly][stack]" )
{
    const char code[] =
        "var m := 3, n := 2, p := 0\n"
        "def a\n"
        "  p := m < n or m <> n\n"
        "  p\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 1, 0, 4 );
}

TEST_CASE( "Algoly: StackUse: NOT atomized AND with comparisons", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3, p := 0\n"
        "def a\n"
        "  p := not (m < n and m <> n)\n"
        "  p\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 0, 0, 4 );
}

TEST_CASE( "Algoly: StackUse: NOT atomized OR with comparisons", "[algoly][stack]" )
{
    const char code[] =
        "var m := 3, n := 2, p := 0\n"
        "def a\n"
        "  p := not (m < n or m <> n)\n"
        "  p\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 0, 0, 4 );
}

TEST_CASE( "Algoly: StackUse: return global const", "[algoly][stack]" )
{
    const char code[] =
        "const N = 5\n"
        "def a N end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 5, 0, 3 );
}

TEST_CASE( "Algoly: StackUse: return local const", "[algoly][stack]" )
{
    const char code[] =
        "def a const N = 5; N end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 5, 0, 3 );
}

TEST_CASE( "Algoly: StackUse: return local", "[algoly][stack]" )
{
    const char code[] =
        "def a\n"
        "  var n := 5\n"
        "  n\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 5, 0, 4 );
}
TEST_CASE( "Algoly: StackUse: return local, 2 locals", "[algoly][stack]" )
{
    const char code[] =
        "def a\n"
        "  var m := 5\n"
        "  var n := m\n"
        "  n\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 5, 0, 5 );
}

TEST_CASE( "Algoly: StackUse: call return local", "[algoly][stack]" )
{
    const char code[] =
        "def a\n"
        "  B()\n"
        "end\n"
        "def B\n"
        "  var n := 5\n"
        "  n\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 5, 0, 6 );
}

TEST_CASE( "Algoly: StackUse: call, local, arg, binary", "[algoly][stack]" )
{
    const char code[] =
        "def a\n"
        "  B(1)\n"
        "end\n"
        "def B(x)\n"
        "  var n := 5\n"
        "  n + x\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 6, 0, 8 );
}

TEST_CASE( "Algoly: StackUse: call at right at end of deep tree going right", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3\n"
        "def a m+(m+(m+(m+B()))) end\n"
        "def B 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 9, 0, 9 );
}

TEST_CASE( "Algoly: StackUse: call at left in deep tree going right", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3\n"
        "def a m+(B()+(m+(m+m))) end\n"
        "def B C() end\n"
        "def C D() end\n"
        "def D E() end\n"
        "def E 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 9, 0, 12 );
}

TEST_CASE( "Algoly: StackUse: return binary, then tree going right", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3\n"
        "def a return m+m; m+(m+(m+(m+m))) end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 4, 0, 7 );
}

TEST_CASE( "Algoly: StackUse: add countof and var", "[algoly][stack]" )
{
    const char code[] =
        "var ar: [2] := []\n"
        "var m := 1\n"
        "def a m+countof(ar) end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 3, 0, 4 );
}

TEST_CASE( "Algoly: StackUse: add countof and const", "[algoly][stack]" )
{
    const char code[] =
        "var ar: [2] := []\n"
        "const M = 1\n"
        "def a M+countof(ar) end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 3, 0, 3 );
}

TEST_CASE( "Algoly: StackUse: if with comparison and no else", "[algoly][stack]" )
{
    const char code[] =
        "var n := 3\n"
        "def a(x) if x < n then 10 end end\n"
        ;

    WHEN( "2" )
        TestCompileAndRunAlgoly( code, sizeof code, 10, 2, 5 );

    WHEN( "3" )
        TestCompileAndRunAlgoly( code, sizeof code, 0, 3, 5 );
}

TEST_CASE( "Algoly: StackUse: if with comparison and else", "[algoly][stack]" )
{
    const char code[] =
        "var n := 3\n"
        "def a(x) if x < n then 10 else 20 end end\n"
        ;

    WHEN( "2" )
        TestCompileAndRunAlgoly( code, sizeof code, 10, 2, 5 );

    WHEN( "3" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, 3, 5 );
}

TEST_CASE( "Algoly: StackUse: if with comparison and no else, shallow call tree", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3\n"
        "def a if m < n then B() end end\n"
        "def B C() end\n"
        "def C 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 1, 0, 7 );
}

TEST_CASE( "Algoly: StackUse: if with comparison and else, shallow call tree", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3\n"
        "def a if m < n then B() else 20 end end\n"
        "def B C() end\n"
        "def C 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 1, 0, 7 );
}

TEST_CASE( "Algoly: StackUse: if with comparison and else, shallow call tree in else", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3\n"
        "def a if m < n then 10 else B() end end\n"
        "def B C() end\n"
        "def C 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 10, 0, 7 );
}

TEST_CASE( "Algoly: StackUse: assign global", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3\n"
        "def a m := 9; 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 1, 0, 3 );
}

TEST_CASE( "Algoly: StackUse: assign global return", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3\n"
        "def a m := 9 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 9, 0, 4 );
}

TEST_CASE( "Algoly: StackUse: assign local", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3\n"
        "def a var x := 0; x := 9; 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 1, 0, 4 );
}

TEST_CASE( "Algoly: StackUse: assign local return", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3\n"
        "def a var x := 0; x := 9 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 9, 0, 5 );
}

TEST_CASE( "Algoly: StackUse: assign param", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3\n"
        "def a(x) x := 9; 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 1, 0, 4 );
}

TEST_CASE( "Algoly: StackUse: assign param return", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3\n"
        "def a(x) x := 9 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 9, 0, 5 );
}

TEST_CASE( "Algoly: StackUse: have arg, assign local", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3\n"
        "def a(x) var y := 0; y := 9; 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 1, 0, 5 );
}

TEST_CASE( "Algoly: StackUse: have arg, assign local return", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3\n"
        "def a(x) var y := 0; y := 9 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 9, 0, 6 );
}

TEST_CASE( "Algoly: StackUse: 2-nested call, assign", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3\n"
        "def a m := B(); 1 end\n"
        "def B C() end\n"
        "def C 9 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 1, 0, 7 );
}

TEST_CASE( "Algoly: StackUse: 2-nested call, assign return", "[algoly][stack]" )
{
    const char code[] =
        "var m := 2, n := 3\n"
        "def a m := B() end\n"
        "def B C() end\n"
        "def C 9 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 9, 0, 7 );
}

TEST_CASE( "Algoly: StackUse: return lambda", "[algoly][stack]" )
{
    const char code[] =
        "def a B(); 1 end\n"
        "def B->&proc lambda 3 end end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 1, 0, 5 );
}

TEST_CASE( "Algoly: StackUse: assign lambda", "[algoly][stack]" )
{
    const char code[] =
        "var f := &C\n"
        "def a B(); 1 end\n"
        "def B f := lambda 3 end; 2 end\n"
        "def C 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 1, 0, 5 );
}

TEST_CASE( "Algoly: StackUse: assign return lambda", "[algoly][stack]" )
{
    const char code[] =
        "var f := &C\n"
        "def a B(); 1 end\n"
        "def B->&proc f := lambda 3 end end\n"
        "def C 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 1, 0, 6 );
}

TEST_CASE( "Algoly: StackUse: call lambda", "[algoly][stack]" )
{
    const char code[] =
        "def a() (lambda 3 end)() end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 3, 0, 3 );
}

TEST_CASE( "Algoly: StackUse: assign call lambda", "[algoly][stack]" )
{
    const char code[] =
        "var f := &B\n"
        "def a() f := lambda 3 end; (f)() end\n"
        "def B 8 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 3, 0, 3 );
}

//--------------

TEST_CASE( "Algoly: StackUse: addrof", "[algoly][stack]" )
{
    const char code[] =
        "def a B(); 1 end\n"
        "def B->&proc &C end\n"
        "def C 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 1, 0, 5 );
}

TEST_CASE( "Algoly: StackUse: assign addrof", "[algoly][stack]" )
{
    const char code[] =
        "var f := &D\n"
        "def a B(); 1 end\n"
        "def B f := &C; 2 end\n"
        "def C 3 end\n"
        "def D 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 1, 0, 5 );
}

TEST_CASE( "Algoly: StackUse: assign return addrof", "[algoly][stack]" )
{
    const char code[] =
        "var f := &D\n"
        "def a B(); 1 end\n"
        "def B->&proc f := &C end\n"
        "def C 3 end\n"
        "def D 0 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 1, 0, 6 );
}

TEST_CASE( "Algoly: StackUse: call addrof", "[algoly][stack]" )
{
    const char code[] =
        "def a() (&C)() end\n"
        "def C 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 3, 0, 3 );
}

TEST_CASE( "Algoly: StackUse: assign call addrof", "[algoly][stack]" )
{
    const char code[] =
        "var f := &B\n"
        "def a() f := &C; (f)() end\n"
        "def B 8 end\n"
        "def C 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 3, 0, 3 );
}

TEST_CASE( "Algoly: StackUse: global chain assign 3 fptr", "[algoly][stack]" )
{
    const char code[] =
        "var f := &B, g := &B, h := &B\n"
        "def a()\n"
        "  f := g := h := &C\n"
        "  (f)() + (g)() + (h)()\n"
        "end\n"
        "def B 8 end\n"
        "def C 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 9, 0, 4 );
}

TEST_CASE( "Algoly: StackUse: local chain assign 3 fptr", "[algoly][stack]" )
{
    const char code[] =
        "def a()\n"
        "  var f := &B, g := &B, h := &B\n"
        "  f := g := h := &C\n"
        "  (f)() + (g)() + (h)()\n"
        "end\n"
        "def B 8 end\n"
        "def C 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 9, 0, 4+3 );
}

TEST_CASE( "Algoly: StackUse: even call tree", "[algoly][stack]" )
{
    const char code[] =
        "def a B(\n"
        "  B( B( 1, 2 ), B( 3, 4 ) ),\n"
        "  B( B( 5, 6 ), B( 7, 8 ) )\n"
        ") end\n"
        "def B(x, y) x+y end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 36, 0, 10 );
}

TEST_CASE( "Algoly: StackUse: deep calls", "[algoly][stack]" )
{
    const char code[] =
        "def a\n"
        "  B( 1, 2 )\n"
        "end\n"
        "def B(x, y) var m := x+1, n := y+2; C(m, n) end\n"
        "def C(x, y) var m := x+2, n := y+3; D(m, n) end\n"
        "def D(x, y) var m := x+3, n := y+4; E(m, n) end\n"
        "def E(x, y) var m := x+4, n := y+5; F(m, n) end\n"
        "def F(x, y) var m := x+5, n := y+6; m+n end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 16+22, 0, 34 );
}

TEST_CASE( "Algoly: StackUse: native basic 2", "[algoly][stack]" )
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

    TestCompileAndRun( Language::Gema, modSources, 56, 0, 6, gNatives );
}

TEST_CASE( "Algoly: StackUse: for", "[algoly][stack]" )
{
    const char code[] =
        "var t := 0\n"
        "def a\n"
        "  for i := 1 to 3 do\n"
        "    t := i\n"
        "  end\n"
        "  t\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 3, 0, 5 );
}

TEST_CASE( "Algoly: StackUse: for by", "[algoly][stack]" )
{
    const char code[] =
        "var t := 0\n"
        "def a\n"
        "  for i := 1 to 3 by 2 do\n"
        "    t := i\n"
        "  end\n"
        "  t\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 3, 0, 5 );
}

TEST_CASE( "Algoly: StackUse: loop", "[algoly][stack]" )
{
    const char code[] =
        "def a loop break end end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 0, 0, 3 );
}

TEST_CASE( "Algoly: StackUse: loop while", "[algoly][stack]" )
{
    const char code[] =
        "var m := 0\n"
        "def a loop do while m end end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 0, 0, 3 );
}

TEST_CASE( "Algoly: StackUse: while", "[algoly][stack]" )
{
    const char code[] =
        "var m := 0\n"
        "def a while m do end end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 0, 0, 3 );
}

TEST_CASE( "Algoly: StackUse: if no else", "[algoly][stack]" )
{
    const char code[] =
        "def a(x) if x then 4 end end\n"
        ;

    WHEN( "0" )
        TestCompileAndRunAlgoly( code, sizeof code, 0, 0, 4 );

    WHEN( "1" )
        TestCompileAndRunAlgoly( code, sizeof code, 4, 1, 4 );
}

TEST_CASE( "Algoly: StackUse: if else", "[algoly][stack]" )
{
    const char code[] =
        "def a(x) if x then 4 else 5 end end\n"
        ;

    WHEN( "0" )
        TestCompileAndRunAlgoly( code, sizeof code, 5, 0, 4 );

    WHEN( "1" )
        TestCompileAndRunAlgoly( code, sizeof code, 4, 1, 4 );
}

TEST_CASE( "Algoly: StackUse: if many elsif", "[algoly][stack]" )
{
    const char code[] =
        "var m := 0\n"
        "def a(x)\n"
        "  m :=\n"
        "  if x = 2 then 20\n"
        "  elsif x = 3 then 30\n"
        "  elsif x = 4 then 40\n"
        "  elsif x = 5 then 50\n"
        "  elsif x = 6 then 60\n"
        "  elsif x = 7 then 70\n"
        "  elsif x = 8 then 80\n"
        "  elsif x = 9 then 90\n"
        "  else 100\n"
        "  end\n"
        "B()\n"
        "end\n"
        "def B C() end\n"
        "def C 20 end\n"
        ;

    WHEN( "2" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, 2, 8 );

    WHEN( "3" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, 3, 8 );

    WHEN( "4" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, 4, 8 );

    WHEN( "5" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, 5, 8 );

    WHEN( "6" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, 6, 8 );

    WHEN( "7" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, 7, 8 );

    WHEN( "8" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, 8, 8 );

    WHEN( "9" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, 9, 8 );

    WHEN( "10" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, 10, 8 );
}

TEST_CASE( "Algoly: StackUse: add two if no else", "[algoly][stack]" )
{
    const char code[] =
        "def a(x, y) (if x then 4 end) + (if y then 5 end) end\n"
        ;

    WHEN( "0, 0" )
        TestCompileAndRunAlgoly( code, sizeof code, 0, { 0, 0 }, 6 );

    WHEN( "0, 1" )
        TestCompileAndRunAlgoly( code, sizeof code, 5, { 0, 1 }, 6 );

    WHEN( "1, 0" )
        TestCompileAndRunAlgoly( code, sizeof code, 4, { 1, 0 }, 6 );

    WHEN( "1, 1" )
        TestCompileAndRunAlgoly( code, sizeof code, 9, { 1, 1 }, 6 );
}

TEST_CASE( "Algoly: StackUse: add two if else", "[algoly][stack]" )
{
    const char code[] =
        "def a(x, y) (if x then 4 else 10 end) + (if y then 5 else 20 end) end\n"
        ;

    WHEN( "0, 0" )
        TestCompileAndRunAlgoly( code, sizeof code, 30, { 0, 0 }, 6 );

    WHEN( "0, 1" )
        TestCompileAndRunAlgoly( code, sizeof code, 15, { 0, 1 }, 6 );

    WHEN( "1, 0" )
        TestCompileAndRunAlgoly( code, sizeof code, 24, { 1, 0 }, 6 );

    WHEN( "1, 1" )
        TestCompileAndRunAlgoly( code, sizeof code, 9, { 1, 1 }, 6 );
}

TEST_CASE( "Algoly: StackUse: case no else", "[algoly][stack]" )
{
    const char code[] =
        "def a(x) case x when 2 then 10 end end\n"
        ;

    WHEN( "2" )
        TestCompileAndRunAlgoly( code, sizeof code, 10, 2, 5 );

    WHEN( "3" )
        TestCompileAndRunAlgoly( code, sizeof code, 0, 3, 5 );
}

TEST_CASE( "Algoly: StackUse: case else", "[algoly][stack]" )
{
    const char code[] =
        "def a(x) case x when 2 then 10 else 20 end end\n"
        ;

    WHEN( "2" )
        TestCompileAndRunAlgoly( code, sizeof code, 10, 2, 5 );

    WHEN( "3" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, 3, 5 );
}

TEST_CASE( "Algoly: StackUse: case many clauses", "[algoly][stack]" )
{
    const char code[] =
        "var m := 0\n"
        "def a(x)\n"
        "  m :=\n"
        "  case x\n"
        "    when 2 then 20\n"
        "    when 3 then 30\n"
        "    when 4 then 40\n"
        "    when 5 then 50\n"
        "    when 6 then 60\n"
        "    when 7 then 70\n"
        "    when 8 then 80\n"
        "    when 9 then 90\n"
        "    else 100\n"
        "  end\n"
        "B()\n"
        "end\n"
        "def B C() end\n"
        "def C 20 end\n"
        ;

    WHEN( "2" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, 2, 8 );

    WHEN( "3" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, 3, 8 );

    WHEN( "4" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, 4, 8 );

    WHEN( "5" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, 5, 8 );

    WHEN( "6" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, 6, 8 );

    WHEN( "7" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, 7, 8 );

    WHEN( "8" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, 8, 8 );

    WHEN( "9" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, 9, 8 );

    WHEN( "10" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, 10, 8 );
}

TEST_CASE( "Algoly: StackUse: add two case no else", "[algoly][stack]" )
{
    const char code[] =
        "def a(x, y) (case x when 2 then 10 end) + (case y when 2 then 20 end) end\n"
        ;

    WHEN( "1, 1" )
        TestCompileAndRunAlgoly( code, sizeof code, 0, { 1, 1 }, 7 );

    WHEN( "1, 2" )
        TestCompileAndRunAlgoly( code, sizeof code, 20, { 1, 2 }, 7 );

    WHEN( "2, 1" )
        TestCompileAndRunAlgoly( code, sizeof code, 10, { 2, 1 }, 7 );

    WHEN( "2, 2" )
        TestCompileAndRunAlgoly( code, sizeof code, 30, { 2, 2 }, 7 );
}

TEST_CASE( "Algoly: StackUse: add two case else", "[algoly][stack]" )
{
    const char code[] =
        "def a(x, y) (case x when 2 then 10 else 100 end) + (case y when 2 then 20 else 200 end) end\n"
        ;

    WHEN( "1, 1" )
        TestCompileAndRunAlgoly( code, sizeof code, 300, { 1, 1 }, 7 );

    WHEN( "1, 2" )
        TestCompileAndRunAlgoly( code, sizeof code, 120, { 1, 2 }, 7 );

    WHEN( "2, 1" )
        TestCompileAndRunAlgoly( code, sizeof code, 210, { 2, 1 }, 7 );

    WHEN( "2, 2" )
        TestCompileAndRunAlgoly( code, sizeof code, 30, { 2, 2 }, 7 );
}

TEST_CASE( "Algoly: StackUse: read global array 2x2 2-const indexes", "[algoly][stack]" )
{
    const char code[] =
        "var i := 1, j := 1\n"
        "var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  ar[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 4, 0, 3 );
}

TEST_CASE( "Algoly: StackUse: read local array 2x2 2-const indexes", "[algoly][stack]" )
{
    const char code[] =
        "var i := 1, j := 1\n"
        "def a\n"
        "  var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  ar[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 4, 0, 7 );
}

TEST_CASE( "Algoly: StackUse: read global array 2x2 1-const index, 1-var index", "[algoly][stack]" )
{
    const char code[] =
        "var i := 1, j := 1\n"
        "var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  ar[1][j]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 4, 0, 4 );
}

TEST_CASE( "Algoly: StackUse: read local array 2x2 1-const index, 1-var index", "[algoly][stack]" )
{
    const char code[] =
        "var i := 1, j := 1\n"
        "def a\n"
        "  var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  ar[1][j]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 4, 0, 8 );
}

TEST_CASE( "Algoly: StackUse: read global array 2x2 1-var index, 1-const index", "[algoly][stack]" )
{
    const char code[] =
        "var i := 1, j := 1\n"
        "var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  ar[i][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 4, 0, 4 );
}

TEST_CASE( "Algoly: StackUse: read local array 2x2 1-var index, 1-const index", "[algoly][stack]" )
{
    const char code[] =
        "var i := 1, j := 1\n"
        "def a\n"
        "  var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  ar[i][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 4, 0, 8 );
}

TEST_CASE( "Algoly: StackUse: read global array 2x2 2-var indexes", "[algoly][stack]" )
{
    const char code[] =
        "var i := 1, j := 1\n"
        "var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  ar[i][j]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 4, 0, 4 );
}

TEST_CASE( "Algoly: StackUse: read local array 2x2 2-var indexes", "[algoly][stack]" )
{
    const char code[] =
        "var t := 0\n"
        "var i := 1, j := 1\n"
        "def a\n"
        "  var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  ar[i][j]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 4, 0, 8 );
}

TEST_CASE( "Algoly: StackUse: write global array 2x2 2-const indexes", "[algoly][stack]" )
{
    const char code[] =
        "var t := 0\n"
        "var i := 1, j := 1\n"
        "var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  ar[1][1] := 5\n"
        "  ar[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 5, 0, 3 );
}

TEST_CASE( "Algoly: StackUse: write local array 2x2 2-const indexes", "[algoly][stack]" )
{
    const char code[] =
        "var t := 0\n"
        "var i := 1, j := 1\n"
        "var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  ar[1][1] := 5\n"
        "  ar[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 5, 0, 7 );
}


TEST_CASE( "Algoly: StackUse: write global array 2x2 2-var indexes", "[algoly][stack]" )
{
    const char code[] =
        "var t := 0\n"
        "var i := 1, j := 1\n"
        "var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  ar[i][j] := 5\n"
        "  ar[i][j]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 5, 0, 5 );
}

TEST_CASE( "Algoly: StackUse: write local array 2x2 2-var indexes", "[algoly][stack]" )
{
    const char code[] =
        "var t := 0\n"
        "var i := 1, j := 1\n"
        "def a\n"
        "  var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  ar[i][j] := 5\n"
        "  ar[i][j]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 5, 0, 9 );
}

TEST_CASE( "Algoly: StackUse: global slice dim1 const indexes", "[algoly][stack]" )
{
    const char code[] =
        "var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  ar[1][1..2][0]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 4, 0, 3 );
}

TEST_CASE( "Algoly: StackUse: global slice dim0 const indexes", "[algoly][stack]" )
{
    const char code[] =
        "var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  ar[1..2][0][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 4, 0, 3 );
}

TEST_CASE( "Algoly: StackUse: global slice dim1 var indexes", "[algoly][stack]" )
{
    const char code[] =
        "var i := 1, j := 0\n"
        "var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  ar[i][1..2][j]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 4, 0, 4 );
}

TEST_CASE( "Algoly: StackUse: global slice dim0 var indexes", "[algoly][stack]" )
{
    const char code[] =
        "var i := 0, j := 1\n"
        "var ar: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  ar[1..2][i][j]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 4, 0, 4 );
}

TEST_CASE( "Algoly: calls across 3 modules", "[algoly][stack]" )
{
    const char* modeCodeB[] =
    {
        "def Z(x) var q := x+2; q end\n"
        ,
        nullptr
    };

    const char* modeCodeA[] =
    {
        "import ModB\n"
        "def Y(p) ModB.Z(p) end\n"
        "def X var n := 3; Y(n) end\n"
        "def W X() end\n"
        ,
        nullptr
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "def a ModA.W() end\n"
        ,
        nullptr
    };

    const ModuleSource modSources[] =
    {
        { "ModB",   modeCodeB },
        { "ModA",   modeCodeA },
        { "Main",   mainCode },
        { },
    };

    TestCompileAndRun( Language::Gema, modSources, 5, 0, 16 );
}

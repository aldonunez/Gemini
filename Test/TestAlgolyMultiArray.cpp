#include "pch.h"
#include "TestBase.h"


//----------------------------------------------------------------------------
// Multidimensional Arrays
//----------------------------------------------------------------------------

/*
* TODO: Plan:
* rank
* count
* storage
* elem type
* slice
* countof
*/

TEST_CASE( "Algoly: Global multi array int 2x2 const indexing", "[algoly][multidim]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [2] of [2] :=\n"
        "      [\n"
        "        [1, 2],\n"
        "        [3, 4]\n"
        "      ]\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 10 );
}

TEST_CASE( "Algoly: Local multi array int 2x2 const indexing", "[algoly][multidim]" )
{
    const char code[] =
        "var ar: [2] of [2] :=\n"
        "      [\n"
        "        [1, 2],\n"
        "        [3, 4]\n"
        "      ]\n"
        "def a\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 10 );
}

TEST_CASE( "Algoly: Global multi array fptr 2x2 var indexing", "[algoly][multidim]" )
{
    const char code[] =
        "var ar: [2] of [2] of &proc :=\n"
        "      [\n"
        "        [&F1, &F2],\n"
        "        [&F3, &F4]\n"
        "      ]\n"
        "def a\n"
        "  var t := 0\n"
        "  for i := 0 to 1 do\n"
        "    for j := 0 to 1 do\n"
        "      t := t + (ar[i][j])()\n"
        "    end\n"
        "  end\n"
        "  t\n"
        "end\n"
        "def F1 1 end\n"
        "def F2 2 end\n"
        "def F3 3 end\n"
        "def F4 4 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 10 );
}

TEST_CASE( "Algoly: Local multi array fptr 2x2 const indexing", "[algoly][multidim]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [2] of [2] of &proc :=\n"
        "      [\n"
        "        [&F1, &F2],\n"
        "        [&F3, &F4]\n"
        "      ]\n"
        "  var t := 0\n"
        "  for i := 0 to 1 do\n"
        "    for j := 0 to 1 do\n"
        "      t := t + (ar[i][j])()\n"
        "    end\n"
        "  end\n"
        "  t\n"
        "end\n"
        "def F1 1 end\n"
        "def F2 2 end\n"
        "def F3 3 end\n"
        "def F4 4 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 10 );
}

TEST_CASE( "Algoly: Global multi array int 3x3x3 var indexing", "[algoly][multidim]" )
{
    const char code[] =
        "var ar: [3] of [3] of [3] :=\n"
        "  [\n"
        "    [[ 1,  2,  3], [ 4,  5,  6], [ 7,  8,  9]],\n"
        "    [[10, 11, 12], [13, 14, 15], [16, 17, 18]],\n"
        "    [[19, 20, 21], [22, 23, 24], [25, 26, 27]]\n"
        "  ]\n"
        "def a\n"
        "  var t := 0\n"
        "  for i := 0 to 2 do\n"
        "    for j := 0 to 2 do\n"
        "      for k := 0 to 2 do\n"
        "        t := t + ar[i][j][k]\n"
        "      end\n"
        "    end\n"
        "  end\n"
        "  t\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 378 );
}

TEST_CASE( "Algoly: Local multi array int 3x3x3 var indexing", "[algoly][multidim]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [3] of [3] of [3] :=\n"
        "  [\n"
        "    [[ 1,  2,  3], [ 4,  5,  6], [ 7,  8,  9]],\n"
        "    [[10, 11, 12], [13, 14, 15], [16, 17, 18]],\n"
        "    [[19, 20, 21], [22, 23, 24], [25, 26, 27]]\n"
        "  ]\n"
        "  var t := 0\n"
        "  for i := 0 to 2 do\n"
        "    for j := 0 to 2 do\n"
        "      for k := 0 to 2 do\n"
        "        t := t + ar[i][j][k]\n"
        "      end\n"
        "    end\n"
        "  end\n"
        "  t\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 378 );
}

TEST_CASE( "Algoly: Global multi array int 2x2 const indexing modify", "[algoly][multidim]" )
{
    const char code[] =
        "  var ar1: [2] of [2] :=\n"
        "      [\n"
        "        [1, 2],\n"
        "        [3, 4]\n"
        "      ]\n"
        "  var ar2: [2] of [2] :=\n"
        "      [\n"
        "        [5, 6],\n"
        "        [7, 8]\n"
        "      ]\n"
        "  var ar3: [2] of [2] :=\n"
        "      [\n"
        "        [ 9, 10],\n"
        "        [11, 12]\n"
        "      ]\n"
        "def a\n"
        "  ar3[0][0] := ar1[0][0] * ar2[0][0]\n"
        "  ar3[0][1] := ar1[0][1] * ar2[0][1]\n"
        "  ar3[1][0] := ar1[1][0] * ar2[1][0]\n"
        "  ar3[1][1] := ar1[1][1] * ar2[1][1]\n"
        "  ar3[0][0] + ar3[0][1] + ar3[1][0] + ar3[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 70 );
}

TEST_CASE( "Algoly: Local multi array int 2x2 const indexing modify", "[algoly][multidim]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [2] of [2] :=\n"
        "      [\n"
        "        [1, 2],\n"
        "        [3, 4]\n"
        "      ]\n"
        "  var ar2: [2] of [2] :=\n"
        "      [\n"
        "        [5, 6],\n"
        "        [7, 8]\n"
        "      ]\n"
        "  var ar3: [2] of [2] :=\n"
        "      [\n"
        "        [ 9, 10],\n"
        "        [11, 12]\n"
        "      ]\n"
        "  ar3[0][0] := ar1[0][0] * ar2[0][0]\n"
        "  ar3[0][1] := ar1[0][1] * ar2[0][1]\n"
        "  ar3[1][0] := ar1[1][0] * ar2[1][0]\n"
        "  ar3[1][1] := ar1[1][1] * ar2[1][1]\n"
        "  ar3[0][0] + ar3[0][1] + ar3[1][0] + ar3[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 70 );
}

TEST_CASE( "Algoly: Global multi array int 2x2 const indexing default [1, 1]", "[algoly][multidim]" )
{
    const char code[] =
        "  var ar: [2] of [2] :=\n"
        "      [\n"
        "        [1, 2],\n"
        "        [3]\n"
        "      ]\n"
        "def a\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 6 );
}

TEST_CASE( "Algoly: Local multi array int 2x2 const indexing default [1, 1]", "[algoly][multidim]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [2] of [2] :=\n"
        "      [\n"
        "        [1, 2],\n"
        "        [3]\n"
        "      ]\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 6 );
}

TEST_CASE( "Algoly: Global multi array int 2x2 const indexing default [1, 0..2]", "[algoly][multidim]" )
{
    const char code[] =
        "  var ar: [2] of [2] :=\n"
        "      [\n"
        "        [1, 2],\n"
        "        []\n"
        "      ]\n"
        "def a\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 3 );
}

TEST_CASE( "Algoly: Local multi array int 2x2 const indexing default [1, 0..2]", "[algoly][multidim]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [2] of [2] :=\n"
        "      [\n"
        "        [1, 2],\n"
        "        []\n"
        "      ]\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 3 );
}

TEST_CASE( "Algoly: Global multi array int 2x2 const indexing default [1]", "[algoly][multidim]" )
{
    const char code[] =
        "  var ar: [2] of [2] :=\n"
        "      [\n"
        "        [1, 2]\n"
        "      ]\n"
        "def a\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 3 );
}

TEST_CASE( "Algoly: Local multi array int 2x2 const indexing default [1]", "[algoly][multidim]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [2] of [2] :=\n"
        "      [\n"
        "        [1, 2]\n"
        "      ]\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 3 );
}

TEST_CASE( "Algoly: Global multi array int 2x2 const indexing repeat [0]", "[algoly][multidim]" )
{
    const char code[] =
        "  var ar: [2] of [2] :=\n"
        "      [\n"
        "        [1, 2]...\n"
        "      ]\n"
        "def a\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 6 );
}

TEST_CASE( "Algoly: Local multi array int 2x2 const indexing repeat [0]", "[algoly][multidim]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [2] of [2] :=\n"
        "      [\n"
        "        [1, 2]...\n"
        "      ]\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 6 );
}

TEST_CASE( "Algoly: Global multi array int 2x2 const indexing default [0, 0..2] and [1]", "[algoly][multidim]" )
{
    const char code[] =
        "  var ar: [2] of [2] :=\n"
        "      [\n"
        "        []\n"
        "      ]\n"
        "def a\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 0 );
}

TEST_CASE( "Algoly: Local multi array int 2x2 const indexing default [0, 0..2] and [1]", "[algoly][multidim]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [2] of [2] :=\n"
        "      [\n"
        "        []\n"
        "      ]\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 0 );
}

TEST_CASE( "Algoly: Global multi array int 2x2 const indexing default [0]", "[algoly][multidim]" )
{
    const char code[] =
        "  var ar: [2] of [2] :=\n"
        "      [\n"
        "        [],\n"
        "        [3, 4]\n"
        "      ]\n"
        "def a\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 7 );
}

TEST_CASE( "Algoly: Local multi array int 2x2 const indexing default [0]", "[algoly][multidim]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [2] of [2] :=\n"
        "      [\n"
        "        [],\n"
        "        [3, 4]\n"
        "      ]\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 7 );
}

TEST_CASE( "Algoly: Global multi array int 2x2 const indexing default [0] and [1]", "[algoly][multidim]" )
{
    const char code[] =
        "  var ar: [2] of [2] :=\n"
        "      [\n"
        "      ]\n"
        "def a\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 0 );
}

TEST_CASE( "Algoly: Local multi array int 2x2 const indexing default [0] and [1]", "[algoly][multidim]" )
{
    const char code[] =
        "def a\n"
        "  var ar: [2] of [2] :=\n"
        "      [\n"
        "      ]\n"
        "  ar[0][0] + ar[0][1] + ar[1][0] + ar[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 0 );
}

TEST_CASE( "Algoly: Global multi array int 3x3x3 slice", "[algoly][multidim]" )
{
    const char code[] =
        "var ar: [3] of [3] of [3] :=\n"
        "  [\n"
        "    [[ 1,  2,  3], [ 4,  5,  6], [ 7,  8,  9]],\n"
        "    [[10, 11, 12], [13, 14, 15], [16, 17, 18]],\n"
        "    [[19, 20, 21], [22, 23, 24], [25, 26, 27]]\n"
        "  ]\n"
        "def a\n"
        "  var t := 0\n"
        "  ar[2][2][1..3][1] := 100\n"
        "  ar[2][1..3][1][1] := 200\n"
        "  ar[1..3][1][2][0] := 300\n"
        "  var t := 0\n"
        "  for i := 0 to 2 do\n"
        "    for j := 0 to 2 do\n"
        "      for k := 0 to 2 do\n"
        "        t := t + ar[i][j][k]\n"
        "      end\n"
        "    end\n"
        "  end\n"
        "  t\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 378-25-26-27+100+200+300 );
}

TEST_CASE( "Algoly: Local multi array int 3x3x3 slice", "[algoly][multidim]" )
{
    const char code[] =
        "var ar: [3] of [3] of [3] :=\n"
        "  [\n"
        "    [[ 1,  2,  3], [ 4,  5,  6], [ 7,  8,  9]],\n"
        "    [[10, 11, 12], [13, 14, 15], [16, 17, 18]],\n"
        "    [[19, 20, 21], [22, 23, 24], [25, 26, 27]]\n"
        "  ]\n"
        "def a\n"
        "  var t := 0\n"
        "  ar[2][2][1..3][1] := 100\n"
        "  ar[2][1..3][1][1] := 200\n"
        "  ar[1..3][1][2][0] := 300\n"
        "  var t := 0\n"
        "  for i := 0 to 2 do\n"
        "    for j := 0 to 2 do\n"
        "      for k := 0 to 2 do\n"
        "        t := t + ar[i][j][k]\n"
        "      end\n"
        "    end\n"
        "  end\n"
        "  t\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 378 - 25 - 26 - 27 + 100 + 200 + 300 );
}

TEST_CASE( "Algoly: Global multi array int 3x2x1 countof", "[algoly][multidim]" )
{
    const char code[] =
        "var ar: [3] of [2] of [1] :=\n"
        "  [\n"
        "    [[ 1], [ 4]],\n"
        "    [[10], [13]],\n"
        "    [[19], [22]]\n"
        "  ]\n"
        "def a\n"
        "  countof( ar ) * 100 +\n"
        "  countof( ar[0] ) * 10 +\n"
        "  countof( ar[0][0] )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 321 );
}

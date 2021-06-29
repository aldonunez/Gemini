#include "pch.h"
#include "TestBase.h"


TEST_CASE( "Algoly: CopyArray: global copy 2 elems", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] := [1, 2]\n"
        "var ar2: [2] := [3, 4]\n"
        "def a\n"
        "  ar1 := ar2\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 7, 0, 5 );
}

TEST_CASE( "Algoly: CopyArray: local copy 2 elems", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [2] := [1, 2]\n"
        "  var ar2: [2] := [3, 4]\n"
        "  ar1 := ar2\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 7, 0, 5+4 );
}

TEST_CASE( "Algoly: CopyArray: global to local copy 2 elems", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] := [3, 4]\n"
        "def a\n"
        "  var ar1: [2] := [1, 2]\n"
        "  ar1 := ar2\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 7, 0, 7 );
}

TEST_CASE( "Algoly: CopyArray: local to global copy 2 elems", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] := [1, 2]\n"
        "def a\n"
        "  var ar2: [2] := [3, 4]\n"
        "  ar1 := ar2\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 7, 0, 7 );
}

//---

TEST_CASE( "Algoly: CopyArray: init global array with global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] := [3, 4]\n"
        "var ar1 := ar2\n"
        "def a\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 7, 0, 4 );
}

TEST_CASE( "Algoly: CopyArray: init local array with local", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar2: [2] := [3, 4]\n"
        "  var ar1 := ar2\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 7, 0, 9 );
}

TEST_CASE( "Algoly: CopyArray: init local array with global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] := [3, 4]\n"
        "def a\n"
        "  var ar1 := ar2\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 7, 0, 7 );
}

// TODO: Consider removing the automatic insertion of zero at the end of stmt list that ends in array copy
#if 0
#endif

TEST_CASE( "Algoly: CopyArray: assign shorter array to longer global-global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] := [4, 5]\n"
        "var ar1: [3] := [1, 2, 3]\n"
        "def a\n"
        "  ar1 := ar2\n"
        "  ar1[0] + ar1[1] + ar1[2]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 12, 0, 5 );
}

TEST_CASE( "Algoly: CopyArray: assign shorter array to longer local-local", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] := [4, 5]\n"
        "var ar1: [3] := [1, 2, 3]\n"
        "def a\n"
        "  ar1 := ar2\n"
        "  ar1[0] + ar1[1] + ar1[2]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 12, 0, 5 );
}

TEST_CASE( "Algoly: CopyArray: init array with shorter array global-global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] := [4, 5]\n"
        "var ar1: [3] := ar2\n"
        "def a\n"
        "  ar2[0] + ar2[1] + \n"
        "  ar1[0] + ar1[1] + ar1[2]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 18, 0, 4 );
}

TEST_CASE( "Algoly: CopyArray: init array with shorter array local-local", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar2: [2] := [4, 5]\n"
        "  var ar1: [3] := ar2\n"
        "  ar2[0] + ar2[1] + \n"
        "  ar1[0] + ar1[1] + ar1[2]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 18, 0, 10 );
}

TEST_CASE( "Algoly: CopyArray: global chained array assignment 3", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] := []\n"
        "var ar2: [2] := []\n"
        "var ar3: [2] := [1, 2]\n"
        "def a\n"
        "  ar1 := ar2 := ar3\n"
        "  ar1[0] + ar1[1] +\n"
        "  ar2[0] + ar2[1] +\n"
        "  ar3[0] + ar3[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 9, 0, 7 );
}

TEST_CASE( "Algoly: CopyArray: local chained array assignment 3", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [2] := []\n"
        "  var ar2: [2] := []\n"
        "  var ar3: [2] := [1, 2]\n"
        "  ar1 := ar2 := ar3\n"
        "  ar1[0] + ar1[1] +\n"
        "  ar2[0] + ar2[1] +\n"
        "  ar3[0] + ar3[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 9, 0, 7 + 6 );
}

TEST_CASE( "Algoly: CopyArray: global chained array assignment 3, slice on right", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] := []\n"
        "var ar2: [2] := []\n"
        "var ar3: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  ar1 := ar2 := ar3[1]\n"
        "  ar1[0] + ar1[1] +\n"
        "  ar2[0] + ar2[1] +\n"
        "  ar3[1][0] + ar3[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 21, 0, 7 );
}

TEST_CASE( "Algoly: CopyArray: local chained array assignment 3, slice on right", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [2] := []\n"
        "  var ar2: [2] := []\n"
        "  var ar3: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  ar1 := ar2 := ar3[1]\n"
        "  ar1[0] + ar1[1] +\n"
        "  ar2[0] + ar2[1] +\n"
        "  ar3[1][0] + ar3[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 21, 0, 7+8 );
}

TEST_CASE( "Algoly: CopyArray: global chained array assignment 3, slice in middle", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] := []\n"
        "var ar2: [2] of [2] := []\n"
        "var ar3: [2] := [3, 4]\n"
        "def a\n"
        "  ar1 := ar2[1] := ar3\n"
        "  ar1[0] + ar1[1] +\n"
        "  ar2[1][0] + ar2[1][1] +\n"
        "  ar3[0] + ar3[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 21, 0, 7 );
}

TEST_CASE( "Algoly: CopyArray: local chained array assignment 3, slice in middle", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [2] := []\n"
        "  var ar2: [2] of [2] := []\n"
        "  var ar3: [2] := [3, 4]\n"
        "  ar1 := ar2[1] := ar3\n"
        "  ar1[0] + ar1[1] +\n"
        "  ar2[1][0] + ar2[1][1] +\n"
        "  ar3[0] + ar3[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 21, 0, 7 + 8 );
}

TEST_CASE( "Algoly: CopyArray: global chained array assignment 3, slice on left", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] of [2] := []\n"
        "var ar2: [2] := []\n"
        "var ar3: [2] := [3, 4]\n"
        "def a\n"
        "  ar1[1] := ar2 := ar3\n"
        "  ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0] + ar2[1] +\n"
        "  ar3[0] + ar3[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 21, 0, 7 );
}

TEST_CASE( "Algoly: CopyArray: local chained array assignment 3, slice on left", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "var ar1: [2] of [2] := []\n"
        "var ar2: [2] := []\n"
        "  var ar3: [2] := [3, 4]\n"
        "  ar1[1] := ar2 := ar3\n"
        "  ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0] + ar2[1] +\n"
        "  ar3[0] + ar3[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 21, 0, 7 + 8 );
}

TEST_CASE( "Algoly: CopyArray: global chained array assignment 3, slice on left and right", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] of [2] := []\n"
        "var ar2: [2] := []\n"
        "var ar3: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  ar1[1] := ar2 := ar3[1]\n"
        "  ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0] + ar2[1] +\n"
        "  ar3[1][0] + ar3[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 21, 0, 7 );
}

TEST_CASE( "Algoly: CopyArray: local chained array assignment 3, slice on left and right", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [2] of [2] := []\n"
        "  var ar2: [2] := []\n"
        "  var ar3: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  ar1[1] := ar2 := ar3[1]\n"
        "  ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0] + ar2[1] +\n"
        "  ar3[1][0] + ar3[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 21, 0, 7+10 );
}

TEST_CASE( "Algoly: CopyArray: global chained array assignment 3, slice all 3", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] of [2] := []\n"
        "var ar2: [2] of [2] := []\n"
        "var ar3: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  ar1[1] := ar2[1] := ar3[1]\n"
        "  ar1[1][0] + ar1[1][1] +\n"
        "  ar2[1][0] + ar2[1][1] +\n"
        "  ar3[1][0] + ar3[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 21, 0, 7 );
}

TEST_CASE( "Algoly: CopyArray: local chained array assignment 3, slice all 3", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [2] of [2] := []\n"
        "  var ar2: [2] of [2] := []\n"
        "  var ar3: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  ar1[1] := ar2[1] := ar3[1]\n"
        "  ar1[1][0] + ar1[1][1] +\n"
        "  ar2[1][0] + ar2[1][1] +\n"
        "  ar3[1][0] + ar3[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 21, 0, 7 + 12 );
}

TEST_CASE( "Algoly: CopyArray: init array with slice, global-global", "[algoly][copy-array]" )
{
    const char code[] =
        // Output: 14
        "var ar2: [2] of [2] := [[1, 2], [3, 4]]\n"
        "var ar1: [2] := ar2[1]\n"
        "def a\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 17, 0, 4 );
}

TEST_CASE( "Algoly: CopyArray: init array with slice, local-local", "[algoly][copy-array]" )
{
    const char code[] =
        // Output: 14
        "def a\n"
        "  var ar2: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  var ar1: [2] := ar2[1]\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 17, 0, 11 );
}

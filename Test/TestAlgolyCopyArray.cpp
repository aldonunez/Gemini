#include "pch.h"
#include "TestBase.h"

using namespace Gemini;


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

    TestCompileAndRunAlgoly( code, 7, 0, 4 );
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

    TestCompileAndRunAlgoly( code, 7, 0, 4+4 );
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

    TestCompileAndRunAlgoly( code, 7, 0, 6 );
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

    TestCompileAndRunAlgoly( code, 7, 0, 6 );
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

    TestCompileAndRunAlgoly( code, 7, 0, 4 );
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

    TestCompileAndRunAlgoly( code, 7, 0, 8 );
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

    TestCompileAndRunAlgoly( code, 7, 0, 6 );
}

//-----

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

    TestCompileAndRunAlgoly( code, 12, 0, 4 );
}

TEST_CASE( "Algoly: CopyArray: assign shorter array to longer local-local", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar2: [2] := [4, 5]\n"
        "  var ar1: [3] := [1, 2, 3]\n"
        "  ar1 := ar2\n"
        "  ar1[0] + ar1[1] + ar1[2]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 12, 0, 4+5 );
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

    TestCompileAndRunAlgoly( code, 18, 0, 4 );
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

    TestCompileAndRunAlgoly( code, 18, 0, 9 );
}

TEST_CASE( "Algoly: init global array by array initializer and copying", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [3, 4]\n"
        "var ar2 := [[1, 2], ar1]\n"
        "def a\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}

TEST_CASE( "Algoly: init local array by array initializer and copying global", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [3, 4]\n"
        "def a\n"
        "  var ar2 := [[1, 2], ar1]\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}

TEST_CASE( "Algoly: init local array by array initializer and copying local", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a\n"
        "  var ar1 := [3, 4]\n"
        "  var ar2 := [[1, 2], ar1]\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}


//----------------------------------------------------------------------------
//  Assign array with indexing
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: CopyArray: assign array with indexing, global-global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] of [2] := [[1, 2], [3, 4]]\n"
        "var ar1: [2] := []\n"
        "def a\n"
        "  ar1 := ar2[1]\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 4 );
}

TEST_CASE( "Algoly: CopyArray: assign array with indexing, global-local", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  var ar1: [2] := []\n"
        "  ar1 := ar2[1]\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 6 );
}

TEST_CASE( "Algoly: CopyArray: assign array with indexing, local-global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] := []\n"
        "def a\n"
        "  var ar2: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  ar1 := ar2[1]\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 8 );
}

TEST_CASE( "Algoly: CopyArray: assign array with indexing, local-local", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar2: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  var ar1: [2] := []\n"
        "  ar1 := ar2[1]\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 10 );
}

TEST_CASE( "Algoly: CopyArray: assign array with dynamic indexing, global-global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] of [2] := [[1, 2], [3, 4]]\n"
        "var ar1: [2] := []\n"
        "var n := 1\n"
        "def a\n"
        "  ar1 := ar2[n]\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 4 );
}

TEST_CASE( "Algoly: CopyArray: assign array with dynamic indexing, global-local", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] of [2] := [[1, 2], [3, 4]]\n"
        "var n := 1\n"
        "def a\n"
        "  var ar1: [2] := []\n"
        "  ar1 := ar2[n]\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 6 );
}

TEST_CASE( "Algoly: CopyArray: assign array with dynamic indexing, local-global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] := []\n"
        "var n := 1\n"
        "def a\n"
        "  var ar2: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  ar1 := ar2[n]\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 8 );
}

TEST_CASE( "Algoly: CopyArray: assign array with dynamic indexing, local-local", "[algoly][copy-array]" )
{
    const char code[] =
        "var n := 1\n"
        "def a\n"
        "  var ar2: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  var ar1: [2] := []\n"
        "  ar1 := ar2[n]\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 10 );
}

//----------

TEST_CASE( "Algoly: CopyArray: assign array with indexing on left, global-global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] of [2] := [[1, 2], []]\n"
        "var ar2: [2] := [3, 4]\n"
        "def a\n"
        "  ar1[1] := ar2\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0] + ar2[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 4 );
}

TEST_CASE( "Algoly: CopyArray: assign array with indexing on left, global-local", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] of [2] := [[1, 2], []]\n"
        "def a\n"
        "  var ar2: [2] := [3, 4]\n"
        "  ar1[1] := ar2\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0] + ar2[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 6 );
}

TEST_CASE( "Algoly: CopyArray: assign array with indexing on left, local-global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] := [3, 4]\n"
        "def a\n"
        "  var ar1: [2] of [2] := [[1, 2], []]\n"
        "  ar1[1] := ar2\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0] + ar2[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 8 );
}

TEST_CASE( "Algoly: CopyArray: assign array with indexing on left, local-local", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [2] of [2] := [[1, 2], []]\n"
        "  var ar2: [2] := [3, 4]\n"
        "  ar1[1] := ar2\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0] + ar2[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 10 );
}

TEST_CASE( "Algoly: CopyArray: assign array with dynamic indexing on left, global-global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] of [2] := [[1, 2], []]\n"
        "var ar2: [2] := [3, 4]\n"
        "var n := 1\n"
        "def a\n"
        "  ar1[n] := ar2\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0] + ar2[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 5 );
}

TEST_CASE( "Algoly: CopyArray: assign array with dynamic indexing on left, global-local", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] of [2] := [[1, 2], []]\n"
        "var n := 1\n"
        "def a\n"
        "  var ar2: [2] := [3, 4]\n"
        "  ar1[n] := ar2\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0] + ar2[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 7 );
}

TEST_CASE( "Algoly: CopyArray: assign array with dynamic indexing on left, local-global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] := [3, 4]\n"
        "var n := 1\n"
        "def a\n"
        "  var ar1: [2] of [2] := [[1, 2], []]\n"
        "  ar1[n] := ar2\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0] + ar2[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 9 );
}

TEST_CASE( "Algoly: CopyArray: assign array with dynamic indexing on left, local-local", "[algoly][copy-array]" )
{
    const char code[] =
        "var n := 1\n"
        "def a\n"
        "  var ar1: [2] of [2] := [[1, 2], []]\n"
        "  var ar2: [2] := [3, 4]\n"
        "  ar1[n] := ar2\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0] + ar2[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 11 );
}

//----------

TEST_CASE( "Algoly: CopyArray: assign array with indexing both, global-global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] of [2] := [[1, 2], []]\n"
        "var ar2: [2] of [2] := [[], [3, 4]]\n"
        "def a\n"
        "  ar1[1] := ar2[1]\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 4 );
}

TEST_CASE( "Algoly: CopyArray: assign array with indexing both, global-local", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] of [2] := [[1, 2], []]\n"
        "def a\n"
        "  var ar2: [2] of [2] := [[], [3, 4]]\n"
        "  ar1[1] := ar2[1]\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 6+2 );
}

TEST_CASE( "Algoly: CopyArray: assign array with indexing both, local-global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] of [2] := [[], [3, 4]]\n"
        "def a\n"
        "  var ar1: [2] of [2] := [[1, 2], []]\n"
        "  ar1[1] := ar2[1]\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 6 + 2 );
}

TEST_CASE( "Algoly: CopyArray: assign array with indexing both, local-local", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [2] of [2] := [[1, 2], []]\n"
        "  var ar2: [2] of [2] := [[], [3, 4]]\n"
        "  ar1[1] := ar2[1]\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 10+2 );
}

TEST_CASE( "Algoly: CopyArray: assign array with dynamic indexing both, global-global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] of [2] := [[1, 2], []]\n"
        "var ar2: [2] of [2] := [[], [3, 4]]\n"
        "var n := 1\n"
        "def a\n"
        "  ar1[n] := ar2[n]\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 5 );
}

TEST_CASE( "Algoly: CopyArray: assign array with dynamic indexing both, global-local", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] of [2] := [[1, 2], []]\n"
        "var n := 1\n"
        "def a\n"
        "  var ar2: [2] of [2] := [[], [3, 4]]\n"
        "  ar1[n] := ar2[n]\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 7+2 );
}

TEST_CASE( "Algoly: CopyArray: assign array with dynamic indexing both, local-global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] of [2] := [[], [3, 4]]\n"
        "var n := 1\n"
        "def a\n"
        "  var ar1: [2] of [2] := [[1, 2], []]\n"
        "  ar1[n] := ar2[n]\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 7 + 2 );
}

TEST_CASE( "Algoly: CopyArray: assign array with dynamic indexing both, local-local", "[algoly][copy-array]" )
{
    const char code[] =
        "var n := 1\n"
        "def a\n"
        "  var ar1: [2] of [2] := [[1, 2], []]\n"
        "  var ar2: [2] of [2] := [[], [3, 4]]\n"
        "  ar1[n] := ar2[n]\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 11+2 );
}


//----------------------------------------------------------------------------
//  Chained assignment 3
//----------------------------------------------------------------------------

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

    TestCompileAndRunAlgoly( code, 9, 0, 5 );
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

    TestCompileAndRunAlgoly( code, 9, 0, 5 + 6 );
}

TEST_CASE( "Algoly: CopyArray: global chained array assignment 3, indexing on right", "[algoly][copy-array]" )
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

    TestCompileAndRunAlgoly( code, 21, 0, 5 );
}

TEST_CASE( "Algoly: CopyArray: local chained array assignment 3, indexing on right", "[algoly][copy-array]" )
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

    TestCompileAndRunAlgoly( code, 21, 0, 5+8 );
}

TEST_CASE( "Algoly: CopyArray: global chained array assignment 3, indexing in middle", "[algoly][copy-array]" )
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

    TestCompileAndRunAlgoly( code, 21, 0, 5 );
}

TEST_CASE( "Algoly: CopyArray: local chained array assignment 3, indexing in middle", "[algoly][copy-array]" )
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

    TestCompileAndRunAlgoly( code, 21, 0, 5 + 8 );
}

TEST_CASE( "Algoly: CopyArray: global chained array assignment 3, indexing on left", "[algoly][copy-array]" )
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

    TestCompileAndRunAlgoly( code, 21, 0, 5 );
}

TEST_CASE( "Algoly: CopyArray: local chained array assignment 3, indexing on left", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [2] of [2] := []\n"
        "  var ar2: [2] := []\n"
        "  var ar3: [2] := [3, 4]\n"
        "  ar1[1] := ar2 := ar3\n"
        "  ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0] + ar2[1] +\n"
        "  ar3[0] + ar3[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 21, 0, 5 + 8 );
}

TEST_CASE( "Algoly: CopyArray: global chained array assignment 3, indexing on left and right", "[algoly][copy-array]" )
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

    TestCompileAndRunAlgoly( code, 21, 0, 5 );
}

TEST_CASE( "Algoly: CopyArray: local chained array assignment 3, indexing on left and right", "[algoly][copy-array]" )
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

    TestCompileAndRunAlgoly( code, 21, 0, 5+10 );
}

TEST_CASE( "Algoly: CopyArray: global chained array assignment 3, indexing all 3", "[algoly][copy-array]" )
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

    TestCompileAndRunAlgoly( code, 21, 0, 5 );
}

TEST_CASE( "Algoly: CopyArray: local chained array assignment 3, indexing all 3", "[algoly][copy-array]" )
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

    TestCompileAndRunAlgoly( code, 21, 0, 5 + 12 );
}


//----------------------------------------------------------------------------
//  Init array with indexing
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: CopyArray: init array with indexing, global-global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] of [2] := [[1, 2], [3, 4]]\n"
        "var ar1: [2] := ar2[1]\n"
        "def a\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 4 );
}

TEST_CASE( "Algoly: CopyArray: init array with indexing, global-local", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  var ar1: [2] := ar2[1]\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 6 );
}

TEST_CASE( "Algoly: CopyArray: init array with indexing, local-local", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar2: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  var ar1: [2] := ar2[1]\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 10 );
}

TEST_CASE( "Algoly: CopyArray: init array with dynamic indexing, global-local", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] of [2] := [[1, 2], [3, 4]]\n"
        "var n := 1\n"
        "def a\n"
        "  var ar1: [2] := ar2[n]\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 6 );
}

TEST_CASE( "Algoly: CopyArray: init array with dynamic indexing, local-local", "[algoly][copy-array]" )
{
    const char code[] =
        "var n := 1\n"
        "def a\n"
        "  var ar2: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  var ar1: [2] := ar2[n]\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 10 );
}


//----------------------------------------------------------------------------
//  Self
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: CopyArray: global copy self", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [4] := [1, 2, 3, 4]\n"
        "def a\n"
        "  ar1 := ar1\n"
        "  ar1[0] + ar1[1] + ar1[2] + ar1[3]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10, 0, 4 );
}

TEST_CASE( "Algoly: CopyArray: local copy self", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [4] := [1, 2, 3, 4]\n"
        "  ar1 := ar1\n"
        "  ar1[0] + ar1[1] + ar1[2] + ar1[3]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10, 0, 4 + 4 );
}

TEST_CASE( "Algoly: CopyArray: global copy self slice", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [4] := [1, 2, 3, 4]\n"
        "def a\n"
        "  ar1[0..2] := ar1[2..4]\n"
        "  ar1[0] + ar1[1] + ar1[2] + ar1[3]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 14, 0, 4 );
}

TEST_CASE( "Algoly: CopyArray: local copy self slice", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "var ar1: [4] := [1, 2, 3, 4]\n"
        "  ar1[0..2] := ar1[2..4]\n"
        "  ar1[0] + ar1[1] + ar1[2] + ar1[3]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 14, 0, 4 + 4 );
}


//----------------------------------------------------------------------------
//  Slice
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: CopyArray: global chained array assignment 3 slices, indexing on right", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [6] := [1, 2, 3, 4, 5, 6]\n"
        "var ar2: [4] := [10, 20, 30, 40]\n"
        "var ar3: [4] := [100, 200, 300, 400]\n"
        "def a\n"
        "  ar1[4..6] := ar2[0..2] := ar3[2..4]\n"
        "  ar1[0] + ar1[1] + ar1[2] + ar1[3] + ar1[4] + ar1[5]+\n"
        "  ar2[0] + ar2[1] + ar2[2] + ar2[3] +\n"
        "  ar3[0] + ar3[1] + ar3[2] + ar3[3]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 1000+770+710, 0, 5 );
}

TEST_CASE( "Algoly: CopyArray: local chained array assignment 3 slices, indexing on right", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [6] := [1, 2, 3, 4, 5, 6]\n"
        "  var ar2: [4] := [10, 20, 30, 40]\n"
        "  var ar3: [4] := [100, 200, 300, 400]\n"
        "  ar1[4..6] := ar2[0..2] := ar3[2..4]\n"
        "  ar1[0] + ar1[1] + ar1[2] + ar1[3] + ar1[4] + ar1[5]+\n"
        "  ar2[0] + ar2[1] + ar2[2] + ar2[3] +\n"
        "  ar3[0] + ar3[1] + ar3[2] + ar3[3]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 1000+770+710, 0, 5 + 14 );
}


TEST_CASE( "Algoly: CopyArray: init array with slice, global-global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [4] := [1, 2, 3, 4]\n"
        "var ar1: [4] := ar2[2..4]\n"
        "def a\n"
        "  ar2[0] + ar2[1] + ar2[2] + ar2[3] +\n"
        "  ar1[0] + ar1[1] + ar1[2] + ar1[3]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 4 );
}

TEST_CASE( "Algoly: CopyArray: init array with slice, global-local", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [4] := [1, 2, 3, 4]\n"
        "def a\n"
        "  var ar1: [4] := ar2[2..4]\n"
        "  ar2[0] + ar2[1] + ar2[2] + ar2[3] +\n"
        "  ar1[0] + ar1[1] + ar1[2] + ar1[3]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 8 );
}

TEST_CASE( "Algoly: CopyArray: init array with slice, local-local", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar2: [4] := [1, 2, 3, 4]\n"
        "  var ar1: [4] := ar2[2..4]\n"
        "  ar2[0] + ar2[1] + ar2[2] + ar2[3] +\n"
        "  ar1[0] + ar1[1] + ar1[2] + ar1[3]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0, 12 );
}


//----------------------------------------------------------------------------
//  Again but with multidim
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: CopyArray: global copy 2x2 elems", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] of [2] := [[1, 2], [5, 6]]\n"
        "var ar2: [2] of [2] := [[3, 4], [7, 8]]\n"
        "def a\n"
        "  ar1 := ar2\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 22, 0, 4 );
}

TEST_CASE( "Algoly: CopyArray: local copy 2x2 elems", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [2] of [2] := [[1, 2], [5, 6]]\n"
        "  var ar2: [2] of [2] := [[3, 4], [7, 8]]\n"
        "  ar1 := ar2\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 22, 0, 4 + 8 );
}

TEST_CASE( "Algoly: CopyArray: global to local copy 2x2 elems", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] of [2] := [[3, 4], [7, 8]]\n"
        "def a\n"
        "  var ar1: [2] of [2] := [[1, 2], [5, 6]]\n"
        "  ar1 := ar2\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 22, 0, 8 );
}

TEST_CASE( "Algoly: CopyArray: local to global copy 2x2 elems", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] of [2] := [[1, 2], [5, 6]]\n"
        "def a\n"
        "  var ar2: [2] of [2] := [[3, 4], [7, 8]]\n"
        "  ar1 := ar2\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 22, 0, 8 );
}

//---

TEST_CASE( "Algoly: CopyArray: init global array 2x2 with global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] of [2] := [[3, 4], [7, 8]]\n"
        "var ar1 := ar2\n"
        "def a\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 22, 0, 4 );
}

TEST_CASE( "Algoly: CopyArray: init local array 2x2 with local", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar2: [2] of [2] := [[3, 4], [7, 8]]\n"
        "  var ar1 := ar2\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 22, 0, 12 );
}

TEST_CASE( "Algoly: CopyArray: init local array 2x2 with global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] of [2] := [[3, 4], [7, 8]]\n"
        "def a\n"
        "  var ar1 := ar2\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 22, 0, 8 );
}


//----------

TEST_CASE( "Algoly: CopyArray: assign shorter array 2x2 to longer global-global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] of [2] := [[7, 8], [9, 10]]\n"
        "var ar1: [3] of [2] := [[1, 2], [3, 4], [5, 6]]\n"
        "def a\n"
        "  ar1 := ar2\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] + ar1[2][0] + ar1[2][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 45, 0, 4 );
}

TEST_CASE( "Algoly: CopyArray: assign shorter array 2x2 to longer local-local", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar2: [2] of [2] := [[7, 8], [9, 10]]\n"
        "  var ar1: [3] of [2] := [[1, 2], [3, 4], [5, 6]]\n"
        "  ar1 := ar2\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] + ar1[2][0] + ar1[2][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 45, 0, 4+10 );
}

TEST_CASE( "Algoly: CopyArray: init array with shorter array 2x2 global-global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] of [2] := [[7, 8], [9, 10]]\n"
        "var ar1: [3] of [2] := ar2\n"
        "def a\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] + \n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] + ar1[2][0] + ar1[2][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 34*2, 0, 4 );
}

TEST_CASE( "Algoly: CopyArray: init array with shorter array 2x2 local-local", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar2: [2] of [2] := [[7, 8], [9, 10]]\n"
        "  var ar1: [3] of [2] := ar2\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] + \n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] + ar1[2][0] + ar1[2][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 34*2, 0, 14 );
}

TEST_CASE( "Algoly: CopyArray: global chained array 2x2 assignment 3", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] of [2] := []\n"
        "var ar2: [2] of [2] := []\n"
        "var ar3: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  ar1 := ar2 := ar3\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar3[0][0] + ar3[0][1] + ar3[1][0] + ar3[1][1] \n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30, 0, 5 );
}

TEST_CASE( "Algoly: CopyArray: local chained array 2x2 assignment 3", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [2] of [2] := []\n"
        "  var ar2: [2] of [2] := []\n"
        "  var ar3: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  ar1 := ar2 := ar3\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar3[0][0] + ar3[0][1] + ar3[1][0] + ar3[1][1] \n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30, 0, 5 + 12 );
}

TEST_CASE( "Algoly: CopyArray: global chained array 2x2 assignment 3, indexing on right", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] of [2] := []\n"
        "var ar2: [2] of [2] := []\n"
        "var ar3: [2] of [2] of [2] := [[[5, 6], [7, 8]], [[1, 2], [3, 4]]]\n"
        "def a\n"
        "  ar1 := ar2 := ar3[1]\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar3[1][0][0] + ar3[1][0][1] + ar3[1][1][0] + ar3[1][1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30, 0, 5 );
}

TEST_CASE( "Algoly: CopyArray: local chained array 2x2 assignment 3, indexing on right", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [2] of [2] := []\n"
        "  var ar2: [2] of [2] := []\n"
        "  var ar3: [2] of [2] of [2] := [[[5, 6], [7, 8]], [[1, 2], [3, 4]]]\n"
        "  ar1 := ar2 := ar3[1]\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar3[1][0][0] + ar3[1][0][1] + ar3[1][1][0] + ar3[1][1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30, 0, 5 + 16 );
}

TEST_CASE( "Algoly: CopyArray: global chained array 2x2 assignment 3, indexing in middle", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] of [2] := []\n"
        "var ar2: [2] of [2] of [2] := []\n"
        "var ar3: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  ar1 := ar2[1] := ar3\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[1][0][0] + ar2[1][0][1] + ar2[1][1][0] + ar2[1][1][1] +\n"
        "  ar3[0][0] + ar3[0][1] + ar3[1][0] + ar3[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30, 0, 5 );
}

TEST_CASE( "Algoly: CopyArray: local chained array 2x2 assignment 3, indexing in middle", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [2] of [2] := []\n"
        "  var ar2: [2] of [2] of [2] := []\n"
        "  var ar3: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  ar1 := ar2[1] := ar3\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  ar2[1][0][0] + ar2[1][0][1] + ar2[1][1][0] + ar2[1][1][1] +\n"
        "  ar3[0][0] + ar3[0][1] + ar3[1][0] + ar3[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30, 0, 5 + 16 );
}

TEST_CASE( "Algoly: CopyArray: global chained array 2x2 assignment 3, indexing on left", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] of [2] of [2] := []\n"
        "var ar2: [2] of [2] := []\n"
        "var ar3: [2] of [2] := [[1, 2], [3, 4]]\n"
        "def a\n"
        "  ar1[1] := ar2 := ar3\n"
        "  ar1[1][0][0] + ar1[1][0][1] + ar1[1][1][0] + ar1[1][1][1] +\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar3[0][0] + ar3[0][1] + ar3[1][0] + ar3[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30, 0, 5 );
}

TEST_CASE( "Algoly: CopyArray: local chained array 2x2 assignment 3, indexing on left", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [2] of [2] of [2] := []\n"
        "  var ar2: [2] of [2] := []\n"
        "  var ar3: [2] of [2] := [[1, 2], [3, 4]]\n"
        "  ar1[1] := ar2 := ar3\n"
        "  ar1[1][0][0] + ar1[1][0][1] + ar1[1][1][0] + ar1[1][1][1] +\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar3[0][0] + ar3[0][1] + ar3[1][0] + ar3[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30, 0, 5 + 16 );
}

TEST_CASE( "Algoly: CopyArray: global chained array 2x2 assignment 3, indexing on left and right", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] of [2] of [2] := []\n"
        "var ar2: [2] of [2] := []\n"
        "var ar3: [2] of [2] of [2] := [[[5, 6], [7, 8]], [[1, 2], [3, 4]]]\n"
        "def a\n"
        "  ar1[1] := ar2 := ar3[1]\n"
        "  ar1[1][0][0] + ar1[1][0][1] + ar1[1][1][0] + ar1[1][1][1] +\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar3[1][0][0] + ar3[1][0][1] + ar3[1][1][0] + ar3[1][1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30, 0, 5 );
}

TEST_CASE( "Algoly: CopyArray: local chained array 2x2 assignment 3, indexing on left and right", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [2] of [2] of [2] := []\n"
        "  var ar2: [2] of [2] := []\n"
        "  var ar3: [2] of [2] of [2] := [[[5, 6], [7, 8]], [[1, 2], [3, 4]]]\n"
        "  ar1[1] := ar2 := ar3[1]\n"
        "  ar1[1][0][0] + ar1[1][0][1] + ar1[1][1][0] + ar1[1][1][1] +\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar3[1][0][0] + ar3[1][0][1] + ar3[1][1][0] + ar3[1][1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30, 0, 5 + 20 );
}

TEST_CASE( "Algoly: CopyArray: global chained array 2x2 assignment 3, indexing all 3", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] of [2] of [2] := []\n"
        "var ar2: [2] of [2] of [2] := []\n"
        "var ar3: [2] of [2] of [2] := [[[5, 6], [7, 8]], [[1, 2], [3, 4]]]\n"
        "def a\n"
        "  ar1[1] := ar2[1] := ar3[1]\n"
        "  ar1[1][0][0] + ar1[1][0][1] + ar1[1][1][0] + ar1[1][1][1] +\n"
        "  ar2[1][0][0] + ar2[1][0][1] + ar2[1][1][0] + ar2[1][1][1] +\n"
        "  ar3[1][0][0] + ar3[1][0][1] + ar3[1][1][0] + ar3[1][1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30, 0, 5 );
}

TEST_CASE( "Algoly: CopyArray: local chained array 2x2 assignment 3, indexing all 3", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "var ar1: [2] of [2] of [2] := []\n"
        "var ar2: [2] of [2] of [2] := []\n"
        "var ar3: [2] of [2] of [2] := [[[5, 6], [7, 8]], [[1, 2], [3, 4]]]\n"
        "  ar1[1] := ar2[1] := ar3[1]\n"
        "  ar1[1][0][0] + ar1[1][0][1] + ar1[1][1][0] + ar1[1][1][1] +\n"
        "  ar2[1][0][0] + ar2[1][0][1] + ar2[1][1][0] + ar2[1][1][1] +\n"
        "  ar3[1][0][0] + ar3[1][0][1] + ar3[1][1][0] + ar3[1][1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 30, 0, 5 + 24 );
}

TEST_CASE( "Algoly: CopyArray: init array 2x2 with indexing, global-global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] of [2] of [2] := [[[5, 6], [7, 8]], [[1, 2], [3, 4]]]\n"
        "var ar1: [2] of [2] := ar2[1]\n"
        "def a\n"
        "  ar2[1][0][0] + ar2[1][0][1] + ar2[1][1][0] + ar2[1][1][1] +\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 20, 0, 4 );
}

TEST_CASE( "Algoly: CopyArray: init array 2x2 with indexing, local-local", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar2: [2] of [2] of [2] := [[[5, 6], [7, 8]], [[1, 2], [3, 4]]]\n"
        "  var ar1: [2] of [2] := ar2[1]\n"
        "  ar2[1][0][0] + ar2[1][0][1] + ar2[1][1][0] + ar2[1][1][1] +\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 20, 0, 4 + 12 );
}


//----------------------------------------------------------------------------
//  Self
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: CopyArray: global copy self 2x2", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [4] of [2] := [[1, 2], [3, 4], [5, 6], [7, 8]]\n"
        "def a\n"
        "  ar1 := ar1\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] + ar1[2][0] + ar1[2][1] + ar1[3][0] + ar1[3][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 36, 0, 4 );
}

TEST_CASE( "Algoly: CopyArray: local copy self 2x2", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [4] of [2] := [[1, 2], [3, 4], [5, 6], [7, 8]]\n"
        "  ar1 := ar1\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] + ar1[2][0] + ar1[2][1] + ar1[3][0] + ar1[3][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 36, 0, 4 + 8 );
}

TEST_CASE( "Algoly: CopyArray: global copy self slice 2x2", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [4] of [2] := [[1, 2], [3, 4], [5, 6], [7, 8]]\n"
        "def a\n"
        "  ar1[0..2] := ar1[2..4]\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] + ar1[2][0] + ar1[2][1] + ar1[3][0] + ar1[3][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 52, 0, 4 );
}

TEST_CASE( "Algoly: CopyArray: local copy self slice 2x2", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [4] of [2] := [[1, 2], [3, 4], [5, 6], [7, 8]]\n"
        "  ar1[0..2] := ar1[2..4]\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] + ar1[2][0] + ar1[2][1] + ar1[3][0] + ar1[3][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 52, 0, 4 + 8 );
}


//----------------------------------------------------------------------------
//  Slice
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: CopyArray: global chained array 2x2 assignment 3 slices, indexing on right", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [3] of [2] := [[1, 2], [3, 4], [5, 6]]\n"
        "var ar2: [2] of [2] := [[10, 20], [30, 40]]\n"
        "var ar3: [2] of [2] := [[100, 200], [300, 400]]\n"
        "def a\n"
        "  ar1[2..3] := ar2[0..1] := ar3[1..2]\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] + ar1[2][0] + ar1[2][1] +\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar3[0][0] + ar3[0][1] + ar3[1][0] + ar3[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 1000 + 770 + 710, 0, 5 );
}

TEST_CASE( "Algoly: CopyArray: local chained array 2x2 assignment 3 slices, indexing on right", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [3] of [2] := [[1, 2], [3, 4], [5, 6]]\n"
        "  var ar2: [2] of [2] := [[10, 20], [30, 40]]\n"
        "  var ar3: [2] of [2] := [[100, 200], [300, 400]]\n"
        "  ar1[2..3] := ar2[0..1] := ar3[1..2]\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] + ar1[2][0] + ar1[2][1] +\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] +\n"
        "  ar3[0][0] + ar3[0][1] + ar3[1][0] + ar3[1][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 1000 + 770 + 710, 0, 5 + 14 );
}


TEST_CASE( "Algoly: CopyArray: init array 2x2 with slice, global-global", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [4] of [2] := [[1, 2], [3, 4], [5, 6], [7, 8]]\n"
        "var ar1: [4] of [2] := ar2[2..4]\n"
        "def a\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] + ar2[2][0] + ar2[2][1] + ar2[3][0] + ar2[3][1] +\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] + ar1[2][0] + ar1[2][1] + ar1[3][0] + ar1[3][1] \n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 36+26, 0, 4 );
}

TEST_CASE( "Algoly: CopyArray: init array 2x2 with slice, global-local", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [4] of [2] := [[1, 2], [3, 4], [5, 6], [7, 8]]\n"
        "def a\n"
        "  var ar1: [4] of [2] := ar2[2..4]\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] + ar2[2][0] + ar2[2][1] + ar2[3][0] + ar2[3][1] +\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] + ar1[2][0] + ar1[2][1] + ar1[3][0] + ar1[3][1] \n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 36+26, 0, 4+8 );
}

TEST_CASE( "Algoly: CopyArray: init array 2x2 with slice, local-local", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar2: [4] of [2] := [[1, 2], [3, 4], [5, 6], [7, 8]]\n"
        "  var ar1: [4] of [2] := ar2[2..4]\n"
        "  ar2[0][0] + ar2[0][1] + ar2[1][0] + ar2[1][1] + ar2[2][0] + ar2[2][1] + ar2[3][0] + ar2[3][1] +\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] + ar1[2][0] + ar1[2][1] + ar1[3][0] + ar1[3][1] \n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 36+26, 0, 4+16 );
}


//----------------------------------------------------------------------------
//  Access assigned array
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: CopyArray: read indexed assigned array", "[algoly][copy-array][negative]" )
{
    const char code[] =
        "var ar1: [2] := [1, 2]\n"
        "var ar2: [2] := [3, 4]\n"
        "def a\n"
        "  (ar1 := ar2)[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: CopyArray: write indexed assigned array", "[algoly][copy-array][negative]" )
{
    const char code[] =
        "var ar1: [2] := [1, 2]\n"
        "var ar2: [2] := [3, 4]\n"
        "def a\n"
        "  (ar1 := ar2)[1] := 10\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: CopyArray: read sliced assigned array", "[algoly][copy-array][negative]" )
{
    const char code[] =
        "var ar1: [2] := [1, 2]\n"
        "var ar2: [2] := [3, 4]\n"
        "def a\n"
        "  (ar1 := ar2)[0..2][1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: CopyArray: write sliced assigned array", "[algoly][copy-array][negative]" )
{
    const char code[] =
        "var ar1: [2] := [1, 2]\n"
        "var ar2: [2] := [3, 4]\n"
        "def a\n"
        "  (ar1 := ar2)[0..2][1] := 10\n"
        "  ar1[0] + ar1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}


//----------------------------------------------------------------------------
//  Open array
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: CopyArray: global copy open to open", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] := [1, 2]\n"
        "var ar2: [2] := [5, 6]\n"
        "def a\n"
        "  B( @ar1, @ar2 )\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1]\n"
        "end\n"
        "def B(var array1: [], var array2: [])\n"
        "  array1 := array2\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 22 );
}

TEST_CASE( "Algoly: CopyArray: global copy open to closed", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] := [1, 2]\n"
        "var ar2: [2] := [5, 6]\n"
        "def a\n"
        "  B( ar1, @ar2 )\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1]\n"
        "end\n"
        "def B(var array1: [2], var array2: [])\n"
        "  array1 := array2\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 22 );
}

TEST_CASE( "Algoly: CopyArray: global copy closed to open", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [2] := [1, 2]\n"
        "var ar2: [2] := [5, 6]\n"
        "def a\n"
        "  B( @ar1, ar2 )\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1]\n"
        "end\n"
        "def B(var array1: [], var array2: [2])\n"
        "  array1 := array2\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 22 );
}

TEST_CASE( "Algoly: CopyArray: local init with open", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] := [5, 6]\n"
        "var n := 0\n"
        "def a\n"
        "  n := B( @ar2 )\n"
        "  n + ar2[0] + ar2[1]\n"
        "end\n"
        "def B(var array2: [])\n"
        "  var array1: [2] := array2\n"
        "  array1[0] + array1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 22 );
}

TEST_CASE( "Algoly: CopyArray: local init with closed slice of open", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] := [5, 6]\n"
        "var n := 0\n"
        "def a\n"
        "  n := B( @ar2 )\n"
        "  n + ar2[0] + ar2[1]\n"
        "end\n"
        "def B(var array2: [])\n"
        "  var array1: [2] := array2[0..2]\n"
        "  array1[0] + array1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 22 );
}

TEST_CASE( "Algoly: CopyArray: local init with open slice of open", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar2: [2] := [5, 6]\n"
        "var n := 0\n"
        "def a\n"
        "  n := B( @ar2 )\n"
        "  n + ar2[0] + ar2[1]\n"
        "end\n"
        "def B(var array2: [])\n"
        "  var zero := 0, two := 2\n"
        "  var array1: [2] := array2[zero..two]\n"
        "  array1[0] + array1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 22 );
}

TEST_CASE( "Algoly: CopyArray: global copy slice of open of slice", "[algoly][copy-array]" )
{
    const char code[] =
        "var ar1: [4] := [1, 2, 3, 4]\n"
        "var ar2: [4] := [5, 6, 7, 8]\n"
        "def a\n"
        "  B( @ar1[0..3], @ar2[1..4] )\n"
        "  ar1[0] + ar1[1] + ar1[2] + ar1[3] + \n"
        "  ar2[0] + ar2[1] + ar2[2] + ar2[3]\n"
        "end\n"
        "def B(var array1: [], var array2: [])\n"
        "  array1[1..3] := array2[1..3]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 46 );
}

TEST_CASE( "Algoly: CopyArray: local copy slice of open of slice", "[algoly][copy-array]" )
{
    const char code[] =
        "def a\n"
        "  var ar1: [4] := [1, 2, 3, 4]\n"
        "  var ar2: [4] := [5, 6, 7, 8]\n"
        "  B( @ar1[0..3], @ar2[1..4] )\n"
        "  ar1[0] + ar1[1] + ar1[2] + ar1[3] + \n"
        "  ar2[0] + ar2[1] + ar2[2] + ar2[3]\n"
        "end\n"
        "def B(var array1: [], var array2: [])\n"
        "  array1[1..3] := array2[1..3]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 46 );
}

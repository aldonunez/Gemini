#include "pch.h"
#include "TestBase.h"

using namespace Gemini;


//----------------------------------------------------------------------------
//  Open array values (capture)
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: CopyArray: global capture open to open", "[algoly][open-array-value]" )
{
    const char code[] =
        "var ar1: [2] := [1, 2]\n"
        "var ar2: [2] := [5, 6]\n"
        "var n := 0\n"
        "def a\n"
        "  n := B( @ar1, @ar2 )\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1] + n\n"
        "end\n"
        "def B(var array1: [], var array2: [])\n"
        "  array1 := @array2\n"
        "  array1[0] + array1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 22 + 3 );
}

TEST_CASE( "Algoly: CopyArray: local capture open param, init", "[algoly][open-array-value]" )
{
    const char code[] =
        "var ar1: [2] := [1, 2]\n"
        "var ar2: [2] := [5, 6]\n"
        "def a\n"
        "  B( @ar1, @ar2 )\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1]\n"
        "end\n"
        "def B(var array1: [], var array2: [])\n"
        "  var array3: [] := @array2\n"
        "  array3[0] := 3\n"
        "  array3[1] := 4\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}

TEST_CASE( "Algoly: CopyArray: local capture open param, assignment", "[algoly][open-array-value]" )
{
    const char code[] =
        "var ar1: [2] := [1, 2]\n"
        "var ar2: [2] := [5, 6]\n"
        "def a\n"
        "  B( @ar1, @ar2 )\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1]\n"
        "end\n"
        "def B(var array1: [], var array2: [])\n"
        "  var array3: [] := @array1\n"
        "  array3 := @array2\n"
        "  array3[0] := 3\n"
        "  array3[1] := 4\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}

TEST_CASE( "Algoly: CopyArray: local capture open local, init", "[algoly][open-array-value]" )
{
    const char code[] =
        "var ar1: [2] := [1, 2]\n"
        "var ar2: [2] := [5, 6]\n"
        "def a\n"
        "  B( @ar1, @ar2 )\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1]\n"
        "end\n"
        "def B(var array1: [], var array2: [])\n"
        "  var array3: [] := @array2\n"
        "  var array4: [] := @array3\n"
        "  array4[0] := 3\n"
        "  array4[1] := 4\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}

TEST_CASE( "Algoly: CopyArray: local capture open local, assignment", "[algoly][open-array-value]" )
{
    const char code[] =
        "var ar1: [2] := [1, 2]\n"
        "var ar2: [2] := [5, 6]\n"
        "var ar3: [2] := [7, 8]\n"
        "def a\n"
        "  B( @ar1, @ar2 )\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1]\n"
        "end\n"
        "def B(var array1: [], var array2: [])\n"
        "  var array3: [] := @array2\n"
        "  var array4: [] := @ar3\n"
        "  array4 := @array3\n"
        "  array4[0] := 3\n"
        "  array4[1] := 4\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}

// TODO: it says closed local, but I don't see one
TEST_CASE( "Algoly: CopyArray: local capture closed local, init", "[algoly][open-array-value]" )
{
    const char code[] =
        "var ar1: [2] := [1, 2]\n"
        "var ar2: [2] := [5, 6]\n"
        "def a\n"
        "  B( @ar1, @ar2 )\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1]\n"
        "end\n"
        "def B(var array1: [], var array2: [])\n"
        "  var array3: [] := @ar2\n"
        "  var array4: [] := @array3\n"
        "  array4[0] := 3\n"
        "  array4[1] := 4\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}

// TODO: it says closed local, but I don't see one
TEST_CASE( "Algoly: CopyArray: local capture closed local, assignment", "[algoly][open-array-value]" )
{
    const char code[] =
        "var ar1: [2] := [1, 2]\n"
        "var ar2: [2] := [5, 6]\n"
        "def a\n"
        "  B( @ar1, @ar2 )\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1]\n"
        "end\n"
        "def B(var array1: [], var array2: [])\n"
        "  var array3: [] := @ar2\n"
        "  var array4: [] := @array1\n"
        "  array4 := @array3\n"
        "  array4[0] := 3\n"
        "  array4[1] := 4\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}

//----------------

TEST_CASE( "Algoly: local capture var array by const", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [1, 2]\n"
        "var ar1: [2] := []\n"
        "var ar2: [2] := [3, 4]\n"
        "def a\n"
        "  B(@AR1, @ar2)\n"
        "  ar1[0] + ar1[1] + AR1[0] + AR1[1]\n"
        "end\n"
        "def B(const array1: [], var array2: [])\n"
        "  array1 := @array2\n"
        "  ar1 := array1\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );// CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: local capture const array by var", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [1, 2]\n"
        "var ar1: [2] := []\n"
        "var ar2: [2] := [3, 4]\n"
        "def a\n"
        "  B(@AR1, @ar2)\n"
        "  ar1[0] + ar1[1] + AR1[0] + AR1[1]\n"
        "end\n"
        "def B(const array1: [], var array2: [])\n"
        "  array2 := @array1\n"
        "  ar1 := array1\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: copy const array by var", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [1, 2]\n"
        "var ar1: [2] := []\n"
        //"var ar2: [2] := [3, 4]\n"
        "def a\n"
        "  B(@AR1, @ar1)\n"
        "  ar1[0] + ar1[1] + AR1[0] + AR1[1]\n"
        "end\n"
        "def B(const array1: [], var array2: [])\n"
        "  array2 := array1\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 6 );// CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: copy const array by var param", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [1, 2]\n"
        "var ar1: [2] := []\n"
        "var ar2: [2] := [3, 4]\n"
        "def a\n"
        "  B(AR1, ar2)\n"
        "  ar1[0] + ar1[1] + AR1[0] + AR1[1]\n"
        "end\n"
        "def B(const array1: [], var array2: [])\n"
        "  array1 := array2\n"
        "  ar1 := array1\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: copy const array in array param by var local", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [[1, 2], [3, 4]]\n"
        "var ar1: [2] of [2] := [[5, 6], [7, 8]]\n"
        "def a\n"
        "  B(@AR1, @ar1)\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  AR1[0][0] + AR1[0][1] + AR1[1][0] + AR1[1][1]\n"
        "end\n"
        "def B(const array1: [] of [2], var array2: [] of [2])\n"
        "  array2[0] := array1[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 32 );// CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: copy var array in array param by const local", "[algoly][ptr-const]" )
{
    const char code[] =
        "const AR1 = [[1, 2], [3, 4]]\n"
        "var ar1: [2] of [2] := []\n"
        "def a\n"
        "  B(AR1, ar1)\n"
        "  ar1[0][0] + ar1[0][1] + ar1[1][0] + ar1[1][1] +\n"
        "  AR1[0][0] + AR1[0][1] + AR1[1][0] + AR1[1][1]\n"
        "end\n"
        "def B(const array1: [] of [2], var array2: [] of [2])\n"
        "  array1[0] := array2[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}


//----------------------------------------------------------------------------
// Local capture global
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: local var open array capture var global, assign other global to it", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [1, 2]\n"
        "var ar2 := [7, 8]\n"
        "var ar3 := [3, 4]\n"
        "def a\n"
        "  var array: [] := @ar2\n"
        "  array := ar3\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}

TEST_CASE( "Algoly: local const open array capture var global, assign it to other global", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [1, 2]\n"
        "var ar2 := [7, 8]\n"
        "var ar3 := [3, 4]\n"
        "def a\n"
        "  const array: [] = @ar3\n"
        "  ar2 := array\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 );
}


//----------------------------------------------------------------------------
// Local capture local capture global
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: local var open array capture local var open array that captured var global, assign other global to it", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [1, 2]\n"
        "var ar2 := [7, 8]\n"
        "var ar3 := [3, 4]\n"
        "def a\n"
        "  var array1: [] := @ar2\n"
        "  var array2: [] := @array1\n"
        "  array2 := ar3\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1] + countof(array2)\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 + 2 );
}

TEST_CASE( "Algoly: local const open array capture local var open array that captured var global, assign it to other global", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [1, 2]\n"
        "var ar2 := [7, 8]\n"
        "var ar3 := [3, 4]\n"
        "def a\n"
        "  var array1: [] := @ar3\n"
        "  const array2: [] = @array1\n"
        "  ar2 := array2\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1] + countof(array2)\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 + 2 );
}

TEST_CASE( "Algoly: local var open array capture local var open array that captured const global, assign other global to it", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [1, 2]\n"
        "const ar2 = [7, 8]\n"
        "var ar3 := [3, 4]\n"
        "def a\n"
        "  var array1: [] := @ar2\n"
        "  var array2: [] := @array1\n"
        "  array2 := ar3\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1] + countof(array2)\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: local const open array capture local var open array that captured const global, assign it to other global", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [1, 2]\n"
        "var ar2 := [7, 8]\n"
        "const ar3 = [3, 4]\n"
        "def a\n"
        "  var array1: [] := @ar3\n"
        "  const array2: [] = @array1\n"
        "  ar2 := array2\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1] + countof(array2)\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: local var open array capture local const open array that captured var global, assign other global to it", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [1, 2]\n"
        "var ar2 := [7, 8]\n"
        "var ar3 := [3, 4]\n"
        "def a\n"
        "  const array1: [] = @ar2\n"
        "  var array2: [] := @array1\n"
        "  array2 := ar3\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1] + countof(array2)\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: local const open array capture local const open array that captured var global, assign it to other global", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [1, 2]\n"
        "var ar2 := [7, 8]\n"
        "var ar3 := [3, 4]\n"
        "def a\n"
        "  const array1: [] = @ar3\n"
        "  const array2: [] = @array1\n"
        "  ar2 := array2\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1] + countof(array2)\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 + 2 );
}

TEST_CASE( "Algoly: local var open array capture local const open array that captured const global, assign other global to it", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [1, 2]\n"
        "const ar2 = [7, 8]\n"
        "var ar3 := [3, 4]\n"
        "def a\n"
        "  const array1: [] = @ar2\n"
        "  var array2: [] := @array1\n"
        "  array2 := ar3\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1] + countof(array2)\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: local const open array capture local const open array that captured const global, assign it to other global", "[algoly][ptr-const]" )
{
    const char code[] =
        "var ar1 := [1, 2]\n"
        "var ar2 := [7, 8]\n"
        "const ar3 = [3, 4]\n"
        "def a\n"
        "  const array1: [] = @ar3\n"
        "  const array2: [] = @array1\n"
        "  ar2 := array2\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1] + countof(array2)\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 + 2 );
}


//----------------------------------------------------------------------------
// Local capture local capture local
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: local var open array capture local var open array that captured var local, assign other local to it", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a\n"
        "  var ar1 := [1, 2]\n"
        "  var ar2 := [7, 8]\n"
        "  var ar3 := [3, 4]\n"
        "  var array1: [] := @ar2\n"
        "  var array2: [] := @array1\n"
        "  array2 := ar3\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1] + countof(array2)\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 + 2 );
}

TEST_CASE( "Algoly: local const open array capture local var open array that captured var local, assign it to other local", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a\n"
        "  var ar1 := [1, 2]\n"
        "  var ar2 := [7, 8]\n"
        "  var ar3 := [3, 4]\n"
        "  var array1: [] := @ar3\n"
        "  const array2: [] = @array1\n"
        "  ar2 := array2\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1] + countof(array2)\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 + 2 );
}

TEST_CASE( "Algoly: local var open array capture local var open array that captured const local, assign other local to it", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a\n"
        "  var ar1 := [1, 2]\n"
        "  const ar2 = [7, 8]\n"
        "  var ar3 := [3, 4]\n"
        "  var array1: [] := @ar2\n"
        "  var array2: [] := @array1\n"
        "  array2 := ar3\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1] + countof(array2)\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: local const open array capture local var open array that captured const local, assign it to other local", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a\n"
        "  var ar1 := [1, 2]\n"
        "  var ar2 := [7, 8]\n"
        "  const ar3 = [3, 4]\n"
        "  var array1: [] := @ar3\n"
        "  const array2: [] = @array1\n"
        "  ar2 := array2\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1] + countof(array2)\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: local var open array capture local const open array that captured var local, assign other local to it", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a\n"
        "  var ar1 := [1, 2]\n"
        "  var ar2 := [7, 8]\n"
        "  var ar3 := [3, 4]\n"
        "  const array1: [] = @ar2\n"
        "  var array2: [] := @array1\n"
        "  array2 := ar3\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1] + countof(array2)\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: local const open array capture local const open array that captured var local, assign it to other local", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a\n"
        "  var ar1 := [1, 2]\n"
        "  var ar2 := [7, 8]\n"
        "  var ar3 := [3, 4]\n"
        "  const array1: [] = @ar3\n"
        "  const array2: [] = @array1\n"
        "  ar2 := array2\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1] + countof(array2)\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 + 2 );
}

TEST_CASE( "Algoly: local var open array capture local const open array that captured const local, assign other local to it", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a\n"
        "  var ar1 := [1, 2]\n"
        "  const ar2 = [7, 8]\n"
        "  var ar3 := [3, 4]\n"
        "  const array1: [] = @ar2\n"
        "  var array2: [] := @array1\n"
        "  array2 := ar3\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1] + countof(array2)\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: local const open array capture local const open array that captured const local, assign it to other local", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a\n"
        "  var ar1 := [1, 2]\n"
        "  var ar2 := [7, 8]\n"
        "  const ar3 = [3, 4]\n"
        "  const array1: [] = @ar3\n"
        "  const array2: [] = @array1\n"
        "  ar2 := array2\n"
        "  ar1[0] + ar1[1] + ar2[0] + ar2[1] + countof(array2)\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10 + 2 );
}


// TODO: is assignment inside inside initializations tested? Shoud it be allowed?

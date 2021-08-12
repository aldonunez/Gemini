#include "pch.h"
#include "TestBase.h"
#include "../Gemini/Syntax.h"

using namespace Gemini;


//----------------------------------------------------------------------------
// Number limits
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: big number", "[algoly][limit]" )
{
    const char code[] =
        "def a 2147483647 end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, INT32_MAX, 0, 3 );
}

TEST_CASE( "Algoly: negative min number", "[algoly][limit]" )
{
    const char code[] =
        "def a -2147483648 end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, INT32_MIN, 0, 3 );
}

TEST_CASE( "Algoly: unary minus negative min number, literal", "[algoly][limit]" )
{
    const char code[] =
        "def a - -2147483648 end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, INT32_MAX, 0, 3 );
}

TEST_CASE( "Algoly: unary minus negative min number, var", "[algoly][limit]" )
{
    const char code[] =
        "var n := -2147483648\n"
        "def a\n"
        "  -n\n"
        "end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, INT32_MAX );
}

TEST_CASE( "Algoly: divide negative min number by -1, literal", "[algoly][limit]" )
{
    const char code[] =
        "def a -2147483648/-1 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, INT32_MAX );
}

TEST_CASE( "Algoly: divide negative min number by -1, var", "[algoly][limit]" )
{
    const char code[] =
        "var n := -2147483648\n"
        "def a\n"
        "  n/-1\n"
        "end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, INT32_MAX );
}

TEST_CASE( "Algoly: addition overflow +/+, var", "[algoly][limit]" )
{
    const char code[] =
        "var n := 2147483647\n"
        "def a\n"
        "  n+n\n"
        "end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, INT32_MAX );
}

TEST_CASE( "Algoly: multiplication overflow +/+, var", "[algoly][limit]" )
{
    const char code[] =
        "var n := 2147483647\n"
        "def a\n"
        "  n*n\n"
        "end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, INT32_MAX );
}

TEST_CASE( "Algoly: subtraction overflow -/+, var", "[algoly][limit]" )
{
    const char code[] =
        "var m := -2147483647\n"
        "var n := 2147483647\n"
        "def a\n"
        "  m-n\n"
        "end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, INT32_MIN );
}

TEST_CASE( "Algoly: number too big", "[algoly][limit][negative]" )
{
    const char code[] =
        "def a 2147483648 end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, CompilerErr::SEMANTICS );
}


//----------------------------------------------------------------------------
// Parameter limits
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: max params for main", "[algoly][limit]" )
{
    const char code[] =
        "def a(\n"
        "  a00, a01, a02, a03, a04, a05, a06, a07, a08, a09, a0A, a0B, a0C, a0D, a0E, a0F, \n"
        "  a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a1A, a1B, a1C, a1D, a1E, a1F, \n"
        "  a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a2A, a2B, a2C, a2D, a2E, a2F, \n"
        "  a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a3A, a3B, a3C, a3D, a3E, a3F, \n"
        "  a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a4A, a4B, a4C, a4D, a4E, a4F, \n"
        "  a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a5A, a5B, a5C, a5D, a5E, a5F, \n"
        "  a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a6A, a6B, a6C, a6D, a6E, a6F, \n"
        "  a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a7A, a7B, a7C, a7D, a7E  \n"
        "  )\n"
        "  a00 + a01 + a02 + a03 + a04 + a05 + a06 + a07 + a08 + a09 + a0A + a0B + a0C + a0D + a0E + a0F + \n"
        "  a10 + a11 + a12 + a13 + a14 + a15 + a16 + a17 + a18 + a19 + a1A + a1B + a1C + a1D + a1E + a1F + \n"
        "  a20 + a21 + a22 + a23 + a24 + a25 + a26 + a27 + a28 + a29 + a2A + a2B + a2C + a2D + a2E + a2F + \n"
        "  a30 + a31 + a32 + a33 + a34 + a35 + a36 + a37 + a38 + a39 + a3A + a3B + a3C + a3D + a3E + a3F + \n"
        "  a40 + a41 + a42 + a43 + a44 + a45 + a46 + a47 + a48 + a49 + a4A + a4B + a4C + a4D + a4E + a4F + \n"
        "  a50 + a51 + a52 + a53 + a54 + a55 + a56 + a57 + a58 + a59 + a5A + a5B + a5C + a5D + a5E + a5F + \n"
        "  a60 + a61 + a62 + a63 + a64 + a65 + a66 + a67 + a68 + a69 + a6A + a6B + a6C + a6D + a6E + a6F + \n"
        "  a70 + a71 + a72 + a73 + a74 + a75 + a76 + a77 + a78 + a79 + a7A + a7B + a7C + a7D + a7E  \n"
        "end\n"
        ;

    int args[ParamSizeMax];

    for ( unsigned long i = 0; i < ParamSizeMax; i++ )
    {
        args[i] = i;
    }

    std::initializer_list<int> initializer( &args[0], &args[ParamSizeMax] );

    TestCompileAndRunAlgoly( code, sizeof code, 8001, initializer, 131 );
}

TEST_CASE( "Algoly: max params for func", "[algoly][limit]" )
{
    const char code[] =
        "def a\n"
        "  B(\n"
        "    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,\n"
        "    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,\n"
        "    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,\n"
        "    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,\n"
        "    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,\n"
        "    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,\n"
        "    96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,\n"
        "    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126\n"
        "  )\n"
        "end\n"
        "def B(\n"
        "  a00, a01, a02, a03, a04, a05, a06, a07, a08, a09, a0A, a0B, a0C, a0D, a0E, a0F, \n"
        "  a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a1A, a1B, a1C, a1D, a1E, a1F, \n"
        "  a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a2A, a2B, a2C, a2D, a2E, a2F, \n"
        "  a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a3A, a3B, a3C, a3D, a3E, a3F, \n"
        "  a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a4A, a4B, a4C, a4D, a4E, a4F, \n"
        "  a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a5A, a5B, a5C, a5D, a5E, a5F, \n"
        "  a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a6A, a6B, a6C, a6D, a6E, a6F, \n"
        "  a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a7A, a7B, a7C, a7D, a7E  \n"
        "  )\n"
        "  a00 + a01 + a02 + a03 + a04 + a05 + a06 + a07 + a08 + a09 + a0A + a0B + a0C + a0D + a0E + a0F + \n"
        "  a10 + a11 + a12 + a13 + a14 + a15 + a16 + a17 + a18 + a19 + a1A + a1B + a1C + a1D + a1E + a1F + \n"
        "  a20 + a21 + a22 + a23 + a24 + a25 + a26 + a27 + a28 + a29 + a2A + a2B + a2C + a2D + a2E + a2F + \n"
        "  a30 + a31 + a32 + a33 + a34 + a35 + a36 + a37 + a38 + a39 + a3A + a3B + a3C + a3D + a3E + a3F + \n"
        "  a40 + a41 + a42 + a43 + a44 + a45 + a46 + a47 + a48 + a49 + a4A + a4B + a4C + a4D + a4E + a4F + \n"
        "  a50 + a51 + a52 + a53 + a54 + a55 + a56 + a57 + a58 + a59 + a5A + a5B + a5C + a5D + a5E + a5F + \n"
        "  a60 + a61 + a62 + a63 + a64 + a65 + a66 + a67 + a68 + a69 + a6A + a6B + a6C + a6D + a6E + a6F + \n"
        "  a70 + a71 + a72 + a73 + a74 + a75 + a76 + a77 + a78 + a79 + a7A + a7B + a7C + a7D + a7E  \n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 8001, {}, 131 + 2 );
}

TEST_CASE( "Algoly: too many params", "[algoly][limit][negative]" )
{
    const char code[] =
        "def a(\n"
        "  a00, a01, a02, a03, a04, a05, a06, a07, a08, a09, a0A, a0B, a0C, a0D, a0E, a0F, \n"
        "  a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a1A, a1B, a1C, a1D, a1E, a1F, \n"
        "  a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a2A, a2B, a2C, a2D, a2E, a2F, \n"
        "  a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a3A, a3B, a3C, a3D, a3E, a3F, \n"
        "  a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a4A, a4B, a4C, a4D, a4E, a4F, \n"
        "  a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a5A, a5B, a5C, a5D, a5E, a5F, \n"
        "  a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a6A, a6B, a6C, a6D, a6E, a6F, \n"
        "  a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a7A, a7B, a7C, a7D, a7E, a7F  \n"
        "  )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: max params with open array for func", "[algoly][limit]" )
{
    const char code[] =
        "var gar: [10] := [ 10, 20 ...+ ]\n"
        "def a\n"
        "  B(\n"
        "    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,\n"
        "    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,\n"
        "    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,\n"
        "    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,\n"
        "    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,\n"
        "    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,\n"
        "    96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,\n"
        "    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, gar\n"
        "  )\n"
        "end\n"
        "def B(\n"
        "  a00, a01, a02, a03, a04, a05, a06, a07, a08, a09, a0A, a0B, a0C, a0D, a0E, a0F, \n"
        "  a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a1A, a1B, a1C, a1D, a1E, a1F, \n"
        "  a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a2A, a2B, a2C, a2D, a2E, a2F, \n"
        "  a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a3A, a3B, a3C, a3D, a3E, a3F, \n"
        "  a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a4A, a4B, a4C, a4D, a4E, a4F, \n"
        "  a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a5A, a5B, a5C, a5D, a5E, a5F, \n"
        "  a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a6A, a6B, a6C, a6D, a6E, a6F, \n"
        "  a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a7A, a7B, a7C, var ar: []  \n"
        "  )\n"
        "  a00 + a01 + a02 + a03 + a04 + a05 + a06 + a07 + a08 + a09 + a0A + a0B + a0C + a0D + a0E + a0F + \n"
        "  a10 + a11 + a12 + a13 + a14 + a15 + a16 + a17 + a18 + a19 + a1A + a1B + a1C + a1D + a1E + a1F + \n"
        "  a20 + a21 + a22 + a23 + a24 + a25 + a26 + a27 + a28 + a29 + a2A + a2B + a2C + a2D + a2E + a2F + \n"
        "  a30 + a31 + a32 + a33 + a34 + a35 + a36 + a37 + a38 + a39 + a3A + a3B + a3C + a3D + a3E + a3F + \n"
        "  a40 + a41 + a42 + a43 + a44 + a45 + a46 + a47 + a48 + a49 + a4A + a4B + a4C + a4D + a4E + a4F + \n"
        "  a50 + a51 + a52 + a53 + a54 + a55 + a56 + a57 + a58 + a59 + a5A + a5B + a5C + a5D + a5E + a5F + \n"
        "  a60 + a61 + a62 + a63 + a64 + a65 + a66 + a67 + a68 + a69 + a6A + a6B + a6C + a6D + a6E + a6F + \n"
        "  a70 + a71 + a72 + a73 + a74 + a75 + a76 + a77 + a78 + a79 + a7A + a7B + a7C + countof(ar) + ar[9]  \n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 7750 + 10 + 100, {}, 131 + 3 );
}

TEST_CASE( "Algoly: too many params with open array", "[algoly][limit][negative]" )
{
    const char code[] =
        "def a(\n"
        "  a00, a01, a02, a03, a04, a05, a06, a07, a08, a09, a0A, a0B, a0C, a0D, a0E, a0F, \n"
        "  a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a1A, a1B, a1C, a1D, a1E, a1F, \n"
        "  a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a2A, a2B, a2C, a2D, a2E, a2F, \n"
        "  a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a3A, a3B, a3C, a3D, a3E, a3F, \n"
        "  a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a4A, a4B, a4C, a4D, a4E, a4F, \n"
        "  a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a5A, a5B, a5C, a5D, a5E, a5F, \n"
        "  a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a6A, a6B, a6C, a6D, a6E, a6F, \n"
        "  a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a7A, a7B, a7C, a7D, a7E, var ar: []  \n"
        "  )\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, CompilerErr::SEMANTICS );
}


//----------------------------------------------------------------------------
// Local limits
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: max locals", "[algoly][limit]" )
{
    const char code[] =
        "def a\n"
        "  var a := 10,\n"
        "      b: [253] := [ 1, 2 ...+ ],\n"
        "      c := 20\n"
        "  B(b) + a + c\n"
        "end\n"
        "def B( var ar: [253] )\n"
        "  var total := 0\n"
        "  for i := 0 below 253 do\n"
        "    total := ar[i] + total\n"
        "  end\n"
        "  total\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 32131 + 30, {}, 257 + 7 );
}

TEST_CASE( "Algoly: too many locals", "[algoly][limit][negative]" )
{
    const char code[] =
        "def a\n"
        "  var a := 10,\n"
        "      b: [254] := [ 1, 2 ...+ ]\n"
        "      c := 20\n"
        "  B(b) + a + c\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, CompilerErr::SEMANTICS );
}


//----------------------------------------------------------------------------
// Global limits
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: max globals", "[algoly][limit]" )
{
    const char code[] =
        "var d := 10,\n"
        "    b: [65533] := [ 1, 2 ...+ ],\n"
        "    c := 20\n"
        "def a\n"
        "  B(b) + d + c\n"
        "end\n"
        "def B( var ar: [65533] )\n"
        "  var total := 0\n"
        "  for i := 0 below countof(ar) do\n"
        "    total := ar[i] + total\n"
        "  end\n"
        "  total\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 2147319811 + 30, {}, 2 + 7 );
}

TEST_CASE( "Algoly: too many globals", "[algoly][limit][negative]" )
{
    const char code[] =
        "var d := 10,\n"
        "    b: [65534] := [ 1, 2 ...+ ],\n"
        "    c := 20\n"
        "def a end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, CompilerErr::SEMANTICS );
}


//----------------------------------------------------------------------------
// Duplicate symbols
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: duplicate param", "[algoly][negative]" )
{
    const char code[] =
        "def F(p, p) end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: duplicate local, same block", "[algoly][negative]" )
{
    const char code[] =
        "def F var local := 3, local := 5 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: duplicate local const, same block", "[algoly][negative]" )
{
    const char code[] =
        "def F const local = 3, local = 5 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: duplicate local, different block OK", "[algoly]" )
{
    const char code[] =
        "var keep := 0\n"
        "def a\n"
        "  var local := 3\n"
        "  keep := local\n"
        "  var local := 5\n"
        "  local + keep\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 8 );
}

TEST_CASE( "Algoly: duplicate local const, different block OK", "[algoly]" )
{
    const char code[] =
        "var keep := 0\n"
        "def a\n"
        "  const local = 3\n"
        "  keep := local\n"
        "  const local = 5\n"
        "  local + keep\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 8 );
}

TEST_CASE( "Algoly: duplicate global", "[algoly][negative]" )
{
    const char code[] =
        "var g := 3, g := 5\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: duplicate global const", "[algoly][negative]" )
{
    const char code[] =
        "const g = 3, g = 5\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: duplicate type", "[algoly][negative]" )
{
    const char code[] =
        "type T = int, T = &proc\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: duplicate global, different block", "[algoly][negative]" )
{
    const char code[] =
        "var g := 3\n"
        "def a end\n"
        "var g := 5\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: duplicate global const, different block", "[algoly][negative]" )
{
    const char code[] =
        "const g = 3\n"
        "def a end\n"
        "const g = 5\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: duplicate type, different block", "[algoly][negative]" )
{
    const char code[] =
        "type T = int\n"
        "def a end\n"
        "type T = &proc\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: duplicate record field", "[algoly][negative]" )
{
    const char code[] =
        "type R = record x, x end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: duplicate enum member", "[algoly][negative]" )
{
    const char code[] =
        "type E = enum x, x end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, CompilerErr::SEMANTICS );
}

TEST_CASE( "Algoly: duplicate func", "[algoly][negative]" )
{
    const char code[] =
        "def a end\n"
        "def a(x) 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, CompilerErr::SEMANTICS );
}

// TODO: add duplicate module name, duplicate module import, duplicate module import as


// TODO: all unary and binary operators
// TODO: const, const folding

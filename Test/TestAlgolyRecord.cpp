#include "pch.h"
#include "TestBase.h"


#if 0
TEST_CASE( "Algoly: global record empty", "[algoly][record]" )
{
    const char code[] =
        "type R = record end\n"
        "var r: R := { }\n"
        "def a\n"
        "end\n"
        ;
    ;

    TestCompileAndRunAlgoly( code, 0, 0, 3 );
}

TEST_CASE( "Algoly: local record empty", "[algoly][record]" )
{
    const char code[] =
        "type R = record end\n"
        "def a\n"
        "  var r: R := { }\n"
        "end\n"
        ;
    ;

    TestCompileAndRunAlgoly( code, 0, 0, 3 );
}
#endif

TEST_CASE( "Algoly: read global record 1 int", "[algoly][record]" )
{
    const char code[] =
        "type R = record x end\n"
        "var r: R := { x:3 }\n"
        "def a\n"
        "  r.x\n"
        "end\n"
        ;
    ;

    TestCompileAndRunAlgoly( code, 3, 0, 3 );
}

TEST_CASE( "Algoly: read local record 1 int", "[algoly][record]" )
{
    const char code[] =
        "type R = record x end\n"
        "def a\n"
        "  var r: R := { x:3 }\n"
        "  r.x\n"
        "end\n"
        ;
    ;

    TestCompileAndRunAlgoly( code, 3, 0, 4 );
}

TEST_CASE( "Algoly: write global record 1 int", "[algoly][record]" )
{
    const char code[] =
        "type R = record x end\n"
        "var r: R := { x:3 }\n"
        "def a\n"
        "  r.x := 9\n"
        "  r.x\n"
        "end\n"
        ;
    ;

    TestCompileAndRunAlgoly( code, 9, 0, 3 );
}

TEST_CASE( "Algoly: write local record 1 int", "[algoly][record]" )
{
    const char code[] =
        "type R = record x end\n"
        "def a\n"
        "  var r: R := { x:3 }\n"
        "  r.x := 9\n"
        "  r.x\n"
        "end\n"
        ;
    ;

    TestCompileAndRunAlgoly( code, 9, 0, 4 );
}

TEST_CASE( "Algoly: read global record 2 int", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y: int end\n"
        "var r: R := { x:3, y: 4 }\n"
        "def a\n"
        "  r.x + r.y\n"
        "end\n"
        ;
    ;

    TestCompileAndRunAlgoly( code, 7, 0, 4 );
}

TEST_CASE( "Algoly: read local record 2 int", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y: int end\n"
        "def a\n"
        "  var r: R := { x:3, y: 4 }\n"
        "  r.x + r.y\n"
        "end\n"
        ;
    ;

    TestCompileAndRunAlgoly( code, 7, 0, 6 );
}

TEST_CASE( "Algoly: write global record 2 int", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y: int end\n"
        "var r: R := { x:3, y: 4 }\n"
        "def a\n"
        "  r.x := 10\n"
        "  r.y := 20\n"
        "  r.x + r.y\n"
        "end\n"
        ;
    ;

    TestCompileAndRunAlgoly( code, 30, 0, 4 );
}

TEST_CASE( "Algoly: write local record 2 int", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y: int end\n"
        "def a\n"
        "  var r: R := { x:3, y: 4 }\n"
        "  r.x := 10\n"
        "  r.y := 20\n"
        "  r.x + r.y\n"
        "end\n"
        ;
    ;

    TestCompileAndRunAlgoly( code, 30, 0, 6 );
}

TEST_CASE( "Algoly: read global record 2 int default init", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y: int end\n"
        "var r: R := { }\n"
        "def a\n"
        "  r.x + r.y\n"
        "end\n"
        ;
    ;

    TestCompileAndRunAlgoly( code, 0, 0, 4 );
}

TEST_CASE( "Algoly: read local record 2 int default init", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y: int end\n"
        "def a\n"
        "  var r: R := { }\n"
        "  r.x + r.y\n"
        "end\n"
        ;
    ;

    TestCompileAndRunAlgoly( code, 0, 0, 6 );
}

//-----------------------

TEST_CASE( "Algoly: read global record int, &proc, int", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var r: R := { x:3, f: &B, y: 4 }\n"
        "def a\n"
        "  r.x + r.y + (r.f)()\n"
        "end\n"
        "def B 100 end\n"
        ;

    TestCompileAndRunAlgoly( code, 107, 0, 4 );
}

TEST_CASE( "Algoly: read local record int, &proc, int", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "def a\n"
        "  var r: R := { x:3, f: &B, y: 4 }\n"
        "  r.x + r.y + (r.f)()\n"
        "end\n"
        "def B 100 end\n"
        ;

    TestCompileAndRunAlgoly( code, 107, 0, 7 );
}

TEST_CASE( "Algoly: write global record int, &proc, int", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var r: R := { x:3, f: &B, y: 4 }\n"
        "def a\n"
        "  r.x := 10\n"
        "  r.y := 20\n"
        "  r.x + r.y + (r.f)()\n"
        "end\n"
        "def B 100 end\n"
        ;

    TestCompileAndRunAlgoly( code, 130, 0, 4 );
}

TEST_CASE( "Algoly: write local record int, &proc, int", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "def a\n"
        "  var r: R := { x:3, f: &B, y: 4 }\n"
        "  r.x := 10\n"
        "  r.y := 20\n"
        "  r.x + r.y + (r.f)()\n"
        "end\n"
        "def B 100 end\n"
        ;

    TestCompileAndRunAlgoly( code, 130, 0, 7 );
}

TEST_CASE( "Algoly: read global record int, &proc, int default init", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var r: R := { f: &B }\n"
        "def a\n"
        "  r.x + r.y + (r.f)()\n"
        "end\n"
        "def B 100 end\n"
        ;

    TestCompileAndRunAlgoly( code, 100, 0, 4 );
}

TEST_CASE( "Algoly: read local record int, &proc, int default init", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "def a\n"
        "  var r: R := { f: &B }\n"
        "  r.x + r.y + (r.f)()\n"
        "end\n"
        "def B 100 end\n"
        ;

    TestCompileAndRunAlgoly( code, 100, 0, 7 );
}

//---

TEST_CASE( "Algoly: read record of record", "[algoly][record]" )
{
    const char code[] =
        // Output: 6 or 12
        "type R = record x, y end\n"
        "type Q = record a, r: R end\n"
        "var q: Q := { a: 1, r: { x: 2, y: 3 } }\n"
        "def a\n"
        "  q.a + q.r.x + q.r.y\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 6, 0, 4 );
}

TEST_CASE( "Algoly: write record of record", "[algoly][record]" )
{
    const char code[] =
        // Output: 6 or 12
        "type R = record x, y end\n"
        "type Q = record a, r: R end\n"
        "var q: Q := { a: 1, r: { x: 2, y: 3 } }\n"
        "def a\n"
        "  q.a := 2; q.r.x := 4; q.r.y := 6\n"
        "  q.a + q.r.x + q.r.y\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 12, 0, 4 );
}


//----------------------------------------------------------------------------
//  array of record
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: read global array of record (int, &proc, int) repeat, const indexes", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var r: [2] of R := [{ x:3, f: &B, y: 4 }...]\n"
        "def a\n"
        "  r[0].x + r[0].y + (r[0].f)() +\n"
        "  r[1].x + r[1].y + (r[1].f)()\n"
        "end\n"
        "def B 100 end\n"
        ;

    TestCompileAndRunAlgoly( code, 214, 0, 4 );
}

TEST_CASE( "Algoly: read local array of record (int, &proc, int) repeat, const indexes", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "def a\n"
        "  var r: [2] of R := [{ x:3, f: &B, y: 4 }...]\n"
        "  r[0].x + r[0].y + (r[0].f)() +\n"
        "  r[1].x + r[1].y + (r[1].f)()\n"
        "end\n"
        "def B 100 end\n"
        ;

    TestCompileAndRunAlgoly( code, 214, 0, 10 );
}

TEST_CASE( "Algoly: read global array of record (int, int), var indexes", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y: int end\n"
        "var r: [2] of R := [{ x:3, y: 4 }, { x: 5, y : 6 }]\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  r[zero].x + r[zero].y +\n"
        "  r[one].x + r[one].y\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 18, 0, 5 );
}

TEST_CASE( "Algoly: read local array of record (int, int), var indexes", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y: int end\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  var r: [2] of R := [{ x:3, y: 4 }, { x: 5, y : 6 }]\n"
        "  r[zero].x + r[zero].y +\n"
        "  r[one].x + r[one].y\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 18, 0, 9 );
}


//----------------------------------------------------------------------------
//  record of array
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: read global record of array", "[algoly][record]" )
{
    const char code[] =
        // Output: 10
        "type R = record a: [2], b: [2] end\n"
        "var r: R := { a: [1, 2], b: [3, 4] }\n"
        "def a\n"
        "  r.a[0] + r.a[1] + r.b[0] + r.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10, 0, 4 );
}

TEST_CASE( "Algoly: read local record of array", "[algoly][record]" )
{
    const char code[] =
        // Output: 10
        "type R = record a: [2], b: [2] end\n"
        "def a\n"
        "  var r: R := { a: [1, 2], b: [3, 4] }\n"
        "  r.a[0] + r.a[1] + r.b[0] + r.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10, 0, 8 );
}

TEST_CASE( "Algoly: read global record of array, var indexes", "[algoly][record]" )
{
    const char code[] =
        // Output: 10
        "type R = record a: [2], b: [2] end\n"
        "var r: R := { a: [1, 2], b: [3, 4] }\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  r.a[zero] + r.a[one] + r.b[zero] + r.b[one]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10, 0, 5 );
}

TEST_CASE( "Algoly: read local record of array, var indexes", "[algoly][record]" )
{
    const char code[] =
        // Output: 10
        "type R = record a: [2], b: [2] end\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  var r: R := { a: [1, 2], b: [3, 4] }\n"
        "  r.a[zero] + r.a[one] + r.b[zero] + r.b[one]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 10, 0, 9 );
}


//----------------------------------------------------------------------------
//  array of array of record
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: global array of array of record, const indexes", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y end\n"
        "var ar: [2] of [2] of R := [\n"
        "    [{x:1, y:2}, {x:3, y:4}], [{x:5, y:6}, {x:7, y:8}]\n"
        "  ]\n"
        "def a\n"
        "  ar[1][1].y\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 8, 0, 3 );
}

TEST_CASE( "Algoly: local array of array of record, const indexes", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y end\n"
        "def a\n"
        "  var ar: [2] of [2] of R := [\n"
        "    [{x:1, y:2}, {x:3, y:4}], [{x:5, y:6}, {x:7, y:8}]\n"
        "  ]\n"
        "  ar[1][1].y\n"
        "end\n"
        ;
    ;

    TestCompileAndRunAlgoly( code, 8, 0, 11 );
}

TEST_CASE( "Algoly: global array of array of record, var indexes", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y end\n"
        "var ar: [2] of [2] of R := [\n"
        "    [{x:1, y:2}, {x:3, y:4}], [{x:5, y:6}, {x:7, y:8}]\n"
        "  ]\n"
        "var i := 1\n"
        "def a\n"
        "  ar[i][i].y\n"
        "end\n"
        ;
    ;

    TestCompileAndRunAlgoly( code, 8, 0, 4 );
}

TEST_CASE( "Algoly: local array of array of record, var indexes", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y end\n"
        "var i := 1\n"
        "def a\n"
        "  var ar: [2] of [2] of R := [\n"
        "    [{x:1, y:2}, {x:3, y:4}], [{x:5, y:6}, {x:7, y:8}]\n"
        "  ]\n"
        "  ar[i][i].y\n"
        "end\n"
        ;
    ;

    TestCompileAndRunAlgoly( code, 8, 0, 12 );
}


//----------------------------------------------------------------------------
//  Other nested records and arrays
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: record of array of record, const indexes", "[algoly][record]" )
{
    const char code[] =
        // Output: 36
        "type R = record a: [2] of S, b: [2] of S end\n"
        "type S = record x, y end\n"
        "var r: R :=\n"
        "    {\n"
        "      a: [ { x: 1, y: 2 }, { x: 3, y: 4 } ],\n"
        "      b: [ { x: 5, y: 6 }, { x: 7, y: 8 } ],\n"
        "    }\n"
        "def a\n"
        "  r.a[0].x + r.a[0].y + r.a[1].x + r.a[1].y +\n"
        "  r.b[0].x + r.b[0].y + r.b[1].x + r.b[1].y\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 36, 0, 4 );
}

TEST_CASE( "Algoly: record of array of record, var indexes", "[algoly][record]" )
{
    const char code[] =
        // Output: 36
        "type R = record a: [2] of S, b: [2] of S end\n"
        "type S = record x, y end\n"
        "var r: R :=\n"
        "    {\n"
        "      a: [ { x: 1, y: 2 }, { x: 3, y: 4 } ],\n"
        "      b: [ { x: 5, y: 6 }, { x: 7, y: 8 } ],\n"
        "    }\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  r.a[zero].x + r.a[zero].y + r.a[one].x + r.a[one].y +\n"
        "  r.b[zero].x + r.b[zero].y + r.b[one].x + r.b[one].y\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 36, 0, 5 );
}

TEST_CASE( "Algoly: array of record of array, const indexes", "[algoly][record]" )
{
    const char code[] =
        // Output: 36
        "type R = record a: [2], b: [2] end\n"
        "var r: [2] of R := \n"
        "    [\n"
        "      { a: [1, 2], b: [3, 4] },\n"
        "      { a: [5, 6], b: [7, 8] }\n"
        "    ]\n"
        "def a\n"
        "  r[0].a[0] + r[0].a[1] + r[0].b[0] + r[0].b[1] +\n"
        "  r[1].a[0] + r[1].a[1] + r[1].b[0] + r[1].b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 36, 0, 4 );
}

TEST_CASE( "Algoly: array of record of array, var indexes", "[algoly][record]" )
{
    const char code[] =
        // Output: 36
        "type R = record a: [2], b: [2] end\n"
        "var r: [2] of R := \n"
        "    [\n"
        "      { a: [1, 2], b: [3, 4] },\n"
        "      { a: [5, 6], b: [7, 8] }\n"
        "    ]\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  r[zero].a[zero] + r[zero].a[one] + r[zero].b[zero] + r[zero].b[one] +\n"
        "  r[one].a[zero]  + r[one].a[one]  + r[one].b[zero]  + r[one].b[one]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 36, 0, 5 );
}

TEST_CASE( "Algoly: array of record of record, const indexes", "[algoly][record]" )
{
    const char code[] =
        // Output: 36
        "type R = record a: S, b: S end\n"
        "type S = record x, y end\n"
        "var r: [2] of R := \n"
        "    [\n"
        "      { a: { x:1, y:2 }, b: { x:3, y:4 } },\n"
        "      { a: { x:5, y:6 }, b: { x:7, y:8 } }\n"
        "    ]\n"
        "def a\n"
        "  r[0].a.x + r[0].a.y + r[0].b.x + r[0].b.y +\n"
        "  r[1].a.x + r[1].a.y + r[1].b.x + r[1].b.y\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 36, 0, 4 );
}

TEST_CASE( "Algoly: array of record of record, var indexes", "[algoly][record]" )
{
    const char code[] =
        // Output: 36
        "type R = record a: S, b: S end\n"
        "type S = record x, y end\n"
        "var r: [2] of R := \n"
        "    [\n"
        "      { a: { x:1, y:2 }, b: { x:3, y:4 } },\n"
        "      { a: { x:5, y:6 }, b: { x:7, y:8 } }\n"
        "    ]\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  r[zero].a.x + r[zero].a.y + r[zero].b.x + r[zero].b.y +\n"
        "  r[one].a.x  + r[one].a.y  + r[one].b.x  + r[one].b.y\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 36, 0, 5 );
}

TEST_CASE( "Algoly: record of record of array, const indexes", "[algoly][record]" )
{
    const char code[] =
        // Output: 36
        "type Q = record x: R, y: R end\n"
        "type R = record a: [2], b: [2] end\n"
        "var q: Q := \n"
        "    {\n"
        "      x: { a: [1, 2], b: [3, 4] },\n"
        "      y: { a: [5, 6], b: [7, 8] }\n"
        "    }\n"
        "def a\n"
        "  q.x.a[0] + q.x.a[1] + q.x.b[0] + q.x.b[1] +\n"
        "  q.y.a[0] + q.y.a[1] + q.y.b[0] + q.y.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 36, 0, 4 );
}

TEST_CASE( "Algoly: record of record of array, var indexes", "[algoly][record]" )
{
    const char code[] =
        // Output: 36
        "type Q = record x: R, y: R end\n"
        "type R = record a: [2], b: [2] end\n"
        "var q: Q := \n"
        "    {\n"
        "      x: { a: [1, 2], b: [3, 4] },\n"
        "      y: { a: [5, 6], b: [7, 8] }\n"
        "    }\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  q.x.a[zero] + q.x.a[one] + q.x.b[zero] + q.x.b[one] +\n"
        "  q.y.a[zero] + q.y.a[one] + q.y.b[zero] + q.y.b[one]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 36, 0, 5 );
}

TEST_CASE( "Algoly: array of record of record of array, loop indexing", "[algoly][record]" )
{
    const char code[] =
        // Output: 35
        "type R = record x: [2], y: [2] end\n"
        "type Q = record a, r: R end\n"
        "var total := 0\n"
        "def a\n"
        "  var q: [2] of Q := \n"
        "    [\n"
        "      { a: 1, r: { x: [2, 3], y: [4, 5] } },\n"
        "      { a: 2, r: { x: [3, 4], y: [5, 6] } }\n"
        "    ]\n"
        "  for i := 0 to 1 do\n"
        "    total := total + q[i].a\n"
        "    for j := 0 to 1 do\n"
        "      total := q[i].r.x[j] + q[i].r.y[j] + total\n"
        "    end\n"
        "  end\n"
        "  total\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 35, 0, 17 );
}

TEST_CASE( "Algoly: read global array of array of repeating record with fptr and default init int, loop indexing", "[algoly][record]" )
{
    const char code[] =
        // Output: 12
        "type Q = record a, b: &proc end\n"
        "var ar: [2] of [2] of Q := [[{b: &B}...]...]\n"
        "var total := 0\n"
        "def a \n"
        "  for i := 0 to 1 do\n"
        "    for j := 0 to 1 do\n"
        "      total := total + (ar[i][j].b)() + ar[i][j].a\n"
        "    end\n"
        "  end\n"
        "  total\n"
        "end\n"
        "def B 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 12, 0, 7 );
}

TEST_CASE( "Algoly: read local array of array of repeating record with fptr and default init int, loop indexing", "[algoly][record]" )
{
    const char code[] =
        // Output: 12
        "type Q = record a, b: &proc end\n"
        "var total := 0\n"
        "def a \n"
        "  var ar: [2] of [2] of Q := [[{b: &B}...]...]\n"
        "  for i := 0 to 1 do\n"
        "    for j := 0 to 1 do\n"
        "      total := total + (ar[i][j].b)() + ar[i][j].a\n"
        "    end\n"
        "  end\n"
        "  total\n"
        "end\n"
        "def B 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, 12, 0, 7+8 );
}


//----------------------------------------------------------------------------
//  Assign record
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: assign record, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var r2: R := { x: 3, f: &B, y: 4 }\n"
        "var r1: R := { x: 1, f: &C, y: 2 }\n"
        "def a\n"
        "  r1 := r2\n"
        "  r1.x + r1.y + (r1.f)() + \n"
        "  r2.x + r2.y + (r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        ;

    TestCompileAndRunAlgoly( code, 214, 0 );
}

TEST_CASE( "Algoly: assign record, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var r2: R := { x: 3, f: &B, y: 4 }\n"
        "def a\n"
        "  var r1: R := { x: 1, f: &C, y: 2 }\n"
        "  r1 := r2\n"
        "  r1.x + r1.y + (r1.f)() + \n"
        "  r2.x + r2.y + (r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        ;

    TestCompileAndRunAlgoly( code, 214, 0 );
}

TEST_CASE( "Algoly: assign record, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "def a\n"
        "  var r2: R := { x: 3, f: &B, y: 4 }\n"
        "  var r1: R := { x: 1, f: &C, y: 2 }\n"
        "  r1 := r2\n"
        "  r1.x + r1.y + (r1.f)() + \n"
        "  r2.x + r2.y + (r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        ;

    TestCompileAndRunAlgoly( code, 214, 0 );
}


//----------------------------------------------------------------------------
//  Init record with record
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: init record, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var r2: R := { x: 3, f: &B, y: 4 }\n"
        "var r1: R := r2\n"
        "def a\n"
        "  r1.x + r1.y + (r1.f)() + \n"
        "  r2.x + r2.y + (r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        ;

    TestCompileAndRunAlgoly( code, 214, 0 );
}

TEST_CASE( "Algoly: init record, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var r2: R := { x: 3, f: &B, y: 4 }\n"
        "def a\n"
        "  var r1: R := r2\n"
        "  r1.x + r1.y + (r1.f)() + \n"
        "  r2.x + r2.y + (r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        ;

    TestCompileAndRunAlgoly( code, 214, 0 );
}

TEST_CASE( "Algoly: init record, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "def a\n"
        "  var r2: R := { x: 3, f: &B, y: 4 }\n"
        "  var r1: R := r2\n"
        "  r1.x + r1.y + (r1.f)() + \n"
        "  r2.x + r2.y + (r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        ;

    TestCompileAndRunAlgoly( code, 214, 0 );
}


//----------------------------------------------------------------------------
//  Assign record inside record
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: assign record in record to record, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "type Q = record r1: R, r2: R end\n"
        "var q: Q := { r1: { x: 1, f: &C, y: 2 }, r2: { x: 3, f: &B, y: 4 } }\n"
        "var r: R := { x: 5, f: &D, y: 6 }\n"
        "def a\n"
        "  r := q.r2\n"
        "  r.x + r.y + (r.f)() + \n"
        "  q.r1.x + q.r1.y + (q.r1.f)() + q.r2.x + q.r2.y + (q.r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: assign record in record to record, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "type Q = record r1: R, r2: R end\n"
        "var q: Q := { r1: { x: 1, f: &C, y: 2 }, r2: { x: 3, f: &B, y: 4 } }\n"
        "def a\n"
        "  var r: R := { x: 5, f: &D, y: 6 }\n"
        "  r := q.r2\n"
        "  r.x + r.y + (r.f)() + \n"
        "  q.r1.x + q.r1.y + (q.r1.f)() + q.r2.x + q.r2.y + (q.r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: assign record in record to record, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "type Q = record r1: R, r2: R end\n"
        "def a\n"
        "  var q: Q := { r1: { x: 1, f: &C, y: 2 }, r2: { x: 3, f: &B, y: 4 } }\n"
        "  var r: R := { x: 5, f: &D, y: 6 }\n"
        "  r := q.r2\n"
        "  r.x + r.y + (r.f)() + \n"
        "  q.r1.x + q.r1.y + (q.r1.f)() + q.r2.x + q.r2.y + (q.r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: assign record to record in record, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "type Q = record r1: R, r2: R end\n"
        "var r: R := { x: 3, f: &B, y: 4 }\n"
        "var q: Q := { r1: { x: 1, f: &C, y: 2 }, r2: { x: 5, f: &D, y: 6 } }\n"
        "def a\n"
        "  q.r2 := r\n"
        "  r.x + r.y + (r.f)() + \n"
        "  q.r1.x + q.r1.y + (q.r1.f)() + q.r2.x + q.r2.y + (q.r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: assign record to record in record, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "type Q = record r1: R, r2: R end\n"
        "var r: R := { x: 3, f: &B, y: 4 }\n"
        "def a\n"
        "  var q: Q := { r1: { x: 1, f: &C, y: 2 }, r2: { x: 5, f: &D, y: 6 } }\n"
        "  q.r2 := r\n"
        "  r.x + r.y + (r.f)() + \n"
        "  q.r1.x + q.r1.y + (q.r1.f)() + q.r2.x + q.r2.y + (q.r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: assign record to record in record, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "type Q = record r1: R, r2: R end\n"
        "def a\n"
        "  var r: R := { x: 3, f: &B, y: 4 }\n"
        "  var q: Q := { r1: { x: 1, f: &C, y: 2 }, r2: { x: 5, f: &D, y: 6 } }\n"
        "  q.r2 := r\n"
        "  r.x + r.y + (r.f)() + \n"
        "  q.r1.x + q.r1.y + (q.r1.f)() + q.r2.x + q.r2.y + (q.r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: assign record in record to record in record, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "type Q = record r1: R, r2: R end\n"
        "var q2: Q := { r1: { x: 1, f: &C, y: 2 }, r2: { x: 3, f: &B, y: 4 } }\n"
        "var q1: Q := { r1: { x: 5, f: &D, y: 6 }, r2: { x: 7, f: &D, y: 8 } }\n"
        "def a\n"
        "  q1.r2 := q2.r2\n"
        "  q1.r1.x + q1.r1.y + (q1.r1.f)() + q1.r2.x + q1.r2.y + (q1.r2.f)() + \n"
        "  q2.r1.x + q2.r1.y + (q2.r1.f)() + q2.r2.x + q2.r2.y + (q2.r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 239, 0 );
}

TEST_CASE( "Algoly: assign record in record to record in record, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "type Q = record r1: R, r2: R end\n"
        "var q2: Q := { r1: { x: 1, f: &C, y: 2 }, r2: { x: 3, f: &B, y: 4 } }\n"
        "def a\n"
        "  var q1: Q := { r1: { x: 5, f: &D, y: 6 }, r2: { x: 7, f: &D, y: 8 } }\n"
        "  q1.r2 := q2.r2\n"
        "  q1.r1.x + q1.r1.y + (q1.r1.f)() + q1.r2.x + q1.r2.y + (q1.r2.f)() + \n"
        "  q2.r1.x + q2.r1.y + (q2.r1.f)() + q2.r2.x + q2.r2.y + (q2.r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 239, 0 );
}

TEST_CASE( "Algoly: assign record in record to record in record, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "type Q = record r1: R, r2: R end\n"
        "def a\n"
        "  var q2: Q := { r1: { x: 1, f: &C, y: 2 }, r2: { x: 3, f: &B, y: 4 } }\n"
        "  var q1: Q := { r1: { x: 5, f: &D, y: 6 }, r2: { x: 7, f: &D, y: 8 } }\n"
        "  q1.r2 := q2.r2\n"
        "  q1.r1.x + q1.r1.y + (q1.r1.f)() + q1.r2.x + q1.r2.y + (q1.r2.f)() + \n"
        "  q2.r1.x + q2.r1.y + (q2.r1.f)() + q2.r2.x + q2.r2.y + (q2.r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 239, 0 );
}


//----------------------------------------------------------------------------
//  Init record inside record
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: init record in record to record, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "type Q = record r1: R, r2: R end\n"
        "var q: Q := { r1: { x: 1, f: &C, y: 2 }, r2: { x: 3, f: &B, y: 4 } }\n"
        "var r: R := q.r2\n"
        "def a\n"
        "  r.x + r.y + (r.f)() + \n"
        "  q.r1.x + q.r1.y + (q.r1.f)() + q.r2.x + q.r2.y + (q.r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: init record in record to record, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "type Q = record r1: R, r2: R end\n"
        "var q: Q := { r1: { x: 1, f: &C, y: 2 }, r2: { x: 3, f: &B, y: 4 } }\n"
        "def a\n"
        "  var r: R := q.r2\n"
        "  r := q.r2\n"
        "  r.x + r.y + (r.f)() + \n"
        "  q.r1.x + q.r1.y + (q.r1.f)() + q.r2.x + q.r2.y + (q.r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: init record in record to record, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "type Q = record r1: R, r2: R end\n"
        "def a\n"
        "  var q: Q := { r1: { x: 1, f: &C, y: 2 }, r2: { x: 3, f: &B, y: 4 } }\n"
        "  var r: R := q.r2\n"
        "  r := q.r2\n"
        "  r.x + r.y + (r.f)() + \n"
        "  q.r1.x + q.r1.y + (q.r1.f)() + q.r2.x + q.r2.y + (q.r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: init record to record in record, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "type Q = record r1: R, r2: R end\n"
        "var r: R := { x: 3, f: &B, y: 4 }\n"
        "var q: Q := { r1: { x: 1, f: &C, y: 2 }, r2: r }\n"
        "def a\n"
        "  r.x + r.y + (r.f)() + \n"
        "  q.r1.x + q.r1.y + (q.r1.f)() + q.r2.x + q.r2.y + (q.r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: init record to record in record, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "type Q = record r1: R, r2: R end\n"
        "var r: R := { x: 3, f: &B, y: 4 }\n"
        "def a\n"
        "  var q: Q := { r1: { x: 1, f: &C, y: 2 }, r2: r }\n"
        "  r.x + r.y + (r.f)() + \n"
        "  q.r1.x + q.r1.y + (q.r1.f)() + q.r2.x + q.r2.y + (q.r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: init record to record in record, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "type Q = record r1: R, r2: R end\n"
        "def a\n"
        "  var r: R := { x: 3, f: &B, y: 4 }\n"
        "  var q: Q := { r1: { x: 1, f: &C, y: 2 }, r2: r }\n"
        "  r.x + r.y + (r.f)() + \n"
        "  q.r1.x + q.r1.y + (q.r1.f)() + q.r2.x + q.r2.y + (q.r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: init record in record to record in record, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "type Q = record r1: R, r2: R end\n"
        "var q2: Q := { r1: { x: 1, f: &C, y: 2 }, r2: { x: 3, f: &B, y: 4 } }\n"
        "var q1: Q := { r1: { x: 5, f: &D, y: 6 }, r2: q2.r2 }\n"
        "def a\n"
        "  q1.r1.x + q1.r1.y + (q1.r1.f)() + q1.r2.x + q1.r2.y + (q1.r2.f)() + \n"
        "  q2.r1.x + q2.r1.y + (q2.r1.f)() + q2.r2.x + q2.r2.y + (q2.r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 239, 0 );
}

TEST_CASE( "Algoly: init record in record to record in record, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "type Q = record r1: R, r2: R end\n"
        "var q2: Q := { r1: { x: 1, f: &C, y: 2 }, r2: { x: 3, f: &B, y: 4 } }\n"
        "def a\n"
        "  var q1: Q := { r1: { x: 5, f: &D, y: 6 }, r2: q2.r2 }\n"
        "  q1.r1.x + q1.r1.y + (q1.r1.f)() + q1.r2.x + q1.r2.y + (q1.r2.f)() + \n"
        "  q2.r1.x + q2.r1.y + (q2.r1.f)() + q2.r2.x + q2.r2.y + (q2.r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 239, 0 );
}

TEST_CASE( "Algoly: init record in record to record in record, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "type Q = record r1: R, r2: R end\n"
        "def a\n"
        "  var q2: Q := { r1: { x: 1, f: &C, y: 2 }, r2: { x: 3, f: &B, y: 4 } }\n"
        "  var q1: Q := { r1: { x: 5, f: &D, y: 6 }, r2: q2.r2 }\n"
        "  q1.r1.x + q1.r1.y + (q1.r1.f)() + q1.r2.x + q1.r2.y + (q1.r2.f)() + \n"
        "  q2.r1.x + q2.r1.y + (q2.r1.f)() + q2.r2.x + q2.r2.y + (q2.r2.f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 239, 0 );
}


//----------------------------------------------------------------------------
//  Assign record inside array
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: assign record in array to record, const indexes, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var ar: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 3, f: &B, y: 4 } ]\n"
        "var r: R := { x: 5, f: &D, y: 6 }\n"
        "def a\n"
        "  r := ar[1]\n"
        "  r.x + r.y + (r.f)() + \n"
        "  ar[0].x + ar[0].y + (ar[0].f)() + ar[1].x + ar[1].y + (ar[1].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: assign record in array to record, const indexes, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var ar: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 3, f: &B, y: 4 } ]\n"
        "def a\n"
        "  var r: R := { x: 5, f: &D, y: 6 }\n"
        "  r := ar[1]\n"
        "  r.x + r.y + (r.f)() + \n"
        "  ar[0].x + ar[0].y + (ar[0].f)() + ar[1].x + ar[1].y + (ar[1].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: assign record in array to record, const indexes, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "def a\n"
        "  var ar: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 3, f: &B, y: 4 } ]\n"
        "  var r: R := { x: 5, f: &D, y: 6 }\n"
        "  r := ar[1]\n"
        "  r.x + r.y + (r.f)() + \n"
        "  ar[0].x + ar[0].y + (ar[0].f)() + ar[1].x + ar[1].y + (ar[1].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: assign record in array to record, var indexes, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var ar: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 3, f: &B, y: 4 } ]\n"
        "var r: R := { x: 5, f: &D, y: 6 }\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  r := ar[one]\n"
        "  r.x + r.y + (r.f)() + \n"
        "  ar[zero].x + ar[zero].y + (ar[zero].f)() + ar[one].x + ar[one].y + (ar[one].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: assign record in array to record, var indexes, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var ar: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 3, f: &B, y: 4 } ]\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  var r: R := { x: 5, f: &D, y: 6 }\n"
        "  r := ar[one]\n"
        "  r.x + r.y + (r.f)() + \n"
        "  ar[zero].x + ar[zero].y + (ar[zero].f)() + ar[one].x + ar[one].y + (ar[one].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: assign record in array to record, var indexes, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  var ar: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 3, f: &B, y: 4 } ]\n"
        "  var r: R := { x: 5, f: &D, y: 6 }\n"
        "  r := ar[one]\n"
        "  r.x + r.y + (r.f)() + \n"
        "  ar[zero].x + ar[zero].y + (ar[zero].f)() + ar[one].x + ar[one].y + (ar[one].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: assign record to record in array, const indexes, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var r: R := { x: 3, f: &B, y: 4 }\n"
        "var ar: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 5, f: &D, y: 6 } ]\n"
        "def a\n"
        "  ar[1] := r\n"
        "  r.x + r.y + (r.f)() + \n"
        "  ar[0].x + ar[0].y + (ar[0].f)() + ar[1].x + ar[1].y + (ar[1].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: assign record to record in array, const indexes, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var r: R := { x: 3, f: &B, y: 4 }\n"
        "def a\n"
        "  var ar: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 5, f: &D, y: 6 } ]\n"
        "  ar[1] := r\n"
        "  r.x + r.y + (r.f)() + \n"
        "  ar[0].x + ar[0].y + (ar[0].f)() + ar[1].x + ar[1].y + (ar[1].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: assign record to record in array, const indexes, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "def a\n"
        "  var r: R := { x: 3, f: &B, y: 4 }\n"
        "  var ar: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 5, f: &D, y: 6 } ]\n"
        "  ar[1] := r\n"
        "  r.x + r.y + (r.f)() + \n"
        "  ar[0].x + ar[0].y + (ar[0].f)() + ar[1].x + ar[1].y + (ar[1].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: assign record to record in array, var indexes, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var r: R := { x: 3, f: &B, y: 4 }\n"
        "var ar: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 5, f: &D, y: 6 } ]\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  ar[one] := r\n"
        "  r.x + r.y + (r.f)() + \n"
        "  ar[zero].x + ar[zero].y + (ar[zero].f)() + ar[one].x + ar[one].y + (ar[one].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: assign record to record in array, var indexes, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var r: R := { x: 3, f: &B, y: 4 }\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  var ar: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 5, f: &D, y: 6 } ]\n"
        "  ar[one] := r\n"
        "  r.x + r.y + (r.f)() + \n"
        "  ar[zero].x + ar[zero].y + (ar[zero].f)() + ar[one].x + ar[one].y + (ar[one].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: assign record to record in array, var indexes, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  var r: R := { x: 3, f: &B, y: 4 }\n"
        "  var ar: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 5, f: &D, y: 6 } ]\n"
        "  ar[one] := r\n"
        "  r.x + r.y + (r.f)() + \n"
        "  ar[zero].x + ar[zero].y + (ar[zero].f)() + ar[one].x + ar[one].y + (ar[one].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: assign record in array to record in array, const indexes, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var ar2: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 3, f: &B, y: 4 } ]\n"
        "var ar1: [2] of R := [ { x: 5, f: &D, y: 6 }, { x: 7, f: &D, y: 8 } ]\n"
        "def a\n"
        "  ar1[1] := ar2[1]\n"
        "  ar1[0].x + ar1[0].y + (ar1[0].f)() + ar1[1].x + ar1[1].y + (ar1[1].f)() + \n"
        "  ar2[0].x + ar2[0].y + (ar2[0].f)() + ar2[1].x + ar2[1].y + (ar2[1].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 239, 0 );
}

TEST_CASE( "Algoly: assign record in array to record in array, const indexes, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var ar2: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 3, f: &B, y: 4 } ]\n"
        "def a\n"
        "  var ar1: [2] of R := [ { x: 5, f: &D, y: 6 }, { x: 7, f: &D, y: 8 } ]\n"
        "  ar1[1] := ar2[1]\n"
        "  ar1[0].x + ar1[0].y + (ar1[0].f)() + ar1[1].x + ar1[1].y + (ar1[1].f)() + \n"
        "  ar2[0].x + ar2[0].y + (ar2[0].f)() + ar2[1].x + ar2[1].y + (ar2[1].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 239, 0 );
}

TEST_CASE( "Algoly: assign record in array to record in array, const indexes, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "def a\n"
        "  var ar2: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 3, f: &B, y: 4 } ]\n"
        "  var ar1: [2] of R := [ { x: 5, f: &D, y: 6 }, { x: 7, f: &D, y: 8 } ]\n"
        "  ar1[1] := ar2[1]\n"
        "  ar1[0].x + ar1[0].y + (ar1[0].f)() + ar1[1].x + ar1[1].y + (ar1[1].f)() + \n"
        "  ar2[0].x + ar2[0].y + (ar2[0].f)() + ar2[1].x + ar2[1].y + (ar2[1].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 239, 0 );
}

TEST_CASE( "Algoly: assign record in array to record in array, var indexes, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var ar2: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 3, f: &B, y: 4 } ]\n"
        "var ar1: [2] of R := [ { x: 5, f: &D, y: 6 }, { x: 7, f: &D, y: 8 } ]\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  ar1[one] := ar2[one]\n"
        "  ar1[zero].x + ar1[zero].y + (ar1[zero].f)() + ar1[one].x + ar1[one].y + (ar1[one].f)() + \n"
        "  ar2[zero].x + ar2[zero].y + (ar2[zero].f)() + ar2[one].x + ar2[one].y + (ar2[one].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 239, 0 );
}

TEST_CASE( "Algoly: assign record in array to record in array, var indexes, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var ar2: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 3, f: &B, y: 4 } ]\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  var ar1: [2] of R := [ { x: 5, f: &D, y: 6 }, { x: 7, f: &D, y: 8 } ]\n"
        "  ar1[one] := ar2[one]\n"
        "  ar1[zero].x + ar1[zero].y + (ar1[zero].f)() + ar1[one].x + ar1[one].y + (ar1[one].f)() + \n"
        "  ar2[zero].x + ar2[zero].y + (ar2[zero].f)() + ar2[one].x + ar2[one].y + (ar2[one].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 239, 0 );
}

TEST_CASE( "Algoly: assign record in array to record in array, var indexes, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  var ar2: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 3, f: &B, y: 4 } ]\n"
        "  var ar1: [2] of R := [ { x: 5, f: &D, y: 6 }, { x: 7, f: &D, y: 8 } ]\n"
        "  ar1[one] := ar2[one]\n"
        "  ar1[zero].x + ar1[zero].y + (ar1[zero].f)() + ar1[one].x + ar1[one].y + (ar1[one].f)() + \n"
        "  ar2[zero].x + ar2[zero].y + (ar2[zero].f)() + ar2[one].x + ar2[one].y + (ar2[one].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 239, 0 );
}


//----------------------------------------------------------------------------
//  Init record inside array
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: init record in array to record, const indexes, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var ar: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 3, f: &B, y: 4 } ]\n"
        "var r: R := ar[1]\n"
        "def a\n"
        "  r.x + r.y + (r.f)() + \n"
        "  ar[0].x + ar[0].y + (ar[0].f)() + ar[1].x + ar[1].y + (ar[1].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: init record in array to record, const indexes, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var ar: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 3, f: &B, y: 4 } ]\n"
        "def a\n"
        "  var r: R := ar[1]\n"
        "  r.x + r.y + (r.f)() + \n"
        "  ar[0].x + ar[0].y + (ar[0].f)() + ar[1].x + ar[1].y + (ar[1].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: init record in array to record, const indexes, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "def a\n"
        "  var ar: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 3, f: &B, y: 4 } ]\n"
        "  var r: R := ar[1]\n"
        "  r.x + r.y + (r.f)() + \n"
        "  ar[0].x + ar[0].y + (ar[0].f)() + ar[1].x + ar[1].y + (ar[1].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: init record in array to record, var indexes, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var ar: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 3, f: &B, y: 4 } ]\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  var r: R := ar[one]\n"
        "  r.x + r.y + (r.f)() + \n"
        "  ar[zero].x + ar[zero].y + (ar[zero].f)() + ar[one].x + ar[one].y + (ar[one].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: init record in array to record, var indexes, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  var ar: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 3, f: &B, y: 4 } ]\n"
        "  var r: R := ar[one]\n"
        "  r.x + r.y + (r.f)() + \n"
        "  ar[zero].x + ar[zero].y + (ar[zero].f)() + ar[one].x + ar[one].y + (ar[one].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: init record to record in array, const indexes, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var r: R := { x: 3, f: &B, y: 4 }\n"
        "var ar: [2] of R := [ { x: 1, f: &C, y: 2 }, r ]\n"
        "def a\n"
        "  r.x + r.y + (r.f)() + \n"
        "  ar[0].x + ar[0].y + (ar[0].f)() + ar[1].x + ar[1].y + (ar[1].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: init record to record in array, const indexes, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var r: R := { x: 3, f: &B, y: 4 }\n"
        "def a\n"
        "  var ar: [2] of R := [ { x: 1, f: &C, y: 2 }, r ]\n"
        "  r.x + r.y + (r.f)() + \n"
        "  ar[0].x + ar[0].y + (ar[0].f)() + ar[1].x + ar[1].y + (ar[1].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: init record to record in array, const indexes, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "def a\n"
        "  var r: R := { x: 3, f: &B, y: 4 }\n"
        "  var ar: [2] of R := [ { x: 1, f: &C, y: 2 }, r ]\n"
        "  r.x + r.y + (r.f)() + \n"
        "  ar[0].x + ar[0].y + (ar[0].f)() + ar[1].x + ar[1].y + (ar[1].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 227, 0 );
}

TEST_CASE( "Algoly: init record in array to record in array, const indexes, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var ar2: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 3, f: &B, y: 4 } ]\n"
        "var ar1: [2] of R := [ { x: 5, f: &D, y: 6 }, ar2[1] ]\n"
        "def a\n"
        "  ar1[0].x + ar1[0].y + (ar1[0].f)() + ar1[1].x + ar1[1].y + (ar1[1].f)() + \n"
        "  ar2[0].x + ar2[0].y + (ar2[0].f)() + ar2[1].x + ar2[1].y + (ar2[1].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 239, 0 );
}

TEST_CASE( "Algoly: init record in array to record in array, const indexes, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "var ar2: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 3, f: &B, y: 4 } ]\n"
        "def a\n"
        "  var ar1: [2] of R := [ { x: 5, f: &D, y: 6 }, ar2[1] ]\n"
        "  ar1[0].x + ar1[0].y + (ar1[0].f)() + ar1[1].x + ar1[1].y + (ar1[1].f)() + \n"
        "  ar2[0].x + ar2[0].y + (ar2[0].f)() + ar2[1].x + ar2[1].y + (ar2[1].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 239, 0 );
}

TEST_CASE( "Algoly: init record in array to record in array, const indexes, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, f: &proc, y: int end\n"
        "def a\n"
        "  var ar2: [2] of R := [ { x: 1, f: &C, y: 2 }, { x: 3, f: &B, y: 4 } ]\n"
        "  var ar1: [2] of R := [ { x: 5, f: &D, y: 6 }, ar2[1] ]\n"
        "  ar1[0].x + ar1[0].y + (ar1[0].f)() + ar1[1].x + ar1[1].y + (ar1[1].f)() + \n"
        "  ar2[0].x + ar2[0].y + (ar2[0].f)() + ar2[1].x + ar2[1].y + (ar2[1].f)()\n"
        "end\n"
        "def B 100 end\n"
        "def C 10 end\n"
        "def D 1 end\n"
        ;

    TestCompileAndRunAlgoly( code, 239, 0 );
}


//----------------------------------------------------------------------------
//  Assign array of int inside record
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: assign array of int in record to array, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "var r: R := { a: [ 1, 2 ], b: [ 3, 4 ] }\n"
        "var ar: [2] := [ 5, 6 ]\n"
        "def a\n"
        "  ar := r.b\n"
        "  ar[0] + ar[1] + \n"
        "  r.a[0] + r.a[1] + r.b[0] + r.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0 );
}

TEST_CASE( "Algoly: assign array of int in record to array, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "var r: R := { a: [ 1, 2 ], b: [ 3, 4 ] }\n"
        "def a\n"
        "  var ar: [2] := [ 5, 6 ]\n"
        "  ar := r.b\n"
        "  ar[0] + ar[1] + \n"
        "  r.a[0] + r.a[1] + r.b[0] + r.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0 );
}

TEST_CASE( "Algoly: assign array of int in record to array, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "def a\n"
        "  var r: R := { a: [ 1, 2 ], b: [ 3, 4 ] }\n"
        "  var ar: [2] := [ 5, 6 ]\n"
        "  ar := r.b\n"
        "  ar[0] + ar[1] + \n"
        "  r.a[0] + r.a[1] + r.b[0] + r.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0 );
}

TEST_CASE( "Algoly: assign array to array of int in record, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "var ar: [2] := [ 3, 4 ]\n"
        "var r: R := { a: [ 1, 2 ], b: [ 5, 6 ] }\n"
        "def a\n"
        "  r.b := ar\n"
        "  ar[0] + ar[1] + \n"
        "  r.a[0] + r.a[1] + r.b[0] + r.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0 );
}

TEST_CASE( "Algoly: assign array to array of int in record, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "var ar: [2] := [ 3, 4 ]\n"
        "def a\n"
        "  var r: R := { a: [ 1, 2 ], b: [ 5, 6 ] }\n"
        "  r.b := ar\n"
        "  ar[0] + ar[1] + \n"
        "  r.a[0] + r.a[1] + r.b[0] + r.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0 );
}

TEST_CASE( "Algoly: assign array to array of int in record, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "def a\n"
        "  var ar: [2] := [ 3, 4 ]\n"
        "  var r: R := { a: [ 1, 2 ], b: [ 5, 6 ] }\n"
        "  r.b := ar\n"
        "  ar[0] + ar[1] + \n"
        "  r.a[0] + r.a[1] + r.b[0] + r.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0 );
}

TEST_CASE( "Algoly: assign array in record to array of int in record, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "var r2: R := { a: [ 1, 2 ], b: [ 3, 4 ] }\n"
        "var r1: R := { a: [ 5, 6 ], b: [ 7, 8 ] }\n"
        "def a\n"
        "  r1.b := r2.b\n"
        "  r1.a[0] + r1.a[1] + r1.b[0] + r1.b[1] + \n"
        "  r2.a[0] + r2.a[1] + r2.b[0] + r2.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 28, 0 );
}

TEST_CASE( "Algoly: assign array in record to array of int in record, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "var r2: R := { a: [ 1, 2 ], b: [ 3, 4 ] }\n"
        "def a\n"
        "  var r1: R := { a: [ 5, 6 ], b: [ 7, 8 ] }\n"
        "  r1.b := r2.b\n"
        "  r1.a[0] + r1.a[1] + r1.b[0] + r1.b[1] + \n"
        "  r2.a[0] + r2.a[1] + r2.b[0] + r2.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 28, 0 );
}

TEST_CASE( "Algoly: assign array in record to array of int in record, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "def a\n"
        "  var r2: R := { a: [ 1, 2 ], b: [ 3, 4 ] }\n"
        "  var r1: R := { a: [ 5, 6 ], b: [ 7, 8 ] }\n"
        "  r1.b := r2.b\n"
        "  r1.a[0] + r1.a[1] + r1.b[0] + r1.b[1] + \n"
        "  r2.a[0] + r2.a[1] + r2.b[0] + r2.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 28, 0 );
}


//----------------------------------------------------------------------------
//  Init array of int inside record
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: init array of int in record to array, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "var r: R := { a: [ 1, 2 ], b: [ 3, 4 ] }\n"
        "var ar: [2] := r.b\n"
        "def a\n"
        "  ar[0] + ar[1] + \n"
        "  r.a[0] + r.a[1] + r.b[0] + r.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0 );
}

TEST_CASE( "Algoly: init array of int in record to array, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "var r: R := { a: [ 1, 2 ], b: [ 3, 4 ] }\n"
        "def a\n"
        "  var ar: [2] := r.b\n"
        "  ar[0] + ar[1] + \n"
        "  r.a[0] + r.a[1] + r.b[0] + r.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0 );
}

TEST_CASE( "Algoly: init array of int in record to array, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "def a\n"
        "  var r: R := { a: [ 1, 2 ], b: [ 3, 4 ] }\n"
        "  var ar: [2] := r.b\n"
        "  ar[0] + ar[1] + \n"
        "  r.a[0] + r.a[1] + r.b[0] + r.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0 );
}

TEST_CASE( "Algoly: init array to array of int in record, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "var ar: [2] := [ 3, 4 ]\n"
        "var r: R := { a: [ 1, 2 ], b: ar }\n"
        "def a\n"
        "  ar[0] + ar[1] + \n"
        "  r.a[0] + r.a[1] + r.b[0] + r.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0 );
}

TEST_CASE( "Algoly: init array to array of int in record, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "var ar: [2] := [ 3, 4 ]\n"
        "def a\n"
        "  var r: R := { a: [ 1, 2 ], b: ar }\n"
        "  ar[0] + ar[1] + \n"
        "  r.a[0] + r.a[1] + r.b[0] + r.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0 );
}

TEST_CASE( "Algoly: init array to array of int in record, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "def a\n"
        "  var ar: [2] := [ 3, 4 ]\n"
        "  var r: R := { a: [ 1, 2 ], b: ar }\n"
        "  ar[0] + ar[1] + \n"
        "  r.a[0] + r.a[1] + r.b[0] + r.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 17, 0 );
}

TEST_CASE( "Algoly: init array in record to array of int in record, global-global", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "var r2: R := { a: [ 1, 2 ], b: [ 3, 4 ] }\n"
        "var r1: R := { a: [ 5, 6 ], b: r2.b }\n"
        "def a\n"
        "  r1.a[0] + r1.a[1] + r1.b[0] + r1.b[1] + \n"
        "  r2.a[0] + r2.a[1] + r2.b[0] + r2.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 28, 0 );
}

TEST_CASE( "Algoly: init array in record to array of int in record, global-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "var r2: R := { a: [ 1, 2 ], b: [ 3, 4 ] }\n"
        "def a\n"
        "  var r1: R := { a: [ 5, 6 ], b: r2.b }\n"
        "  r1.a[0] + r1.a[1] + r1.b[0] + r1.b[1] + \n"
        "  r2.a[0] + r2.a[1] + r2.b[0] + r2.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 28, 0 );
}

TEST_CASE( "Algoly: init array in record to array of int in record, local-local", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "def a\n"
        "  var r2: R := { a: [ 1, 2 ], b: [ 3, 4 ] }\n"
        "  var r1: R := { a: [ 5, 6 ], b: r2.b }\n"
        "  r1.a[0] + r1.a[1] + r1.b[0] + r1.b[1] + \n"
        "  r2.a[0] + r2.a[1] + r2.b[0] + r2.b[1]\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 28, 0 );
}


//----------------------------------------------------------------------------
//  Pass record by reference
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: pass global record by ref", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y end\n"
        "var r: R := { x: 1, y: 2 }\n"
        "def a\n"
        "  B( r )\n"
        "  r.x + r.y\n"
        "end\n"
        "def B( var br: R )\n"
        "  br.x := br.x + 10\n"
        "  br.y := br.y + 20\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 33, 0 );
}

TEST_CASE( "Algoly: pass local record by ref", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y end\n"
        "def a\n"
        "  var r: R := { x: 1, y: 2 }\n"
        "  B( r )\n"
        "  r.x + r.y\n"
        "end\n"
        "def B( var br: R )\n"
        "  br.x := br.x + 10\n"
        "  br.y := br.y + 20\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 33, 0 );
}

TEST_CASE( "Algoly: pass global record by ref 2", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y end\n"
        "var r: R := { x: 1, y: 2 }\n"
        "def a\n"
        "  B( r )\n"
        "  r.x + r.y\n"
        "end\n"
        "def B( var br: R )\n"
        "  br.x := br.x + 10\n"
        "  br.y := br.y + 20\n"
        "  C( br )\n"
        "end\n"
        "def C( var cr: R )\n"
        "  cr.x := cr.x + 100\n"
        "  cr.y := cr.y + 200\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 333, 0 );
}

TEST_CASE( "Algoly: pass local record by ref 2", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y end\n"
        "def a\n"
        "  var r: R := { x: 1, y: 2 }\n"
        "  B( r )\n"
        "  r.x + r.y\n"
        "end\n"
        "def B( var br: R )\n"
        "  br.x := br.x + 10\n"
        "  br.y := br.y + 20\n"
        "  C( br )\n"
        "end\n"
        "def C( var cr: R )\n"
        "  cr.x := cr.x + 100\n"
        "  cr.y := cr.y + 200\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 333, 0 );
}


//----------------------------------------------------------------------------
//  Pass record in record by reference
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: pass global record in record by ref", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y end\n"
        "type Q = record a: R, b: R end\n"
        "var q: Q := { a: { x: 1, y: 2 }, b: { x: 3, y: 4 } }\n"
        "def a\n"
        "  B( q.b )\n"
        "  q.a.x + q.a.y + q.b.x + q.b.y\n"
        "end\n"
        "def B( var br: R )\n"
        "  br.x := br.x + 10\n"
        "  br.y := br.y + 20\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 40, 0 );
}

TEST_CASE( "Algoly: pass local record in record by ref", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y end\n"
        "type Q = record a: R, b: R end\n"
        "def a\n"
        "  var q: Q := { a: { x: 1, y: 2 }, b: { x: 3, y: 4 } }\n"
        "  B( q.b )\n"
        "  q.a.x + q.a.y + q.b.x + q.b.y\n"
        "end\n"
        "def B( var br: R )\n"
        "  br.x := br.x + 10\n"
        "  br.y := br.y + 20\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 40, 0 );
}

TEST_CASE( "Algoly: pass global record in record by ref 2", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y end\n"
        "type Q = record a, b: R end\n"
        "var q: Q := { a: 2, b: { x: 1, y: 2 } }\n"
        "def a\n"
        "  B( q )\n"
        "  q.a + q.b.x + q.b.y\n"
        "end\n"
        "def B( var bq: Q )\n"
        "  bq.a := bq.a + 1\n"
        "  bq.b.x := bq.b.x + 10\n"
        "  bq.b.y := bq.b.y + 20\n"
        "  C( bq.b )\n"
        "end\n"
        "def C( var cr: R )\n"
        "  cr.x := cr.x + 100\n"
        "  cr.y := cr.y + 200\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 336, 0 );
}

TEST_CASE( "Algoly: pass local record in record by ref 2", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y end\n"
        "type Q = record a, b: R end\n"
        "def a\n"
        "  var q: Q := { a: 2, b: { x: 1, y: 2 } }\n"
        "  B( q )\n"
        "  q.a + q.b.x + q.b.y\n"
        "end\n"
        "def B( var bq: Q )\n"
        "  bq.a := bq.a + 1\n"
        "  bq.b.x := bq.b.x + 10\n"
        "  bq.b.y := bq.b.y + 20\n"
        "  C( bq.b )\n"
        "end\n"
        "def C( var cr: R )\n"
        "  cr.x := cr.x + 100\n"
        "  cr.y := cr.y + 200\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 336, 0 );
}

//----------------------------------------------------------------------------
//  Pass record in array by reference
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: pass global record in array by ref", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y end\n"
        "var ar: [2] of R := [ { x: 1, y: 2 }, { x: 3, y: 4 } ]\n"
        "def a\n"
        "  B( ar[1] )\n"
        "  ar[0].x + ar[0].y + ar[1].x + ar[1].y\n"
        "end\n"
        "def B( var br: R )\n"
        "  br.x := br.x + 10\n"
        "  br.y := br.y + 20\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 40, 0 );
}

TEST_CASE( "Algoly: pass local record in array by ref", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y end\n"
        "def a\n"
        "  var ar: [2] of R := [ { x: 1, y: 2 }, { x: 3, y: 4 } ]\n"
        "  B( ar[1] )\n"
        "  ar[0].x + ar[0].y + ar[1].x + ar[1].y\n"
        "end\n"
        "def B( var br: R )\n"
        "  br.x := br.x + 10\n"
        "  br.y := br.y + 20\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 40, 0 );
}

TEST_CASE( "Algoly: pass global record in array by ref 2", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y end\n"
        "var ar: [2] of R := [ { x: 3, y: 4 }, { x: 1, y: 2 } ]\n"
        "def a\n"
        "  B( ar )\n"
        "  ar[0].x + ar[0].y + ar[1].x + ar[1].y\n"
        "end\n"
        "def B( var bar: [2] of R )\n"
        "  bar[1].x := bar[1].x + 10\n"
        "  bar[1].y := bar[1].y + 20\n"
        "  C( bar[1] )\n"
        "end\n"
        "def C( var cr: R )\n"
        "  cr.x := cr.x + 100\n"
        "  cr.y := cr.y + 200\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 333+7, 0 );
}

TEST_CASE( "Algoly: pass local record in array by ref 2", "[algoly][record]" )
{
    const char code[] =
        "type R = record x, y end\n"
        "def a\n"
        "  var ar: [2] of R := [ { x: 3, y: 4 }, { x: 1, y: 2 } ]\n"
        "  B( ar )\n"
        "  ar[0].x + ar[0].y + ar[1].x + ar[1].y\n"
        "end\n"
        "def B( var bar: [2] of R )\n"
        "  bar[1].x := bar[1].x + 10\n"
        "  bar[1].y := bar[1].y + 20\n"
        "  C( bar[1] )\n"
        "end\n"
        "def C( var cr: R )\n"
        "  cr.x := cr.x + 100\n"
        "  cr.y := cr.y + 200\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 333+7, 0 );
}


//----------------------------------------------------------------------------
//  Pass array in record by reference
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: pass global array in record by ref", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "var r: R := { a: [ 1, 2 ], b: [ 3, 4 ] }\n"
        "def a\n"
        "  B( r.b )\n"
        "  r.a[0] + r.a[1] + r.b[0] + r.b[1]\n"
        "end\n"
        "def B( var bar: [2] )\n"
        "  bar[0] := bar[0] + 10\n"
        "  bar[1] := bar[1] + 20\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 40, 0 );
}

TEST_CASE( "Algoly: pass local array in record by ref", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "def a\n"
        "  var r: R := { a: [ 1, 2 ], b: [ 3, 4 ] }\n"
        "  B( r.b )\n"
        "  r.a[0] + r.a[1] + r.b[0] + r.b[1]\n"
        "end\n"
        "def B( var bar: [2] )\n"
        "  bar[0] := bar[0] + 10\n"
        "  bar[1] := bar[1] + 20\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 40, 0 );
}

TEST_CASE( "Algoly: pass global array in record by ref 2", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "var r: R := { a: [ 1, 2 ], b: [ 3, 4 ] }\n"
        "def a\n"
        "  B( r )\n"
        "  r.a[0] + r.a[1] + r.b[0] + r.b[1]\n"
        "end\n"
        "def B( var br: R )\n"
        "  br.b[0] := br.b[0] + 10\n"
        "  br.b[1] := br.b[1] + 20\n"
        "  C( br.b )\n"
        "end\n"
        "def C( var car: [2] )\n"
        "  car[0] := car[0] + 100\n"
        "  car[1] := car[1] + 200\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 333 + 7, 0 );
}

TEST_CASE( "Algoly: pass local array in record by ref 2", "[algoly][record]" )
{
    const char code[] =
        "type R = record a: [2], b: [2] end\n"
        "def a\n"
        "  var r: R := { a: [ 1, 2 ], b: [ 3, 4 ] }\n"
        "  B( r )\n"
        "  r.a[0] + r.a[1] + r.b[0] + r.b[1]\n"
        "end\n"
        "def B( var br: R )\n"
        "  br.b[0] := br.b[0] + 10\n"
        "  br.b[1] := br.b[1] + 20\n"
        "  C( br.b )\n"
        "end\n"
        "def C( var car: [2] )\n"
        "  car[0] := car[0] + 100\n"
        "  car[1] := car[1] + 200\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, 333 + 7, 0 );
}

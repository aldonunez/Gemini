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

    TestCompileAndRunAlgoly( code, sizeof code, 0, 0, 3 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 0, 0, 3 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 3, 0, 3 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 3, 0, 4 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 9, 0, 3 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 9, 0, 4 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 7, 0, 4 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 7, 0, 6 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 30, 0, 4 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 30, 0, 6 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 0, 0, 4 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 0, 0, 6 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 107, 0, 4 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 107, 0, 7 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 130, 0, 4 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 130, 0, 7 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 100, 0, 4 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 100, 0, 7 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 6, 0, 4 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 12, 0, 4 );
}

//---

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

    TestCompileAndRunAlgoly( code, sizeof code, 214, 0, 4 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 214, 0, 10 );
}

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

    TestCompileAndRunAlgoly( code, sizeof code, 10, 0, 4 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 10, 0, 8 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 8, 0, 3 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 8, 0, 11 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 8, 0, 4 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 8, 0, 12 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 36, 0, 4 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 36, 0, 5 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 36, 0, 4 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 36, 0, 5 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 36, 0, 4 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 36, 0, 5 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 36, 0, 4 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 36, 0, 5 );
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

    TestCompileAndRunAlgoly( code, sizeof code, 35, 0, 17 );
}

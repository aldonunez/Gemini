#include "pch.h"
#include "TestBase.h"

using namespace Gemini;


TEST_CASE( "Algoly: init const record by copying, modules (var : var, fail)", "[algoly][ptr-const]" )
{
    const char* modeCodeA[] =
    {
        "type R = record a, b, f: @proc end\n"
        "var _0 := [10]\n"
        "var R1: R := { a: 1, b: 2, f: @F }\n"
        "def E 10 end\n"
        "def F 3 end\n"
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "var _0 := [10, 20]\n"
        "var R2: ModA.R := ModA.R1\n"
        "def a\n"
        "  R2.a + R2.b + (R2.f)()\n"
        "end\n"
    };

    const ModuleSource modSources[] =
    {
        { "ModA",   Span( modeCodeA ) },
        { "Main",   Span( mainCode ) },
    };

    TestCompileAndRun( Language::Gema, modSources, Emplace<ResultKind::Compiler>( CompilerErr::SEMANTICS ), ParamSpan() );
}


//----------------------------------------------------------------------------
// Init const with const
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: init const int field from const field, modules", "[algoly][ptr-const][module]" )
{
    const char* modeCodeA[] =
    {
        "type R = record a, f: @proc end\n"
        "const _0 = [10]\n"
        "const R1: R = { a: 1, f: @F }\n"
        "def E 10 end\n"
        "def F 3 end\n"
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "const _0 = [10, 20]\n"
        "const R2: ModA.R = { a: 2, f: ModA.R1.f }\n"
        "def a\n"
        "  R2.a + (R2.f)()\n"
        "end\n"
    };

    const ModuleSource modSources[] =
    {
        { "ModA",   Span( modeCodeA ) },
        { "Main",   Span( mainCode ) },
    };

    TestCompileAndRun( Language::Gema, modSources, 5, 0 );
}

TEST_CASE( "Algoly: init const record by copying, modules", "[algoly][ptr-const][module]" )
{
    const char* modeCodeA[] =
    {
        "type R = record a, b, f: @proc end\n"
        "const _0 = [10]\n"
        "const R1: R = { a: 1, b: 2, f: @F }\n"
        "def E 10 end\n"
        "def F 3 end\n"
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "const _0 = [10, 20]\n"
        "const R2: ModA.R = ModA.R1\n"
        "def a\n"
        "  R2.a + R2.b + (R2.f)()\n"
        "end\n"
    };

    const ModuleSource modSources[] =
    {
        { "ModA",   Span( modeCodeA ) },
        { "Main",   Span( mainCode ) },
    };

    TestCompileAndRun( Language::Gema, modSources, 6, 0 );
}

TEST_CASE( "Algoly: init var record by copying const, modules", "[algoly][ptr-const][module]" )
{
    const char* modeCodeA[] =
    {
        "type R = record a, b, f: @proc end\n"
        "const _0 = [10, 20]\n"
        "const R1: R = { a: 1, b: 2, f: @F }\n"
        "def E 10 end\n"
        "def F 3 end\n"
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "var _0 := 30\n"
        "var R2 := ModA.R1\n"
        "def a\n"
        "  R2.a + R2.b + (R2.f)()\n"
        "end\n"
    };

    const ModuleSource modSources[] =
    {
        { "ModA",   Span( modeCodeA ) },
        { "Main",   Span( mainCode ) },
    };

    TestCompileAndRun( Language::Gema, modSources, 6, 0 );
}

TEST_CASE( "Algoly: init const record by record initializer and copying, modules", "[algoly][ptr-const][module]" )
{
    const char* modeCodeA[] =
    {
        "type R = record a, f: @proc end\n"
        "const _0 = [10, 20]\n"
        "const R1: R = { a: 3, f: @F }\n"
        "def E 10 end\n"
        "def F 4 end\n"
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "type S = record c: ModA.R, r: ModA.R end\n"
        "const _0 = [30]\n"
        "const S1: S = { c: { a: 1, f: @H }, r: ModA.R1 }\n"
        "def a\n"
        "  S1.c.a + (S1.c.f)() + S1.r.a + (S1.r.f)()\n"
        "end\n"
        "def H 2 end\n"
    };

    const ModuleSource modSources[] =
    {
        { "ModA",   Span( modeCodeA ) },
        { "Main",   Span( mainCode ) },
    };

    TestCompileAndRun( Language::Gema, modSources, 10, 0 );
}

TEST_CASE( "Algoly: init var record by record initializer and copying const, modules", "[algoly][ptr-const][module]" )
{
    const char* modeCodeA[] =
    {
        "type R = record a, f: @proc end\n"
        "const _0 = [10, 20]\n"
        "const R1: R = { a: 3, f: @F }\n"
        "def E 10 end\n"
        "def F 4 end\n"
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "type S = record c: ModA.R, r: ModA.R end\n"
        "var _0 := 30\n"
        "var S1: S := { c: { a: 1, f: @H }, r: ModA.R1 }\n"
        "def a\n"
        "  S1.c.a + (S1.c.f)() + S1.r.a + (S1.r.f)()\n"
        "end\n"
        "def H 2 end\n"
    };

    const ModuleSource modSources[] =
    {
        { "ModA",   Span( modeCodeA ) },
        { "Main",   Span( mainCode ) },
    };

    TestCompileAndRun( Language::Gema, modSources, 10, 0 );
}

TEST_CASE( "Algoly: init const record field by record initializer and copying const, modules", "[algoly][ptr-const][module]" )
{
    const char* modeCodeA[] =
    {
        "type R = record a, f: @proc end\n"
        "type S = record c: R, r: R end\n"
        "const _0 = [10, 20]\n"
        "const S0: S = { c: { a: 5, f: @F }, r: { a: 3, f: @F } }\n"
        "def E 6 end\n"
        "def F 4 end\n"
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "const _0 = 30\n"
        "const S1: ModA.S = { c: { a: 1, f: @H }, r: ModA.S0.r }\n"
        "def a\n"
        "  S1.c.a + (S1.c.f)() + S1.r.a + (S1.r.f)()\n"
        "end\n"
        "def H 2 end\n"
    };

    const ModuleSource modSources[] =
    {
        { "ModA",   Span( modeCodeA ) },
        { "Main",   Span( mainCode ) },
    };

    TestCompileAndRun( Language::Gema, modSources, 10, 0 );
}

TEST_CASE( "Algoly: init var record field by record initializer and copying const, modules", "[algoly][ptr-const][module]" )
{
    const char* modeCodeA[] =
    {
        "type R = record a, f: @proc end\n"
        "type S = record c: R, r: R end\n"
        "const _0 = [10, 20]\n"
        "const S0: S = { c: { a: 5, f: @F }, r: { a: 3, f: @F } }\n"
        "def E 6 end\n"
        "def F 4 end\n"
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "var _0 := 30\n"
        "var S1: ModA.S := { c: { a: 1, f: @H }, r: ModA.S0.r }\n"
        "def a\n"
        "  S1.c.a + (S1.c.f)() + S1.r.a + (S1.r.f)()\n"
        "end\n"
        "def H 2 end\n"
    };

    const ModuleSource modSources[] =
    {
        { "ModA",   Span( modeCodeA ) },
        { "Main",   Span( mainCode ) },
    };

    TestCompileAndRun( Language::Gema, modSources, 10, 0 );
}


//----------------------------------------------------------------------------
// Const nested record and arrays
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: const record of int and fptr in array, var index, modules", "[algoly][ptr-const][module]" )
{
    const char* modeCodeA[] =
    {
        "type R = record a, b, f: @proc end\n"
        "const _0 = [10]\n"
        "const AR: [2] of R = [{ a: 1, b: 2, f: @B }, { a: 3, b: 4, f: @C }]\n"
        "def B 5 end\n"
        "def C 10 end\n"
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "var i := 0, j := 1\n"
        "def a\n"
        "  ModA.AR[i].a + ModA.AR[i].b + (ModA.AR[i].f)() + \n"
        "  ModA.AR[j].a + ModA.AR[j].b + (ModA.AR[j].f)() \n"
        "end\n"
    };

    const ModuleSource modSources[] =
    {
        { "ModA",   Span( modeCodeA ) },
        { "Main",   Span( mainCode ) },
    };

    TestCompileAndRun( Language::Gema, modSources, 25, 0 );
}


//----------------------------------------------------------------------------
// Var arguments to const parameters
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: pass var record to const param, modules", "[algoly][ptr-const][module]" )
{
    const char* modeCodeA[] =
    {
        "type R = record a, b, f: @proc end\n"
        "var _0 := [10]\n"
        "var r1: R := { a: 1, b: 2, f: @F }\n"
        "def F 3 end\n"
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "def a\n"
        "  B(ModA.r1)\n"
        "end\n"
        "def B(const RR: ModA.R) RR.a + RR.b + (RR.f)() end\n"
    };

    const ModuleSource modSources[] =
    {
        { "ModA",   Span( modeCodeA ) },
        { "Main",   Span( mainCode ) },
    };

    TestCompileAndRun( Language::Gema, modSources, 6, 0 );
}

TEST_CASE( "Algoly: pass var array in array to const param open array, var index array and param, modules", "[algoly][ptr-const][module]" )
{
    const char* modeCodeA[] =
    {
        "var _0 := [10]\n"
        "var ar1 := [[@A, @B], [@C, @D]]\n"
        "def A 1 end\n"
        "def B 2 end\n"
        "def C 3 end\n"
        "def D 4 end\n"
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  B(@ModA.ar1[one])\n"
        "end\n"
        "def B(const ARRAY: [] of @proc) (ARRAY[zero])() + (ARRAY[one])() + countof(ARRAY) end\n"
    };

    const ModuleSource modSources[] =
    {
        { "ModA",   Span( modeCodeA ) },
        { "Main",   Span( mainCode ) },
    };

    TestCompileAndRun( Language::Gema, modSources, 9, 0 );
}


//----------------------------------------------------------------------------
// Const arguments to const parameters - global
//----------------------------------------------------------------------------

TEST_CASE( "Algoly: pass const record to const param, modules", "[algoly][ptr-const][module]" )
{
    const char* modeCodeA[] =
    {
        "type R = record a, b, f: @proc end\n"
        "const _0 = [10]\n"
        "const r1: R = { a: 1, b: 2, f: @F }\n"
        "def F 3 end\n"
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "def a\n"
        "  B(ModA.r1)\n"
        "end\n"
        "def B(const RR: ModA.R) RR.a + RR.b + (RR.f)() end\n"
    };

    const ModuleSource modSources[] =
    {
        { "ModA",   Span( modeCodeA ) },
        { "Main",   Span( mainCode ) },
    };

    TestCompileAndRun( Language::Gema, modSources, 6, 0 );
}

TEST_CASE( "Algoly: pass const array in array to const param open array, var index array and param, modules", "[algoly][ptr-const][module]" )
{
    const char* modeCodeA[] =
    {
        "const _0 = [10]\n"
        "const ar1 = [[@A, @B], [@C, @D]]\n"
        "def A 1 end\n"
        "def B 2 end\n"
        "def C 3 end\n"
        "def D 4 end\n"
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "var zero := 0, one := 1\n"
        "def a\n"
        "  B(@ModA.ar1[one])\n"
        "end\n"
        "def B(const ARRAY: [] of @proc) (ARRAY[zero])() + (ARRAY[one])() + countof(ARRAY) end\n"
    };

    const ModuleSource modSources[] =
    {
        { "ModA",   Span( modeCodeA ) },
        { "Main",   Span( mainCode ) },
    };

    TestCompileAndRun( Language::Gema, modSources, 9, 0 );
}

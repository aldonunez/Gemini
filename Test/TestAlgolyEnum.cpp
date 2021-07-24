#include "pch.h"
#include "TestBase.h"


TEST_CASE( "Algoly: simple enum first", "[algoly][enum]" )
{
    const char code[] =
        "type E = enum E1, E2, E3 end\n"
        "def a->E E.E1 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 0, 0, 3 );
}

TEST_CASE( "Algoly: simple enum last", "[algoly][enum]" )
{
    const char code[] =
        "type E = enum E1, E2, E3 end\n"
        "def a->E E.E3 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 2, 0, 3 );
}

TEST_CASE( "Algoly: simple enum first, trailing comma", "[algoly][enum]" )
{
    const char code[] =
        "type E = enum E1, E2, E3, end\n"
        "def a->E E.E1 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 0, 0, 3 );
}

TEST_CASE( "Algoly: explicit first enum first", "[algoly][enum]" )
{
    const char code[] =
        "type E = enum E1=4, E2, E3 end\n"
        "def a->E E.E1 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 4, 0, 3 );
}

TEST_CASE( "Algoly: explicit first enum last", "[algoly][enum]" )
{
    const char code[] =
        "type E = enum E1=4, E2, E3 end\n"
        "def a->E E.E3 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 6, 0, 3 );
}

TEST_CASE( "Algoly: explicit all enum first", "[algoly][enum]" )
{
    const char code[] =
        "type E = enum E1=4, E2=5, E3=6 end\n"
        "def a->E E.E1 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 4, 0, 3 );
}

TEST_CASE( "Algoly: explicit all enum last", "[algoly][enum]" )
{
    const char code[] =
        "type E = enum E1=4, E2=5, E3=6 end\n"
        "def a->E E.E3 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 6, 0, 3 );
}

TEST_CASE( "Algoly: enum as self", "[algoly][enum]" )
{
    const char code[] =
        "type E = enum E1, E2, E3 end\n"
        "def a -> E E.E3 as E end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 2, 0, 3 );
}

TEST_CASE( "Algoly: explicit first enum last as int", "[algoly][enum]" )
{
    const char code[] =
        "type E = enum E1=1, E2, E3 end\n"
        "def a E.E3 as int end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 3, 0, 3 );
}

TEST_CASE( "Algoly: explicit first enum as int add", "[algoly][enum]" )
{
    const char code[] =
        "type E = enum E1=1, E2, E3 end\n"
        "def a E.E2 as int + E.E3 as int end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 5, 0, 3 );
}

TEST_CASE( "Algoly: infer enum", "[algoly][enum]" )
{
    const char code[] =
        "type E = enum E1=1,  E2, E3 end\n"
        "type F = enum F1=10, F2, F3 end\n"
        "def a->F F.F2 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 11, 0, 3 );
}

TEST_CASE( "Algoly: array of enum assign local", "[algoly][enum]" )
{
    const char code[] =
        "type E = enum E1=1, E2, E3 end\n"
        "var arr: [3] of E := [E.E1, E.E2, E.E3]\n"
        "def a->E var e: E := arr[1]; e end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 2, 0, 4 );
}

TEST_CASE( "Algoly: enum as other enum", "[algoly][enum]" )
{
    const char code[] =
        "type E = enum E1, E2, end\n"
        "type F = enum F1, F2 = E.E2 as int, end\n"
        "def a->E var e: E := F.F2 as E; e end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 1, 0, 4 );
}

TEST_CASE( "Algoly: enum cast and call", "[algoly][enum]" )
{
    const char code[] =
        "type E = enum E1, E2, end\n"
        "type F = enum F1, F2 = E.E2 as int, end\n"
        //"def a1 var e: E := F.F2 as E; B(e) as int end\n"
        "def B(n: E)->E n end\n"
        "def a->F\n"
        "  var e: E := F.F2 as E,\n"
        "      f: &proc(E)->E := &B\n"
        "  (f)(e) as F\n"
        "end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 1, 0, 6 );
}

TEST_CASE( "Algoly: reference member inside enum def", "[algoly][enum]" )
{
    const char code[] =
        "type E = enum E1 = 5, E2=(E1 as int)+10, E3, end\n"
        "def a->E E.E3 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 16, 0, 3 );
}

TEST_CASE( "Algoly: int as int", "[algoly][enum]" )
{
    const char code[] =
        "def a 3 as int end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 3, 0, 3 );
}

TEST_CASE( "Algoly: enum as mod dotted int alias", "[algoly][enum]" )
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
        "type E = enum E3 = 3 end\n"
        "def a var i := E.E3; i as ModA.T end\n"
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

TEST_CASE( "Algoly: int as mod enum", "[algoly][enum]" )
{
    const char* modeCodeA[] =
    {
        "type E = enum E3 = 3 end\n"
        ,
        nullptr
    };

    const char* mainCode[] =
    {
        "import ModA\n"
        "def a -> ModA.E var i := 3; i as ModA.E end\n"
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

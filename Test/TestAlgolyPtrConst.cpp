#include "pch.h"
#include "TestBase.h"


TEST_CASE( "Algoly: simple const fptr", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a const f = &C; (f)() end\n"
        "def C -> int 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 3, 0, 3 );
}

#if 0
// TODO: Also global
TEST_CASE( "Algoly: simple const lambda", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a const f = lambda 3 end; (f)() end\n"
        //"def C -> int 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 3, 0, 3 );
}
#endif

TEST_CASE( "Algoly: const &proc no typeref, &proc(int) typeref, copy const, call", "[algoly][ptr-const]" )
{
    const char code[] =
        // Output: 8
        "def a\n"
        "  const f = &C, g: &proc(int) = &B\n"
        "  const h = f\n"
        "  (f)() + (g)(2) + (h)() end\n"
        "def B(x) x end\n"
        "def C 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 8, 0, 5 );
}

TEST_CASE( "Algoly: complex ptr-const", "[algoly][ptr-const]" )
{
    const char code[] =
        //"def a 33 end"
        // Output: 22
        "const FB = &B\n"
        "const FA = FB\n"
        "var GA := FA\n"
        "var GB := &C\n"
        "var arr: [3] of &proc->int := [&B, &C, FA]\n"
        "def a\n"
        "  const FC = &C, FD = FC\n"
        "  var VD := FD\n"
        "  (FC)() + (FD)() + (VD)() +\n"
        "  (FB)() + (FA)() + (GA)() +\n"
        "  (GB)() + (arr[0])() + (arr[2])()\n"
        "end\n"
        "def B 2 end\n"
        "def C 3 end\n"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 22, 0, 5 );
}

#if 0
TEST_CASE( "Algoly: simple", "[algoly][ptr-const]" )
{
    const char code[] =
        "def a 33 end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 0, 0, 0 );
}
#endif

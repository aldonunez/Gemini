#include "pch.h"
#include "TestBase.h"


TEST_CASE( "Algoly: CopyArray: default return", "[algoly][copyarray]" )
{
    const char code[] =
        "def a end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 0, 0, 3 );
}

#if 0
TEST_CASE( "Algoly: CopyArray: default return", "[algoly][copyarray]" )
{
    const char code[] =
        "def a end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 0, 0, 3 );
}
#endif

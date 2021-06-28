#define CATCH_CONFIG_RUNNER
#include "catch.hpp"


int main( int argc, char* argv[] )
{
    _CrtSetDbgFlag(
        _crtDbgFlag
        | _CRTDBG_LEAK_CHECK_DF
        | _CRTDBG_ALLOC_MEM_DF
    );

    int result = Catch::Session().run( argc, argv );

    return result;
}

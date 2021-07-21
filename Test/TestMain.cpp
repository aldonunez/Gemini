#define CATCH_CONFIG_RUNNER
#include "catch.hpp"


int main( int argc, char* argv[] )
{
#if defined(_WIN32)
    _CrtSetDbgFlag(
        _crtDbgFlag
        | _CRTDBG_LEAK_CHECK_DF
        | _CRTDBG_ALLOC_MEM_DF
    );
#endif

    int result = Catch::Session().run( argc, argv );

    return result;
}

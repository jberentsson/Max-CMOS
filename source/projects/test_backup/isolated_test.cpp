// Include NOTHING before catch.hpp
#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_FAST_COMPILE

// Undefine any potentially conflicting macros
#ifdef TEST_CASE
#undef TEST_CASE
#endif
#ifdef SECTION
#undef SECTION
#endif
#ifdef REQUIRE
#undef REQUIRE
#endif

#include "catch.hpp"

// Now do your tests
TEST_CASE("Basic test") {
    SECTION("Simple math") {
        REQUIRE(1 == 1);
    }
}

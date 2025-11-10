#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#include "CD4017.hpp"

SCENARIO("Test Initialize") {
	CD4017 c = CD4017(10);

	REQUIRE(true);
}

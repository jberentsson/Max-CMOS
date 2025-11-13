#include <catch2/catch.hpp>
#include "CD4017.hpp"

SCENARIO("Test Initialize") {
	CD4017 c = CD4017(10);

	REQUIRE(true);
}

SCENARIO("Test Reset") {
	CD4017 c = CD4017(10);

	REQUIRE(c.value() == 0);

	REQUIRE(c.step() == 1);
	REQUIRE(c.step() == 2);

	REQUIRE(c.value() == 2);

	REQUIRE(c.step() == 3);
	REQUIRE(c.reset() == 0);

	REQUIRE(c.value() == 0);
	REQUIRE(c.step() == 1);
	REQUIRE(c.value() == 1);

	REQUIRE(c.reset() == 0);
}

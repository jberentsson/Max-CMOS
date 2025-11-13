#include <catch2/catch.hpp>
#include "CD4024.hpp"

SCENARIO("Test Initialize") {
	CD4024 c = CD4024(10);

	REQUIRE(true);
}

SCENARIO("Back And Forth") {
	CD4024 c = CD4024(10);
	REQUIRE(c.direction() == 1);
	REQUIRE(c.value() == 0);
	REQUIRE(c.toggle_direction() == 0);
	REQUIRE(c.step() == 9);
	REQUIRE(c.toggle_direction() == 1);
	REQUIRE(c.step() == 0);
	REQUIRE(c.value() == 0);
}

SCENARIO("Preset") {
	CD4024 c = CD4024(10);
	REQUIRE(c.value() == 0);
	REQUIRE(c.set(6) == 6);
	REQUIRE(c.set(10) == 6);
	REQUIRE(c.set(-1) == 6);
	REQUIRE(c.step() == 7);
	REQUIRE(c.step() == 8);
	REQUIRE(c.step() == 9);
	REQUIRE(c.step() == 0);
}

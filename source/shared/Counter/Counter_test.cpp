#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#include "Counter.hpp"

SCENARIO("Test Initialize") {
	Counter c = Counter(10);

	REQUIRE(true);
}

SCENARIO("Test Step") {
	Counter c = Counter(10);

	for (int i = 0; i < 10; i++) {
		c.step();
	}
	
	REQUIRE(true);
	REQUIRE(c.current() == 0);
}

SCENARIO("Test Backwards Step") {
	Counter c = Counter(10);
	c.direction();
	for (int i = 0; i < 10; i++) {
		c.step();
	}
	
	REQUIRE(true);
	REQUIRE(c.current() == 0);
}

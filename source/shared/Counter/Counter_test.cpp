#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#include "Counter.hpp"

SCENARIO("Test Initialize") {
	Counter c = Counter(10);

	REQUIRE(true);
}

SCENARIO("Test Back Step") {
	Counter c = Counter(10);

	for (int i = 0; i < 10; i++) {
		c.back();
	}
	
	REQUIRE(c.current() == 0);
}

SCENARIO("Test Forward Step") {
	Counter c = Counter(10);
	
	c.direction();

	for (int i = 0; i < 10; i++) {
		c.forward();
	}
	
	REQUIRE(c.current() == 0);
}

SCENARIO("Test Reset") {
	Counter c = Counter(10);

	REQUIRE(c.current() == 0);

	REQUIRE(c.forward() == 1);
	REQUIRE(c.forward() == 2);

	REQUIRE(c.current() == 2);

	REQUIRE(c.forward() == 3);
	REQUIRE(c.reset() == 0);

	REQUIRE(c.current() == 0);
	REQUIRE(c.forward() == 1);
	REQUIRE(c.current() == 1);

	REQUIRE(c.reset() == 0);
}

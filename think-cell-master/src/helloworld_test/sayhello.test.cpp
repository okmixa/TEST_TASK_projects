
#include "catch.hpp"
#include "../helloworld/sayHello.h"

TEST_CASE("Test sayHello function", "[helloworld]") {

	SECTION("check sayHello returns 0") {

		REQUIRE(0 == helloworld::sayHello());
	}

}
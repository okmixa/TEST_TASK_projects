
#include "catch.hpp"
#include "interval_map/value.h"

TEST_CASE("check value constraints", "[Value]") {

	Value<int> v1(1);
	Value<int> v2(2);

	SECTION("Value is comparible via operator==") {

		try
		{
			Value<int> one1(1);
			Value<int> one2(1);
			REQUIRE(one1==one2);
		}
		catch (...)
		{
			REQUIRE(false);
		}
	}

	SECTION("keys are assignable") {

		try
		{
			v1 = v2;

			REQUIRE(v1==v2);
		}
		catch (...)
		{
			REQUIRE(false);
		}
	}

	SECTION("keys are copyable") {

		try
		{
			Value v3(v1);

			REQUIRE(v1==v3);
		}
		catch (...)
		{
			REQUIRE(false);
		}
	}

}
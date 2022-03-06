
#include "catch.hpp"
#include "interval_map/key.h"

namespace {

	template<class K>
	bool areEqual(Key<K> k1, Key<K> k2)
	{
		return !(k1 < k2) && !(k2 < k1);
	}
}

TEST_CASE("check key constraints", "[key]") {

	Key<int> k1(1);
	Key<int> k2(2);

	SECTION("key is comparible via operator<") {

		try
		{
			REQUIRE(k1<k2);
		}
		catch (...)
		{
			REQUIRE(false);
		}
	}

	SECTION("keys are assignable") {

		try
		{
			k1 = k2;

			REQUIRE(areEqual(k1, k2));
		}
		catch (...)
		{
			REQUIRE(false);
		}
	}

	SECTION("keys are copyable") {

		try
		{
			Key k3(k1);

			REQUIRE(areEqual(k1, k3));
		}
		catch (...)
		{
			REQUIRE(false);
		}
	}

}
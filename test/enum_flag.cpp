#pragma warning(push, 1)
#include <catch2/catch.hpp>
#pragma warning(pop)

#include <util/flag.hpp>

enum class my_enum : uint8_t {
  none = 0,
  one = 1,
  two = 1 << 1,
  four = 1 << 2,
  eight = 1 << 3
};



SCENARIO("flag_t")
{
  GIVEN("a flag with a single value")
  {
    auto one = flag(my_enum::one);

    THEN("it is the same size as the type it wraps")
    {
      REQUIRE(sizeof(one) == sizeof(my_enum));
    }
    // ==
    AND_THEN("the flag has the enum")
    {
      REQUIRE(one.has(my_enum::one));
    }
    AND_THEN("the flag does not have any of the other values")
    {
      REQUIRE(!one.has(my_enum::two));
      REQUIRE(!one.has(my_enum::four));
      REQUIRE(!one.has(my_enum::eight));
    }

    WHEN("it is assigned a new value by enum")
    {
      one = my_enum::two;

      THEN("it == the new value")
      {
        REQUIRE(one == my_enum::two);
      }
    }

    WHEN("it is assigned a new value by flag")
    {
      one = flag{ my_enum::two };

      THEN("it == the new value")
      {
        REQUIRE(one == my_enum::two);
        REQUIRE(my_enum::two == one);
      }
    }

    WHEN("another flag has the same value")
    {
      flag one_1(my_enum::one);
      flag one_2{ one };
      auto one_3{ one };

      THEN("they are equal")
      {
        REQUIRE(one_1 == one);
        REQUIRE(one == one_1);

        REQUIRE(one == one_2);
        REQUIRE(one_3 == one);
      }
    }
  }
  GIVEN("a flag with multiple values set")
  {
    flag one_two{ my_enum::one, my_enum::two };

    THEN("it is equal to others with the same flags set")
    {
      flag one_two_1{ my_enum::one };
      one_two_1 = one_two_1 | my_enum::two;
      REQUIRE(one_two == one_two_1);

      flag one_two_2{ my_enum::one };
      one_two_2 |= my_enum::two;
      REQUIRE(one_two == one_two_2);
    }
    AND_THEN("it is not equal to others that don't have the same flags set")
    {
      REQUIRE(one_two != flag{ my_enum::one });
      REQUIRE(one_two != flag{ my_enum::one, my_enum::four });
    }
    AND_THEN("it has all of the flags it contains")
    {
      REQUIRE(one_two.has(my_enum::one));
      REQUIRE(one_two.has(my_enum::two));
    }
    AND_THEN("it doesn't have any flags that are not set")
    {
      REQUIRE(!one_two.has(my_enum::four));
      REQUIRE(!one_two.has(my_enum::eight));
    }
  }
}

#pragma warning(push, 1)
#include <catch2/catch.hpp>
#pragma warning(pop)
#include <aoc/message_validator.h>

#include <fstream>
#include <tuple>
#include <sstream>

using namespace std::string_literals;
const auto data_dir = R"(D:\src\AdventOfCode\aoc2020cpp\test\data\)"s;

TEST_CASE("aoc 2020-19-1")
{
  const auto& [expected, path] = GENERATE(
    std::make_tuple(2, "19-simple.txt")
    ,std::make_tuple(3, "19-small.txt")
    ,std::make_tuple(156, "19.txt")
  );

  GIVEN(path)
  {
    std::ifstream input{ data_dir + path };
    
    AND_GIVEN("a message validator")
    {
      message_validator const validator(input,
        { message_validator::option::print_stats });

      WHEN("valid messages are counted")
      {
        auto count = validator.count_valid();

        std::stringstream name;
        name << "there are " << expected;
        THEN(name.str())
        {
          REQUIRE(expected == count);
        }
      }
    }
    AND_GIVEN("an optimized message validator")
    {
      message_validator const validator(input,
        { message_validator::option::optimize_rules, message_validator::option::print_stats });

      WHEN("valid messages are counted")
      {
        auto count = validator.count_valid();

        std::stringstream name;
        name << "there are " << expected;
        THEN(name.str())
        {
          REQUIRE(expected == count);
        }
      }
    }
  }
}

TEST_CASE("aoc 2020-19-2")
{
  const auto &[expected, path] = GENERATE(
    std::make_tuple(12, "19-small-2.txt")
    ,std::make_tuple(156, "19.txt")
  );

  GIVEN(path)
  {
    std::ifstream input{ data_dir + path };

    AND_GIVEN("a message validator")
    {
      message_validator const validator(input,
        { message_validator::option::print_stats });

      WHEN("valid messages are counted")
      {
        auto count = validator.count_valid();

        std::stringstream name;
        name << "there are " << expected;
        THEN(name.str())
        {
          REQUIRE(expected == count);
        }
      }
    }

    AND_GIVEN("an optimized message validator")
    {
      message_validator const validator(input, { message_validator::option::optimize_rules, message_validator::option::print_stats });

      WHEN("valid messages are counted")
      {
        auto count = validator.count_valid();

        std::stringstream name;
        name << "there are " << expected;
        THEN(name.str())
        {
          REQUIRE(expected == count);
        }
      }
    }
  }
}

TEST_CASE("aoc 2020-19-1 benchmarks")
{
  std::ifstream setup(data_dir + "19-small.txt");
  BENCHMARK("no flags")
  {
    message_validator const validator(setup);
    return validator.count_valid();
  };

  BENCHMARK("optimize rules")
  {
    message_validator const validator(setup, message_validator::option::optimize_rules);
    return validator.count_valid();
  };
}

TEST_CASE("aoc 2020-19-2 benchmarks")
{
  std::ifstream setup(data_dir + "19-small-2.txt");
  BENCHMARK("no flags")
  {
    setup.seekg(0, std::ifstream::beg);
    message_validator const validator(setup);
    return validator.count_valid();
  };

  BENCHMARK("optimize rules")
  {
    setup.seekg(0, std::ifstream::beg);
    message_validator const validator(setup, message_validator::option::optimize_rules);
    return validator.count_valid();
  };
}
#pragma warning(push, 1)
#include <catch2/catch.hpp>
#pragma warning(pop)
#include <aoc/message_validator.h>

#include "test_constants.h"

#include <fstream>
#include <tuple>
#include <sstream>
#include <filesystem>


TEST_CASE("aoc 2020-19-1")
{
  const auto& [expected, path] = GENERATE(
    std::make_tuple(2, "19-simple.txt")
    ,std::make_tuple(3, "19-small.txt")
    ,std::make_tuple(156, "19.txt")
  );

  GIVEN(path)
  {
    auto input_path = data_dir / path;
    REQUIRE(std::filesystem::exists(input_path));
    std::ifstream input{ input_path.native() };
    
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
    auto input_path = data_dir / path;
    REQUIRE(std::filesystem::exists(input_path));
    std::ifstream input{ input_path.native() };

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
  auto input_path = data_dir / "19-small.txt";
  REQUIRE(std::filesystem::exists(input_path));
  std::ifstream input{ input_path };
  BENCHMARK("no flags")
  {
    message_validator const validator(input);
    return validator.count_valid();
  };

  BENCHMARK("optimize rules")
  {
    message_validator const validator(input, message_validator::option::optimize_rules);
    return validator.count_valid();
  };
}

TEST_CASE("aoc 2020-19-2 benchmarks")
{
  auto input_path = data_dir / "19-small.txt";
  REQUIRE(std::filesystem::exists(input_path));
  std::ifstream input{ input_path };
  BENCHMARK("no flags")
  {
    message_validator const validator(input);
    return validator.count_valid();
  };

  BENCHMARK("optimize rules")
  {
    message_validator const validator(input, message_validator::option::optimize_rules);
    return validator.count_valid();
  };
}
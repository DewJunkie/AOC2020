#include <catch2/catch.hpp>
#include <aoc/message_validator.h>

#include <fstream>
#include <tuple>
#include <sstream>

TEST_CASE("aoc 2020-19-1")
{
  using namespace std::string_literals;
  const auto& [expected, path] = GENERATE(
    std::make_tuple(2, "19-simple.txt"),
    std::make_tuple(3, "19-small.txt"),
    std::make_tuple(156, "19.txt"));

  GIVEN(path)
  {
    std::ifstream input{ R"(D:\src\AdventOfCode\aoc2020cpp\test\data\)"s + path };
    
    AND_GIVEN("a message validator")
    {
      message_validator const validator ( input );

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
#include "../test_constants.h"
#include <aoc2015/day1.h>

#include <tuple>

namespace aoc2015::day1 {

  TEST_CASE("aoc2015 day 1-1 Find Santas floor")
  {
    using namespace std::string_literals;
    auto [expected, input_type, test_data] = GENERATE(
      std::make_tuple(0, input_type::test_data, "(())"s),
      std::make_tuple(3, input_type::test_data, "))((((("),
      std::make_tuple(74, input_type::file_path, "2015-1.txt"));

    SECTION(test_data)
    {
      auto input = get_input(input_type, test_data);

      auto floor = find_floor(*input);
      REQUIRE(expected == floor);
    }
  }
  TEST_CASE("aoc2015 day 1-2 basement index")
  {
    using namespace std::string_literals;
    auto [expected, input_type, test_data] = GENERATE(
      std::make_tuple(1, input_type::test_data, ")"s),
      std::make_tuple(5, input_type::test_data, "()())"),
      std::make_tuple(1795, input_type::file_path, "2015-1.txt"));

    SECTION(test_data)
    {
      auto input = get_input(input_type, test_data);

      auto floor = find_basement_index(*input);
      REQUIRE(expected == floor);
    }
  }
}// namespace aoc2015

#include "../test_constants.h"

#include <tuple>
#include <algorithm>
#include <array>

namespace aoc2015::day2 {
int calculate_wrapping_paper(std::istream &input)
{
  int length, width, height;
  int total_order = 0;
  while ((input >> length).good()) {
    input.get();
    input >> width;
    input.get();
    input >> height;
    const auto area1 = length * width;
    const auto area2 = length * height;
    const auto area3 = width * height;
    total_order += 2 * area1 + 2 * area2 + 2 * area3 + std::min({ area1, area2, area3 });
  }
  return total_order;
}
int calculate_ribbon(std::istream &input)
{
  std::array<int, 3> dimensions;
  int total_order = 0;
  while ((input >> dimensions[0]).good()) {
    input.get();
    input >> dimensions[1];
    input.get();
    input >> dimensions[2];
    std::sort(dimensions.begin(), dimensions.end());
    total_order += 2 * dimensions[0] + 2 * dimensions[1] + dimensions[0] * dimensions[1] * dimensions[2];
  }
  return total_order;
}

TEST_CASE("aoc2015 day 2-1")
{
  using namespace std::string_literals;
  auto [expected, input_type, test_data] = GENERATE(
    std::make_tuple(58, input_type::test_data, "2x3x4"s),
    std::make_tuple(43, input_type::test_data, "1x1x10"s),
    std::make_tuple(43 + 58, input_type::test_data, "2x3x4 1x1x10"s),
    std::make_tuple(1586300, input_type::file_path, "2015-2.txt"s));

  SECTION(test_data)
  {
    auto input = get_input(input_type, test_data);

    auto order = calculate_wrapping_paper(*input);
    REQUIRE(expected == order);
  }
}
TEST_CASE("aoc2015 day 2-2")
{
  using namespace std::string_literals;
  auto [expected, input_type, test_data] = GENERATE(
    std::make_tuple(34, input_type::test_data, "2x3x4"s),
    std::make_tuple(14, input_type::test_data, "1x1x10"s),
    std::make_tuple(3737498, input_type::file_path, "2015-2.txt"s));

  SECTION(test_data)
  {
    auto input = get_input(input_type, test_data);

    auto ribbon_order = calculate_ribbon(*input);
    REQUIRE(expected == ribbon_order);
  }
}
}// namespace aoc2015::day2
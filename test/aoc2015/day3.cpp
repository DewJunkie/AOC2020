#include "../test_constants.h"

#include <tuple>
#include <utility>
#include <unordered_set>
#include <functional>

namespace std {
template<>
struct hash<std::pair<int,int>>
{
  std::size_t operator()(std::pair<int,int> const &p) const noexcept
  {
    std::size_t h1 = std::hash<int>{}(p.first);
    std::size_t h2 = std::hash<int>{}(p.second);
    return h1 ^ (h2 << 1);// or use boost::hash_combine
  }
};
}// namespace std


namespace aoc2015::day3 {
size_t count_delivered(std::istream& input, int santa_count = 1)
{
  std::unordered_set<std::pair<int, int>> visited{};
  std::vector<std::pair<int, int>> santas(santa_count);
  visited.emplace(0, 0);
  char instruction;
  auto current_santa = santas.begin();
  while ((input >> instruction).good()) {
    switch (instruction) {
    case '^':
      ++current_santa->second;
      break;
    case 'v':
      --current_santa->second;
      break;
    case '<':
      --current_santa->first;
      break;
    case '>':
      ++current_santa->first;
      break;
      default:
      throw std::exception("unknown direction");
    }
    visited.emplace(current_santa->first, current_santa->second);
    ++current_santa;
    if (current_santa == santas.end()) {
      current_santa = santas.begin();
    }
  }
  return visited.size();
}

TEST_CASE("aoc2015 day 3-1")
{
  using namespace std::string_literals;
  auto [expected, input_type, test_data] = GENERATE(
    std::make_tuple(2, input_type::test_data, ">"s),
    std::make_tuple(4, input_type::test_data, "^>v<"s),
    std::make_tuple(2, input_type::test_data, "^v^v^v^v^v"s),
    std::make_tuple(2572, input_type::file_path, "2015-3.txt"s));

  SECTION(test_data)
  {
    auto input = get_input(input_type, test_data);
    REQUIRE(expected == count_delivered(*input));
    
  }
}
TEST_CASE("aoc2015 day 3-2")
{
  using namespace std::string_literals;
  auto [expected, input_type, test_data] = GENERATE(
    std::make_tuple(3, input_type::test_data, "^v"s),
    std::make_tuple(3, input_type::test_data, "^>v<"s),
    std::make_tuple(11, input_type::test_data, "^v^v^v^v^v"s),
    std::make_tuple(2631, input_type::file_path, "2015-3.txt"s));

  SECTION(test_data)
  {
    auto input = get_input(input_type, test_data);
    REQUIRE(expected == count_delivered(*input, 2));
  }
}
}// namespace aoc2015::day2
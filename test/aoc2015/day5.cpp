#include "../test_constants.h"

#include <algorithm>
#include <string_view>
#include <iostream>
#include <unordered_map>

bool is_nice(std::string &data)
{
  std::string_view vowels{ "aeiou" };
  auto vowel_count = 0;
  auto doubles = false;

  auto last = 0;
  for(auto& c: data) {
    if (last == 'a' && c == 'b') return false;
    if (last == 'c' && c == 'd') return false;
    if (last == 'p' && c == 'q') return false;
    if (last == 'x' && c == 'y') return false;
    if (c == last) doubles = true;
    if (std::any_of(vowels.begin(), vowels.end(), [c](char v){return v == c;})) {
      ++vowel_count;
    }
    last = c;
  }
  return doubles && vowel_count > 2;
}

bool is_nice2(std::string& data)
{
  std::unordered_multimap<int, size_t> pair_index;
  bool found_repeat = false;
  bool found_pair = false;
  for(size_t i = 1; i < data.size(); ++i) {
    if (i > 1 && data[i] == data[i - 2]) found_repeat = true;

    const auto pair = (data[i - 1] << 8) + data[i];
    auto [found_begin, found_end] = pair_index.equal_range(pair);
    for (; found_begin != found_end; ++found_begin) {
      if (found_begin->second < i - 1) found_pair = true;
    }

    if (found_pair 
        && found_repeat) return true;

    pair_index.emplace(pair, i);
  }
  return false;
}

namespace aoc2015::day5 {

TEST_CASE("aoc2015 day 5-1")
{
  using namespace std::string_literals;
  auto& [expected, input_type, test_data] = GENERATE(
    std::make_tuple(1, input_type::test_data, "ugknbfddgicrmopn "s),
    std::make_tuple(1, input_type::test_data, "aaa"),
    std::make_tuple(0, input_type::test_data, "jchzalrnumimnmhp"),
    std::make_tuple(0, input_type::test_data, "haegwjzuvuyypxyu"),
    std::make_tuple(0, input_type::test_data, "dvszwmarrgswjxmb"),
    std::make_tuple(236, input_type::file_path, "2015-5.txt"));

  SECTION(test_data)
  {
    auto input = get_input(input_type, test_data);

    std::string word;
    auto count = 0;
    while (!(*input >> word).fail()) {
      if (is_nice(word)) ++count;
    }
    REQUIRE(expected == count);
  }
}
TEST_CASE("aoc2015 day 5-2")
{
  using namespace std::string_literals;
  auto &[expected, input_type, test_data] = GENERATE(
    std::make_tuple(1, input_type::test_data, "qjhvhtzxzqqjkmpb"s),
    std::make_tuple(1, input_type::test_data, "xxyxx"),
    std::make_tuple(1, input_type::test_data, "aaaa"),
    std::make_tuple(0, input_type::test_data, "uurcxstgmygtbstg"),
    std::make_tuple(0, input_type::test_data, "ieodomkazucvgmuy"),
    std::make_tuple(236, input_type::file_path, "2015-5.txt"));

  SECTION(test_data)
  {
    auto input = get_input(input_type, test_data);

    std::string word;
    auto count = 0;
    while (!(*input >> word).fail()) {
      if (is_nice2(word)) ++count;
    }
    REQUIRE(expected == count);
  }
}
}// namespace aoc2015::day5
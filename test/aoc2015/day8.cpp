#include "../test_constants.h"

#include <tuple>
#include <sstream>

namespace aoc2015::day1 {

TEST_CASE("aoc2015 day 8-1")
{
  using namespace std::string_literals;
  auto [expected, input_type, test_data] = GENERATE(
    std::make_tuple(2, input_type::test_data, R"str("")str")
      , std::make_tuple(2, input_type::test_data, R"str("abc")str")
      , std::make_tuple(3, input_type::test_data, R"str("aaa\"aaa")str")
      , std::make_tuple(5, input_type::test_data, R"str("\x27")str")
      , std::make_tuple(4, input_type::test_data, R"str("kali\\zczhiqkqzybjj\"fgdjnik")str")
      , std::make_tuple(1371, input_type::file_path, "2015-8.txt")
  );

  SECTION(test_data)
  {
    auto input = get_input(input_type, test_data);

    auto code_size = 0;
    auto mem_size = 0;
    std::string token;
    std::stringstream message;
    while (!(*input >> token).fail()) {
      code_size -= 2;
      char last = 0;
      for (const auto& c: token) {
        if (last == '\\'){
          switch (c) {
          case '\\':
          case '"':
            code_size -= 1;
            break;
          case 'x':
            code_size -= 3;
            break;
            default:
            message.clear();
              message << last << c << " invalid character escape";
            throw std::invalid_argument(message.str());
          }
            last = 0;
        } else {
          last = c;
        }
        ++code_size;
        ++mem_size;
      }
    }

    REQUIRE(expected == mem_size - code_size);
  }
}

TEST_CASE("aoc2015 day 8-2")
{
  using namespace std::string_literals;
  auto [expected, input_type, test_data] = GENERATE(
    std::make_tuple(4, input_type::test_data, R"str("")str")
      , std::make_tuple(4, input_type::test_data, R"str("abc")str")
      , std::make_tuple(6, input_type::test_data, R"str("aaa\"aaa")str")
      , std::make_tuple(5, input_type::test_data, R"str("\x27")str")
      , std::make_tuple(19, input_type::test_data, R"str("" "abc" "aaa\"aaa" "\x27")str")
      , std::make_tuple(2117, input_type::file_path, "2015-8.txt")
  );

  SECTION(test_data)
  {
    auto input = get_input(input_type, test_data);

    int encoded_size = 0;
    std::string token;
    std::stringstream message;
    while (!(*input >> token).fail()) {
      encoded_size += 2;      
      for (const auto& c: token) {
        switch (c) {
        case '"':
        case '\\':
          ++encoded_size;
        }
      }
    }

    REQUIRE(expected == encoded_size);
  }
}

}// namespace aoc2015::day1

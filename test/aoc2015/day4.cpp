#include "../test_constants.h"

#include <openssl/md5.h>
#include <array>
#include <charconv>
#include <fmt/format.h>
namespace aoc2015::day4 {
int mine(std::string seed, unsigned char max_3rd = 0x0f)
{
  MD5_CTX state;
  MD5_Init(&state);
  MD5_Update(&state, seed.data(), seed.size());

  std::array<unsigned char, MD5_DIGEST_LENGTH> md;
  for (int i = 0; i < 10'000'000; ++i) {
    auto this_state = state;
    auto counter = fmt::format("{}", i);
    MD5_Update(&this_state, counter.data(), counter.size());
    MD5_Final(md.data(), &this_state);
    if (md[0] == 0 && md[1] == 0 && md[2] <= max_3rd) {
      CAPTURE(md);
      return i;
    }
  }
  return 0;
}

int mine_(std::string seed, unsigned char max_3rd = 0x0f)
{
  MD5_CTX state;
  MD5_Init(&state);
  MD5_Update(&state, seed.data(), seed.size());

  std::array<unsigned char, 10> data;
  size_t len = 1;

  std::array<unsigned char, MD5_DIGEST_LENGTH> md;
  int i = 0;
  for (unsigned char digit1 = '0'; digit1 <= '9'; ++digit1) {
    for (unsigned char digit2 = '0'; digit2 <= '9'; ++digit2) {
      for (unsigned char digit3 = '0'; digit3 <= '9'; ++digit3) {
        for (unsigned char digit4 = '0'; digit4 <= '9'; ++digit4) {
          for (unsigned char digit5 = '0'; digit5 <= '9'; ++digit5) {
            for (unsigned char digit6 = '0'; digit6 <= '9'; ++digit6) {
              for (unsigned char digit7 = '0'; digit7 <= '9'; ++digit7) {
                for (unsigned char digit8 = '0'; digit8 <= '9'; ++digit8) {
                  auto this_state = state;
                  if (len > 7) data[len - 8] = digit1;
                  if (len > 6) data[len - 7] = digit2;
                  if (len > 5) data[len - 6] = digit3;
                  if (len > 4) data[len - 5] = digit4;
                  if (len > 3) data[len - 4] = digit5;
                  if (len > 2) data[len - 3] = digit6;
                  if (len > 1) data[len - 2] = digit7;
                  data[len - 1] = digit8;
                  MD5_Update(&this_state, data.data(), len);
                  MD5_Final(md.data(), &this_state);
                  if (md[0] == 0 && md[1] == 0 && md[2] <= max_3rd) {
                    CAPTURE(md);
                    return i;
                  }
                  ++i;
                }
                if (len == 1) len = 2;
              }
              if (len == 2) ++len;
            }
            if (len == 3) ++len;
          }
          if (len == 4) ++len;
        }
        if (len == 5) ++len;
      }
      if (len == 6) ++len;
    }
    if (len == 7) ++len;
  }
  return 0;
}

TEST_CASE("aoc2015 day 4-1")
{
  using namespace std::string_literals;
  auto [expected, input_type, test_data] = GENERATE(
    std::make_tuple(609'043, input_type::test_data, "abcdef"s),
    std::make_tuple(1'048'970, input_type::test_data, "pqrstuv"s),
    std::make_tuple(254'575, input_type::test_data, "bgvyzdsv"s));

  SECTION(test_data)
  {
    //auto input = get_input(input_type, test_data);
    REQUIRE(expected == mine(test_data));
  }
}
TEST_CASE("aoc2015 day 4-1_")
{
  using namespace std::string_literals;
  auto [expected, input_type, test_data] = GENERATE(
    std::make_tuple(609'043, input_type::test_data, "abcdef"s),
    std::make_tuple(1'048'970, input_type::test_data, "pqrstuv"s),
    std::make_tuple(254'575, input_type::test_data, "bgvyzdsv"s));

  SECTION(test_data)
  {
    //auto input = get_input(input_type, test_data);
    REQUIRE(expected == mine_(test_data));
  }
}
TEST_CASE("aoc2015 day 4-2")
{
  using namespace std::string_literals;
  auto [expected, input_type, test_data] = GENERATE(
    std::make_tuple(6'742'839, input_type::test_data, "abcdef"s),
    std::make_tuple(5'714'438, input_type::test_data, "pqrstuv"s),
    std::make_tuple(1'038'736, input_type::test_data, "bgvyzdsv"s));

  SECTION(test_data)
  {
    //auto input = get_input(input_type, test_data);
    REQUIRE(expected == mine(test_data, 0));
  }
}
TEST_CASE("aoc2015 day 4-2_")
{
  using namespace std::string_literals;
  auto [expected, input_type, test_data] = GENERATE(
    std::make_tuple(6'742'839, input_type::test_data, "abcdef"s),
    std::make_tuple(5'714'438, input_type::test_data, "pqrstuv"s),
    std::make_tuple(1'038'736, input_type::test_data, "bgvyzdsv"s));

  SECTION(test_data)
  {
    //auto input = get_input(input_type, test_data);
    REQUIRE(expected == mine_(test_data, 0));
  }
}
}
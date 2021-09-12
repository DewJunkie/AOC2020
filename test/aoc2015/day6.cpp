#include "../test_constants.h"
#include <iostream>
#include <string>
#include <vector>
#include <fmt/core.h>

namespace aoc2015::day6 {
  class christmas
  {
  public:
    christmas(short dimension)
      : _dimension(dimension)
      , lights(dimension * dimension, 0)
    {
    }

    void run(std::istream &program)
    {
      action action{ action::invalid };
      std::string token;
      while (!(program >> token).fail()) {
        if (token == "turn") {
          program >> token;
          if (token == "on") {
            action = action::on;
          } else if (token == "off") {
            action = action::off;
          }
        } else if (token == "toggle") {
          action = action::toggle;
        }

        if (action == action::invalid) {
          throw std::invalid_argument("invalid instruction");
        }

        int x1, y1, x2, y2;
        char comma;
        program >> x1;
        program >> comma; //,
        program >> y1;
        program >> token; // through
        program >> x2;
        program >> comma;//,
        program >> y2;

        if (program.fail()) {
          throw std::invalid_argument("incomplete instruction");
        }

        if (x1 > x2) { std::swap(x1, x2); }
        if (y1 > y2) { std::swap(y1, y2); }

        // Check bounds
        if (std::min({x1, y1, x2, y2}) < 0
            || std::max({x1, y1, x2, y2}) >= _dimension) {
          throw std::invalid_argument("coordinate out of range");
        }


        for (int y = y1; y <= y2; ++y)
          for (int x = x1; x <= x2; ++x) {
            set_light(x, y, action);
          }
        action = action::invalid;
      }
    }

    size_t on_count() const {
      //return std::count_if(lights.begin(), lights.end(), [](bool l) { return l; };
      return std::accumulate(lights.begin(), lights.end(), 0);
    }

  private:
    enum class action { invalid,
      on,
      off,
      toggle
    };
    short _dimension;
    std::vector<uint8_t> lights;

    void set_light(int x, int y, action value)
    {
      const auto offset = y * _dimension + x;
      if (value == action::on)
        lights[offset]++;
      else if (value == action::off && lights[offset] > 0)
        lights[offset]--;
      else if (value == action::toggle)
        lights[offset]+=2;
    }
  };

TEST_CASE("aoc2015 day 6-1")
  {
    using namespace std::string_literals;
    auto &[expected, input_type, test_data] = GENERATE(
        std::make_tuple(1, input_type::test_data, "turn on 0,0 through 0,0")
      , std::make_tuple(18, input_type::test_data, "toggle 0,1 through 2,3")
      , std::make_tuple(18, input_type::test_data, "toggle 2,3 through 0,1")
      , std::make_tuple(18, input_type::test_data, "toggle 0,3 through 2,1")
      , std::make_tuple(15, input_type::test_data, "turn on 0,0 through 2,2\ntoggle 0,0 through 0,2")
      , std::make_tuple(0, input_type::test_data, "turn off 0,0 through 999,999")
      , std::make_tuple(1'000'000, input_type::test_data, "turn on 0,0 through 999,999")
      , std::make_tuple(17836115, input_type::file_path, "2015-6.txt")
    );

    SECTION(test_data)
    {
      auto input = get_input(input_type, test_data);

      christmas _christmas(1000);
      _christmas.run(*input);
      REQUIRE(expected == _christmas.on_count());
    }
  }

}
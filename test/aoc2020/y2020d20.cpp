#include "../test_constants.h"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>
#include <cassert>
#include <optional>
#include <stack>

namespace aoc2020::day20 {
enum class border
{
  top,
  top_reverse,
  bottom,
  bottom_reverse,
  left,
  left_reverse,
  right,
  right_reverse,
  border_count
};
using border_t = unsigned short;
struct image
{
  std::vector<std::string> rows;
  std::array<border_t, static_cast<size_t>(border::border_count)> borders {};

  void add_borders()
  {
    borders = {};
    for (int i = 0; i < rows.size(); ++i) {
      assert(rows.size() == rows[i].size());
      assert(rows.size() < sizeof(decltype(borders)::value_type)*8);
      for (auto& border: borders) {
        border <<= 1;
      }
      if (rows.front()[i] == '#') borders[static_cast<size_t>(border::top)] += 1;
      if (*(rows.front().rbegin() + i) == '#') borders[static_cast<size_t>(border::top_reverse)] += 1;

      if (rows.back()[i] == '#') borders[static_cast<size_t>(border::bottom)] += 1;
      if (*(rows.back().rbegin() + i) == '#') borders[static_cast<size_t>(border::bottom_reverse)] += 1;

      if (rows[i].front() == '#') borders[static_cast<size_t>(border::left)] += 1;
      if ((rows.rbegin() + i)->front() == '#') borders[static_cast<size_t>(border::left_reverse)] += 1;

      if (rows[i].back() == '#') borders[static_cast<size_t>(border::right)] += 1;
      if ((rows.rbegin() + i)->back() == '#') borders[static_cast<size_t>(border::right_reverse)] += 1;
    }
  }

  friend std::ostream &operator<<(std::ostream& stream, const image& this_)
  {
    for (auto& row: this_.rows) {
      stream << row << "\n";
    }
    return stream;
  }
};

enum class transformation
{
  none,
  rotate_clockwise,
  rotate_counter_clockwise,
  flip_horizontal,
  flip_vertical,
};

struct tile
{
  tile(const image& image)
    : left(nullptr)
  , right(nullptr)
  , up(nullptr)
  , down(nullptr)
  , image(image)
  , borders(image.borders)
  {}
  tile *left;
  tile *right;
  tile *up;
  tile *down;
  const image &image;
  
  std::array<border_t, static_cast<size_t>(border::border_count)> borders;

  void transform(transformation transformation)
  {
    transforms.emplace_back(transformation);
    using namespace std;
    switch (transformation) {
    case transformation::rotate_clockwise: {
      const auto top = borders[static_cast<size_t>(border::top)];
      const auto top_reverse = borders[static_cast<size_t>(border::top_reverse)];

      borders[static_cast<size_t>(border::top)] = borders[static_cast<size_t>(border::left_reverse)];
      borders[static_cast<size_t>(border::top_reverse)] = borders[static_cast<size_t>(border::left)];

      borders[static_cast<size_t>(border::left)] = borders[static_cast<size_t>(border::bottom)];
      borders[static_cast<size_t>(border::left_reverse)] = borders[static_cast<size_t>(border::bottom_reverse)];

      borders[static_cast<size_t>(border::bottom)] = borders[static_cast<size_t>(border::right_reverse)];
      borders[static_cast<size_t>(border::bottom_reverse)] = borders[static_cast<size_t>(border::right)];

      borders[static_cast<size_t>(border::right)] = top;
      borders[static_cast<size_t>(border::right_reverse)] = top_reverse;
    } break;
    case transformation::rotate_counter_clockwise: {
      const auto top = borders[static_cast<size_t>(border::top)];
      const auto top_reverse = borders[static_cast<size_t>(border::top_reverse)];

      borders[static_cast<size_t>(border::top)] = borders[static_cast<size_t>(border::right)];
      borders[static_cast<size_t>(border::top_reverse)] = borders[static_cast<size_t>(border::right_reverse)];

      borders[static_cast<size_t>(border::right)] = borders[static_cast<size_t>(border::bottom_reverse)];
      borders[static_cast<size_t>(border::right_reverse)] = borders[static_cast<size_t>(border::bottom)];

      borders[static_cast<size_t>(border::bottom)] = borders[static_cast<size_t>(border::left)];
      borders[static_cast<size_t>(border::bottom_reverse)] = borders[static_cast<size_t>(border::left_reverse)];

      borders[static_cast<size_t>(border::left)] = top_reverse;
      borders[static_cast<size_t>(border::left_reverse)] = top;
    } break;
    case transformation::flip_horizontal:
      swap(borders[static_cast<size_t>(border::left)], borders[static_cast<size_t>(border::right)]);
      swap(borders[static_cast<size_t>(border::left_reverse)], borders[static_cast<size_t>(border::right_reverse)]);
      swap(borders[static_cast<size_t>(border::top)], borders[static_cast<size_t>(border::top_reverse)]);
      swap(borders[static_cast<size_t>(border::bottom)], borders[static_cast<size_t>(border::bottom_reverse)]);
      break;
    case transformation::flip_vertical:
      swap(borders[static_cast<size_t>(border::top)], borders[static_cast<size_t>(border::bottom)]);
      swap(borders[static_cast<size_t>(border::top_reverse)], borders[static_cast<size_t>(border::bottom_reverse)]);
      swap(borders[static_cast<size_t>(border::left)], borders[static_cast<size_t>(border::left_reverse)]);
      swap(borders[static_cast<size_t>(border::right)], borders[static_cast<size_t>(border::right_reverse)]);
      break;
    }
  }

  friend std::ostream &operator<<(std::ostream& out, const tile& this_)
  {
    std::vector<std::string> rows(this_.image.rows);
    for (auto& transform: this_.transforms) {
      std::vector<std::string> temp;

      switch (transform) {
      case transformation::rotate_clockwise:
        out << "rotate_clockwise\n";
        for (size_t y = 0; y < rows.size(); ++y) {
          temp.emplace_back();
          for (size_t x = 0; x < rows[y].size(); ++x) {
            temp.back().push_back(rows[rows.size() - x - 1][y]);
          }
        }
        using namespace std;
        swap(temp, rows);

        break;
      case transformation::rotate_counter_clockwise:
        out << "rotate_counter_clockwise\n";
        for (size_t y = 0; y < rows.size(); ++y) {
          temp.emplace_back();
          for (size_t x = 0; x < rows[y].size(); ++x) {
            temp.back().push_back(rows[x][rows[x].size() - y - 1]);
          }
        }
        using namespace std;
        swap(temp, rows);

        break;
      case transformation::flip_horizontal:
        out << "flip_horizontal\n";
        for (auto&row: rows) {
          std::reverse(row.begin(), row.end());
        }

        break;
      case transformation::flip_vertical:
        out << "flip_vertical\n";
        std::reverse(rows.begin(), rows.end());

        break;
      case transformation::none:
      default: ;
      }
    }

    for (auto& row: rows) {
      out << row << "\n";
    }

    return out;
  }
protected:
  std::vector<transformation> transforms;
};
struct assembler
{
  assembler(std::istream &input)
  {
    std::string temp;
    std::string row;
    int tile_id{};
    image current;

    auto push_image = [&]() {
      if (!current.rows.empty()) {
        current.add_borders();
        images[tile_id] = std::move(current);
        current.rows.clear();  // NOLINT(bugprone-use-after-move)
      }
    };

    while (input.good()) {
      input >> temp;
      if (temp == "Tile") {
        push_image();
        input >> tile_id;
        input.get();
      } else {
        current.rows.push_back(temp);
      }
    }
    push_image();
  }

  std::optional<int> assemble()
  {
    std::vector<tile> solved;
    solved.reserve(images.size());
    solved.emplace_back(images.begin()->second);

    std::stack<decltype(solved)::const_iterator> work;
    work.push(solved.begin()+1);

    while (!work.empty()) {
      auto end = work.top();
      work.pop();

      for (auto begin = solved.begin(); begin != end; ++begin) {
        // try left
      }
    }

    return {};
  }

  friend std::ostream &operator<<(std::ostream& out, const assembler& this_)
  {
    for (auto& image: this_.images) {
      out << "Tile " << image.first << ":\n"
                << image.second
                << "\n";
    }
    return out;
  }

  std::unordered_map<int, image> images;
};
TEST_CASE("test transformations")
{
  std::stringstream input(R"(Tile 0:
###
.#.
##.)");
  assembler assembler(input);
  REQUIRE(assembler.images[0].rows.size() == 3);
  REQUIRE(assembler.images[0].rows[0].size() == 3);
  REQUIRE(assembler.images[0].borders[static_cast<size_t>(border::top)] == 7);
  REQUIRE(assembler.images[0].borders[static_cast<size_t>(border::top_reverse)] == 7);
  REQUIRE(assembler.images[0].borders[static_cast<size_t>(border::bottom)] == 6);
  REQUIRE(assembler.images[0].borders[static_cast<size_t>(border::bottom_reverse)] == 3);
  REQUIRE(assembler.images[0].borders[static_cast<size_t>(border::left)] == 5);
  REQUIRE(assembler.images[0].borders[static_cast<size_t>(border::left_reverse)] == 5);
  REQUIRE(assembler.images[0].borders[static_cast<size_t>(border::right)] == 4);
  REQUIRE(assembler.images[0].borders[static_cast<size_t>(border::right_reverse)] == 1);

  INFO(assembler);
  tile tile(assembler.images[0]);
  auto original_tile{ tile };

  bool should_pass = true;
  SECTION("flip_horizontal")
  {
    INFO("original\n"
         << tile);
    tile.transform(transformation::flip_horizontal);
    INFO("transformed\n"
        << tile);
    
    CHECK(should_pass);
    REQUIRE(tile.borders[static_cast<size_t>(border::left)] == original_tile.borders[static_cast<size_t>(border::right)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::left_reverse)] == original_tile.borders[static_cast<size_t>(border::right_reverse)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::right)] == original_tile.borders[static_cast<size_t>(border::left)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::right_reverse)] == original_tile.borders[static_cast<size_t>(border::left_reverse)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::top)] == original_tile.borders[static_cast<size_t>(border::top_reverse)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::top_reverse)] == original_tile.borders[static_cast<size_t>(border::top)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::bottom)] == original_tile.borders[static_cast<size_t>(border::bottom_reverse)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::bottom_reverse)] == original_tile.borders[static_cast<size_t>(border::bottom)]);
  }
  SECTION("flip_vertical")
  {
    INFO("original\n"
         << tile);
    tile.transform(transformation::flip_vertical);
    INFO("transformed\n"
         << tile);
    CHECK(should_pass);
    REQUIRE(tile.borders[static_cast<size_t>(border::left)] == original_tile.borders[static_cast<size_t>(border::left_reverse)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::left_reverse)] == original_tile.borders[static_cast<size_t>(border::left)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::right)] == original_tile.borders[static_cast<size_t>(border::right_reverse)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::right_reverse)] == original_tile.borders[static_cast<size_t>(border::right)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::top)] == original_tile.borders[static_cast<size_t>(border::bottom)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::top_reverse)] == original_tile.borders[static_cast<size_t>(border::bottom_reverse)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::bottom)] == original_tile.borders[static_cast<size_t>(border::top)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::bottom_reverse)] == original_tile.borders[static_cast<size_t>(border::top_reverse)]);
  }
  SECTION("rotate_clockwise")
  {
    INFO("original\n"
         << tile);
    tile.transform(transformation::rotate_clockwise);
    INFO("transformed\n"
         << tile);
    CHECK(should_pass);
    REQUIRE(tile.borders[static_cast<size_t>(border::left)] == original_tile.borders[static_cast<size_t>(border::bottom)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::left_reverse)] == original_tile.borders[static_cast<size_t>(border::bottom_reverse)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::right)] == original_tile.borders[static_cast<size_t>(border::top)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::right_reverse)] == original_tile.borders[static_cast<size_t>(border::top_reverse)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::top)] == original_tile.borders[static_cast<size_t>(border::left_reverse)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::top_reverse)] == original_tile.borders[static_cast<size_t>(border::left_reverse)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::bottom)] == original_tile.borders[static_cast<size_t>(border::right_reverse)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::bottom_reverse)] == original_tile.borders[static_cast<size_t>(border::right)]);
    SECTION("rotate_clockwise")
    {
      tile.transform(transformation::rotate_clockwise);
      INFO("transformed\n"
           << tile);
      CHECK(should_pass);
    }
  }
  SECTION("rotate_counter_clockwise")
  {
    INFO("original\n"
         << tile);
    tile.transform(transformation::rotate_counter_clockwise);
    INFO("transformed\n"
         << tile);
    CHECK(should_pass);
    REQUIRE(tile.borders[static_cast<size_t>(border::left)] == original_tile.borders[static_cast<size_t>(border::top_reverse)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::left_reverse)] == original_tile.borders[static_cast<size_t>(border::top)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::right)] == original_tile.borders[static_cast<size_t>(border::bottom_reverse)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::right_reverse)] == original_tile.borders[static_cast<size_t>(border::bottom)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::top)] == original_tile.borders[static_cast<size_t>(border::right)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::top_reverse)] == original_tile.borders[static_cast<size_t>(border::right_reverse)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::bottom)] == original_tile.borders[static_cast<size_t>(border::left)]);
    REQUIRE(tile.borders[static_cast<size_t>(border::bottom_reverse)] == original_tile.borders[static_cast<size_t>(border::left_reverse)]);
    SECTION("rotate_counter_clockwise")
    {
      tile.transform(transformation::rotate_counter_clockwise);
      INFO("transformed\n"
           << tile);
      CHECK(should_pass);
    }
  }
}
TEST_CASE("aoc2020 day 20-1")
{
  using namespace std::string_literals;
  auto &[expected, input_type, test_data] = GENERATE(
    std::make_tuple(1, input_type::file_path, "2020-20.sample.txt"s));

  SECTION(test_data)
  {
    auto input = get_input(input_type, test_data);
    assembler assembler(*input);
    INFO(assembler)

    auto count = 0;
    REQUIRE(expected == count);
  }
}
}

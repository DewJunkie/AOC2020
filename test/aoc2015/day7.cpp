#include "../test_constants.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <fmt/core.h>
#include <charconv>
#include <sstream>
#include <optional>

namespace aoc2015::day7 {
class gate;
using parts_bag_t = std::unordered_map<std::string, std::unique_ptr<gate>>;

class gate
{
public:
  gate() : value(){}
  uint16_t output(parts_bag_t &parts_bag)
  {
    if (value) return value.value();
    value = calculate_output(parts_bag);
    return value.value();
  }
  void reset()
  {
    value.reset();
  }
  virtual ~gate() = default;
protected:
  virtual uint16_t calculate_output(parts_bag_t &parts) const = 0;
  std::optional<uint16_t> value;
};
class throw_gate: public gate
{
public:
  throw_gate(std::vector<std::string> operations)
    : operations_(std::move(operations)) {}

  uint16_t calculate_output(parts_bag_t &) const final
  {
    std::stringstream what;
    what << "not implemented:";
    for (auto &op : operations_) what << " " << op;
    throw std::logic_error(what.str());
  }
private:

  std::vector<std::string> operations_;
};

class const_gate : public gate
{
public:
  const_gate(uint16_t value)
    : value_(value) {  }
  uint16_t calculate_output(parts_bag_t &) const final
  {
    return value_;
  }
  uint16_t value_;
};

parts_bag_t::mapped_type &find(parts_bag_t &parts, const parts_bag_t::key_type &key)
{
  auto found = parts.find(key);
  if (found == parts.end()) {
    uint16_t value;
    auto [ptr, ec]{ std::from_chars(key.data(), key.data() + key.size(), value) };
    if (ec == std::errc{}) {
      return parts.emplace(key, std::make_unique<const_gate>(value)).first->second;
    }
    std::stringstream what;
    what << key << " not found";
    throw std::logic_error(what.str());
  }
  return found->second;
}

class identity_gate : public gate
{
public:
  identity_gate(std::string value)
    : value_(value)
  {}

  uint16_t calculate_output(parts_bag_t &parts_bag) const final
  {
    return find(parts_bag, value_)->output(parts_bag);
  }

private:
  std::string value_;
};

class not_gate : public gate
{
public:
  not_gate(std::string op1)
    : op1_(std::move(op1)) {}

  uint16_t calculate_output(parts_bag_t &parts_bag) const final
  {
    return ~find(parts_bag, op1_)->output(parts_bag);
  }

private:
  std::string op1_;
};

class and_gate: public gate
{
public:
  and_gate(std::string op1, std::string op2)
    : op1_(std::move(op1))
    , op2_(std::move(op2)) {}

  uint16_t calculate_output(parts_bag_t &parts_bag) const final
  {
    return find(parts_bag, op1_)->output(parts_bag) & find(parts_bag, op2_)->output(parts_bag);
  }
private:
  std::string op1_;
  std::string op2_;
};

class or_gate: public gate
{
public:
  or_gate(std::string op1, std::string op2)
    : op1_(std::move(op1)), op2_(std::move(op2)) {}

  uint16_t calculate_output(parts_bag_t &parts_bag) const final
  {
    return find(parts_bag, op1_)->output(parts_bag) | find(parts_bag, op2_)->output(parts_bag);
  }

private:
  std::string op1_;
  std::string op2_;
};

class rshift_gate : public gate
{
public:
  rshift_gate(std::string op1, std::string op2)
    : op1_(std::move(op1)), op2_(std::move(op2)) {}

  uint16_t calculate_output(parts_bag_t &parts_bag) const final
  {
    return find(parts_bag, op1_)->output(parts_bag) >> find(parts_bag, op2_)->output(parts_bag);
  }

private:
  std::string op1_;
  std::string op2_;
};

class lshift_gate : public gate
{
public:
  lshift_gate(std::string op1, std::string op2)
    : op1_(std::move(op1)), op2_(std::move(op2)) {}

  uint16_t calculate_output(parts_bag_t &parts_bag) const final
  {
    return find(parts_bag, op1_)->output(parts_bag) << find(parts_bag, op2_)->output(parts_bag);
  }

private:
  std::string op1_;
  std::string op2_;
};

class circuit
{
public:
  circuit()
  {
  }

  void build(std::istream& instructions)
  {
    std::string token;
    std::vector<std::string> tokens;
    while ((instructions >> token).good()) {
      if (token == "->") {
        instructions >> token;
        if (tokens.size() == 1) {
          components.emplace( token, std::make_unique<identity_gate>(tokens[0]));
        } else if (tokens.size() == 3 && tokens[1] == "AND") {
          components.emplace(token, std::make_unique<and_gate>(tokens[0], tokens[2]));
        } else if (tokens.size() == 3 && tokens[1] == "OR") {
          components.emplace(token, std::make_unique<or_gate>(tokens[0], tokens[2]));
        } else if (tokens.size() == 3 && tokens[1] == "RSHIFT") {
          components.emplace(token, std::make_unique<rshift_gate>(tokens[0], tokens[2]));
        } else if (tokens.size() == 3 && tokens[1] == "LSHIFT") {
          components.emplace(token, std::make_unique<lshift_gate>(tokens[0], tokens[2]));
        } else if (tokens.size() == 2 && tokens[0] == "NOT") {
          components.emplace(token, std::make_unique<not_gate>(tokens[1]));
        } else {
          components.emplace(token, std::make_unique<throw_gate>(std::move(tokens)));
        }
        tokens.clear();
      } else {
        tokens.push_back(std::move(token));
      }
    }
  }

  uint16_t probe(const std::string& component_name)
  {
    return components.at(component_name)->output(components);
  }

  uint16_t probe_2(const std::string& component_name, const std::string& replace_name)
  {
    auto test = components.at("b")->output(components);
    auto value = components.at(component_name)->output(components);
    components.at(replace_name) = std::make_unique<const_gate>(value);
    test = components.at("b")->output(components);
    for (auto& component: components) {
      component.second->reset();
    }
    test = components.at("b")->output(components);
    return components.at(component_name)->output(components);
  }

private:
  parts_bag_t components;
};
TEST_CASE("aoc2015 day 7-1")
{
  using namespace std::string_literals;
  auto &[expected, input_type, test_data] = GENERATE(
    std::make_tuple(123, input_type::test_data, "123 -> a")
      , std::make_tuple(0, input_type::test_data, "1 -> x   0 -> y  x AND y -> a")
      , std::make_tuple(1, input_type::test_data, "1 -> x   1 -> y  x AND y -> a")
      , std::make_tuple(0, input_type::test_data, "0 -> x   1 -> y  x AND y -> a")
      , std::make_tuple(0, input_type::test_data, "0 -> x   0 -> y  x AND y -> a")
      , std::make_tuple(1, input_type::test_data, "1 -> x   0 -> y  x OR y -> a")
      , std::make_tuple(1, input_type::test_data, "1 -> x   1 -> y  x OR y -> a")
      , std::make_tuple(1, input_type::test_data, "0 -> x   1 -> y  x OR y -> a")
      , std::make_tuple(0, input_type::test_data, "0 -> x   0 -> y  x OR y -> a")
      , std::make_tuple(956, input_type::file_path, "2015-7.txt")
  );

  SECTION(test_data)
  {
    auto input = get_input(input_type, test_data);
    circuit circuit;
    circuit.build(*input);

    REQUIRE(expected == circuit.probe("a"));
  }
}

TEST_CASE("validate optional")
{
  std::optional<int> x;

  REQUIRE_FALSE(x);
  x = 1;
  REQUIRE(x);
  x.reset();
  REQUIRE_FALSE(x);

}

TEST_CASE("aoc2015 day 7-2")
{
  using namespace std::string_literals;
  auto &[expected, input_type, test_data] = GENERATE(
    std::make_tuple(40149, input_type::file_path, "2015-7.txt")
  );

  SECTION(test_data)
  {
    auto input = get_input(input_type, test_data);
    circuit circuit;
    circuit.build(*input);

    REQUIRE(expected == circuit.probe_2("a", "b"));
  }
}

}// namespace aoc2015::day6
#pragma once

#include <istream>
#include <memory>

#include <util/flag.hpp>

/**
 * \brief https://adventofcode.com/2020/day/19
 */
class message_validator
{
public:
  enum struct option : uint8_t {
    none = 0,
    optimize_rules = 1 << 0,
    day2 = 1 << 1,
    print_rules = 1 << 2,
    print_stats = 1 << 3
  };

  explicit message_validator(std::istream& setup, flag<option> options = option::none);

  /**
   * \brief process the rules provided during construction
   * \return number of valid messages
   */
  [[nodiscard]] int count_valid() const;

private:
  // ReSharper disable once IdentifierTypo
  flag<option> options_;
  struct impl;
  std::unique_ptr<impl, void (*)(impl const*)> impl_;
};

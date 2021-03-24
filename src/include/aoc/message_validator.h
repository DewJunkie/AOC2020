#pragma once

#include <istream>
#include <memory>

/**
 * \brief https://adventofcode.com/2020/day/19
 */
class message_validator
{
public:
  explicit message_validator(std::istream& setup);

  /**
   * \brief process the rules provided during construction
   * \return number of valid messages
   */
  [[nodiscard]] int count_valid() const;

private:
  // ReSharper disable once IdentifierTypo
  struct impl;
  std::unique_ptr<impl, void (*)(impl const*)> impl_;
};

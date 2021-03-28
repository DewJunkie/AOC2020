#pragma once

#include <cstdint>
#include <initializer_list>

/**
 * \brief wrapper for a flag enum
 * \tparam T Type of enum to wrap
 * \tparam TBase Type to use for arithmetic
 */
template<typename T, typename TBase = uint32_t>
struct flag
{
  //using T = my_enum;
  //using TBase = uint32_t;

  constexpr flag(const T value)
    : value_(value)
  {}
  constexpr flag(std::initializer_list<T> values)
    : flag(*values.begin())
  {
    for (const auto *value = values.begin() + 1; value != values.end(); ++value) {
      this->operator|=(*value);
    }
  }
  constexpr flag(std::initializer_list<flag> values)
    : value_(values.begin()->value_)
  {
    for (const auto *value = values.begin() + 1; value != values.end(); ++value) {
      this->operator|=(*value);
    }
  }

  [[nodiscard]] friend bool operator==(const flag &lhs, const flag &rhs)
  {
    return lhs.value_ == rhs.value_;
  }
  [[nodiscard]] friend bool operator!=(const flag &lhs, const flag &rhs)
  {
    return !(lhs == rhs);
  }
  [[nodiscard]] bool has(const T &that) const noexcept
  {
    return (*this & that) == that;
  }


  flag &operator|=(const flag &rhs)
  {
    value_ = static_cast<T>(static_cast<TBase>(value_) | static_cast<TBase>(rhs.value_));
    return *this;
  }
  [[nodiscard]] friend flag operator|(flag lhs, const flag &rhs)
  {
    lhs |= rhs;
    return lhs;
  }

  flag &operator&=(const flag &rhs)
  {
    value_ = static_cast<T>(static_cast<TBase>(value_) & static_cast<TBase>(rhs.value_));
    return *this;
  }
  [[nodiscard]] friend flag operator&(flag lhs, const flag &rhs)
  {
    lhs &= rhs;
    return lhs;
  }

protected:
  T value_;
};
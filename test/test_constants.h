#pragma once
#pragma warning(push, 1)
#include <catch2/catch.hpp>
#pragma warning(pop)
#include <filesystem>
#include <sstream>
#include <fstream>


const std::filesystem::path data_dir( R"(D:\src\AdventOfCode\aoc2020cpp\test\data)");

enum class input_type
{
  file_path,
  test_data
};

/**
 * \brief if test_data is a path return an fstream to read in the data in that file, otherwise create
 * an istrinstream to read the data from the passed in string.
 * \param test_data either test data or a path to a file containing test data.
 * \return istream to test data 
 */
inline auto get_input(const input_type input_type, const std::string &test_data)
{
  std::unique_ptr<std::istream> input;

  if (input_type == input_type::file_path) {
    REQUIRE(std::filesystem::exists(data_dir / test_data));
    input = std::make_unique<std::ifstream>(data_dir / test_data);
  } else {
    input = std::make_unique<std::istringstream>(test_data);
  }
  return input;
}
#include <istream>
int find_floor(std::istream &input)
{
  int floor = 0;
  char c;
  while(input.get(c).good()) {
    if (c == '(') {
      ++floor;
    }
    else {
      --floor;
    }
  }
  return floor;
}

int find_basement_index(std::istream &input)
{
  int floor = 0;
  char c;
  int instruction_index = 0;
  while (input.get(c).good()) {
    ++instruction_index;
    if (c == '(') {
      ++floor;
    } else {
      --floor;
    }
    if (floor < 0) {
      return instruction_index;
    }
  }
  return floor;
  
}
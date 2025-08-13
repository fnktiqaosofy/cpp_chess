#ifndef BOARD_UTILS_H
#define BOARD_UTILS_H

#include <string>
#include <optional>
#include <cstdint>
#include <vector>
#include <array>
#include <algorithm>

namespace BoardUtils 
{
  int coordToIndex(std::string square);
  std::optional<std::string> indexToCoord(int index);
  std::vector<int> findPieces(uint64_t input);
}

#endif
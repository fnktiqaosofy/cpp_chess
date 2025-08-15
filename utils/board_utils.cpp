/**
* Various utility functions for finding pieces, reading algebraic chess notation etc.
*/

#include "board_utils.h"

namespace BoardUtils
{

  std::string buildRemainingMaterial(std::array<uint64_t, 10>& boards)
  {
    std::string ret = "WK";
    if (!(boards[0] == 0)) {ret += "Q";}
    if (!(boards[1] == 0))
    {
      if (__builtin_popcountll(boards[1]) >= 2) {ret += "B0B1";}
      int loc = __builtin_ctzll(boards[1]);
      if ((loc % 2) == 0) {ret += "B0";}
      else {ret += "B1";}
    }
    if(!boards[2] == 0)
    {
      int popcount = __builtin_popcountll(boards[2]);
      if (popcount > 2) {ret += "NNX";}
      if (popcount == 2) {ret += "NN";}
      if (popcount == 1) {ret += "N";}
    }
    if(!boards[3] == 0) {ret += "R";}
    if(!boards[4] == 0) {ret += "P";}

    ret += "-BK";
    if (!(boards[5] == 0)) {ret += "Q";}
    if (!(boards[6] == 0))
    {
      if (__builtin_popcountll(boards[6]) >= 2) {ret += "B0B1";}
      int loc = __builtin_ctzll(boards[6]);
      if ((loc % 2) == 0) {ret += "B0";}
      else {ret += "B1";}
    }
    if(!boards[7] == 0)
    {
      int popcount = __builtin_popcountll(boards[7]);
      if (popcount > 2) {ret += "NNX";}
      if (popcount == 2) {ret += "NN";}
      if (popcount == 1) {ret += "N";}
    }
    if(!boards[8] == 0) {ret += "R";}
    if(!boards[9] == 0) {ret += "P";}

    return ret;
  }

  int coordToIndex(std::string square)
  {
    int file = square[0] - 'a';
    int rank = square[1] - '1';
    return file + (8 * rank);
  }

  std::vector<int> findPieces(uint64_t input)
  {
      std::vector<int> indices;
      for (int i = 0; i < 64; ++i)
      {
          if ((input >> i) & 1ULL)
              indices.push_back(i);
      }
      return indices;
  }

  std::optional<std::string> indexToCoord(int index)
  {
    if (index < 0 || index > 63) return std::nullopt;
    int rank = (index / 8) + 1;
    char file = (index % 8) + '`'; // Offset to get correct file char.
    return file + std::to_string(rank);
  }
}
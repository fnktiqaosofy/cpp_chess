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
  /**
   * Array of piece combinations guaranteed to be drawn. Combinations are given in a string representation.
   */
  constexpr const std::array<std::string_view, 11> drawnPieceSetups = {
      "WK-BK", "WKB0-BK", "WKB1-BK", "WK-BKB0", "WK-BKB1", "WKN-BK", 
      "WK-BKN", "WKB0-BKB0", "WKB1-BKB1", "WKNN-BK", "WK-BKNN"};

  /**
   * Build string representation of remaining pieces, to be used in checking for insufficient material draws.
   * @param boards Array of bitboards.
   * @returns String representation of remaining material.
   */
  std::string buildRemainingMaterial(std::array<uint64_t, 10>& boards);

  /**
   * Convert algebraic chess notation to board index.
   * @param square std::string with square name in algebraic notation.
   * @returns Integer index on bitboard.
   */
  int coordToIndex(std::string square);

  /**
   * Convert integer bitboard index to algebraic chess notation.
   * @param index Integer bitboard index.
   * @returns std::string containing square in algebraic notation.
   */
  std::optional<std::string> indexToCoord(int index);

  /**
  * Find occupied bitboard indices.
  * @param input Occupation bitboard belonging to a color + piece combination.
  * @returns Vector of occupied indices.
  */ 
  std::vector<int> findPieces(uint64_t input);

}

#endif
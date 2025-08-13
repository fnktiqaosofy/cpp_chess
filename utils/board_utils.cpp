/**
* Various utility functions for finding pieces, reading algebraic chess notation etc.
*/

#include "board_utils.h"

namespace BoardUtils
{
  // Convert algebraic notation to bitboard index.
  int coordToIndex(std::string square)
  {
    int file = square[0] - 'a';
    int rank = square[1] - '1';
    return file + (8 * rank);
  }

  /**
  * Find occupied bitboard indices.
  * @param board Occupation bitboard belonging to a color + piece combination.
  */ 
  std::vector<int> findPieces(uint64_t board)
  {
      std::vector<int> indices;
      for (int i = 0; i < 64; ++i)
      {
          if ((board >> i) & 1ULL)
              indices.push_back(i);
      }
      return indices;
  }

/**
 * Convert bitboard index to algebraic notation.
 * @param index Bitboard index
 * @return String of corresponding chessboard square in algebraic notation.
 */
  std::optional<std::string> indexToCoord(int index)
  {
    if (index < 0 || index > 63) return std::nullopt;
    int rank = (index / 8) + 1;
    char file = (index % 8) + '`'; // Offset to get correct file char.
    return file + std::to_string(rank);
  }

  /**
   * Identify which piece has been captured and remove it.
   * @param bitboards Array of piece-pair bitboards
   * @param index The bitboard index of the piece that should be removed.
   */
  void pieceCapture(std::array<uint64_t, 12> bitboards, int index)
  {
    uint64_t mask = 1ULL << index;
    for (uint64_t& board : bitboards)
    {
      if (board & mask) 
      {
        board &= ~ mask;
        break;
      }
    }
  }

  /**
   * Checks if friendly piece is occupying a square.
   */
  inline bool isOccupiedByOwn(uint64_t ownPieces, int square)
  {
      return (ownPieces >> square) & 1ULL;
  }
}
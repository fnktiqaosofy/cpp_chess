#include "knight_pair.h"

// Initialize bitboard_ based on color.
KnightPair::KnightPair(Color color) : PiecePair(color)
{
  if (color == WHITE)
  {
    bitboard_ = (1ULL << 1) | (1ULL << 6);
  }
  else
  {
    bitboard_ = (1ULL << 57) | (1ULL << 62);
  }
}

// Return algebraic chess notation symbol.
char KnightPair::getSymbol() const
{
  return 'N';
}

std::vector<std::pair<int,int>> KnightPair::pseudoLegalMoves(uint64_t ownPieces, uint64_t opponentPieces) const
{
  std::vector<std::pair<int,int>> moves;
  std::vector<int> knightIndices = BoardUtils::findPieces(bitboard_);
  for (int from : knightIndices)
  {
    for (int delta : move_deltas)
    {
      int to = from + delta;
      if (to < 0 || to >= 64) continue;

      int from_file = from % 8;
      int to_file = to % 8;
      if (std::abs(to_file - from_file) > 2) continue;
      if ((ownPieces >> to ) & 1ULL) continue;

      moves.emplace_back(from, to);
    }
  }
  return moves;
}
  
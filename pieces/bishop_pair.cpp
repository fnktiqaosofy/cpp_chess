#include "bishop_pair.h"

// Initialize bitboard_ based on color.
BishopPair::BishopPair(Color color) : PiecePair(color, BISHOP)
{
  if (color == WHITE)
  {
    bitboard_ = (1ULL << 2) | (1ULL << 5);
  }
  else
  {
    bitboard_ = (1ULL << 58) | (1ULL << 61);
  }
}

// Return algebraic chess notation symbol.
char BishopPair::getSymbol() const
{
  return 'B';
}

std::vector<std::pair<int,int>> BishopPair::pseudoLegalMoves(uint64_t ownPieces, uint64_t opponentPieces) const
{
  constexpr const std::array<std::pair<int,int>, 4> directions = {{{1,1}, {1,-1}, {-1,1}, {-1,-1}}};
  std::vector<std::pair<int,int>> moves;
  std::vector<int> bishopIndices = findPieces(bitboard_);

  for (int from : bishopIndices)
  {
    int fromRank = from / 8;
    int fromFile = from % 8;

    for (auto [dr, df] : directions)
    {
      int currRank = fromRank + dr;
      int currFile = fromFile + df;
      while(currRank >= 0 && currRank < 8 && currFile >= 0 && currFile < 8)
      {
        int to = currRank * 8 + currFile;

        if ((ownPieces >> to) & 1ULL) break;

        moves.emplace_back(from, to);

        if ((opponentPieces >> to) & 1ULL) break;

        currRank += dr;
        currFile += df;
      }
    }
  }
  return moves;
}
  
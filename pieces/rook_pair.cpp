#include "rook_pair.h"

// Initialize bitboard_ based on color.
RookPair::RookPair(Color color) : PiecePair(color, ROOK)
{
  if (color == WHITE)
  {
    bitboard_ = (1ULL << 0) | (1ULL << 7);
  }
  else
  {
    bitboard_ = (1ULL << 56) | (1ULL << 63);
  }
}

// Return algebraic chess notation symbol.
char RookPair::getSymbol() const
{
  return 'R';
}

std::vector<std::pair<int,int>> RookPair::pseudoLegalMoves(uint64_t ownPieces, uint64_t opponentPieces) const
{
  constexpr const std::array<std::pair<int,int>, 4> directions = {{{1,0}, {-1,0}, {0,1}, {0,-1}}};
  std::vector<std::pair<int,int>> moves;
  std::vector<int> rookIndices = findPieces(bitboard_);

  for (int from : rookIndices)
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
  
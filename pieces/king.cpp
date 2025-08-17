#include "king.h"

// Initialize bitboard_ based on color.
King::King(Color color) : PiecePair(color, KNIGHT)
{
  if (color == WHITE)
  {
    bitboard_ = (1ULL << 4);
  }
  else
  {
    bitboard_ = (1ULL << 60);
  }
}

// Return algebraic chess notation symbol.
char King::getSymbol() const
{
  return 'K';
}

std::vector<std::pair<int,int>> King::pseudoLegalMoves(uint64_t ownPieces, uint64_t opponentPieces) const
{
  constexpr const std::array<std::pair<int,int>, 8> directions = {{{1,1}, {1,-1}, {-1,1}, {-1,-1}, {1,0}, {-1,0}, {0,1}, {0,-1}}};
  std::vector<std::pair<int,int>> moves;
  std::vector<int> kingIndices = findPieces(bitboard_);

  for (int from : kingIndices)
  {
    int fromRank = from / 8;
    int fromFile = from % 8;

    for (auto [dr, df] : directions)
    {
      int currRank = fromRank + dr;
      int currFile = fromFile + df;
      if(currRank >= 0 && currRank < 8 && currFile >= 0 && currFile < 8)
      {
        int to = currRank * 8 + currFile;

        if ((ownPieces >> to) & 1ULL) break;

        moves.emplace_back(from, to);

      }
    }
  }
  return moves;
}
  
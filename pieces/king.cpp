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
    int from_rank = from / 8;
    int from_file = from % 8;

    for (auto [dr, df] : directions)
    {
      int curr_rank = from_rank + dr;
      int curr_file = from_file + df;
      if(curr_rank >= 0 && curr_rank < 8 && curr_file >= 0 && curr_file < 8)
      {
        int to = curr_rank * 8 + curr_file;

        if ((ownPieces >> to) & 1ULL) break;

        moves.emplace_back(from, to);

      }
    }
  }
  return moves;
}
  
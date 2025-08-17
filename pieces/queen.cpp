#include "queen.h"

// Initialize bitboard_ based on color.
Queen::Queen(Color color) : PiecePair(color, QUEEN)
{
  if (color == WHITE)
  {
    bitboard_ = (1ULL << 3);
  }
  else
  {
    bitboard_ = (1ULL << 59);
  }
}

// Return algebraic chess notation symbol.
char Queen::getSymbol() const
{
  return 'Q';
}

std::vector<std::pair<int,int>> Queen::pseudoLegalMoves(uint64_t ownPieces, uint64_t opponentPieces) const
{
  constexpr const std::array<std::pair<int,int>, 8> directions = {{{1,1}, {1,-1}, {-1,1}, {-1,-1}, {1,0}, {-1,0}, {0,1}, {0,-1}}};
  std::vector<std::pair<int,int>> moves;
  std::vector<int> queenIndices = findPieces(bitboard_);

  for (int from : queenIndices)
  {
    int from_rank = from / 8;
    int from_file = from % 8;

    for (auto [dr, df] : directions)
    {
      int curr_rank = from_rank + dr;
      int curr_file = from_file + df;
      while(curr_rank >= 0 && curr_rank < 8 && curr_file >= 0 && curr_file < 8)
      {
        int to = curr_rank * 8 + curr_file;

        if ((ownPieces >> to) & 1ULL) break;

        moves.emplace_back(from, to);

        if ((opponentPieces >> to) & 1ULL) break;

        curr_rank += dr;
        curr_file += df;
      }
    }
  }
  return moves;
}
  
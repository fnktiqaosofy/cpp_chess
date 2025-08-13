#include "bishop_pair.h"

// Initialize bitboard_ based on color.
BishopPair::BishopPair(Color color) : PiecePair(color)
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
  std::vector<int> bishopIndices = BoardUtils::findPieces(bitboard_);

  for (int from : bishopIndices)
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
  
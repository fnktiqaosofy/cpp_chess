#include "pawn.h"

// Initialize bitboard_ based on color.
Pawn::Pawn(Color color) : PiecePair(color)
{
  if (color == WHITE)
  {
    bitboard_ = (1ULL << 8) | (1ULL << 9) | (1ULL << 10) | (1ULL << 11) | 
                (1ULL << 12) | (1ULL << 13) | (1ULL << 14) | (1ULL << 15);
  }
  else
  {
    bitboard_ = (1ULL << 48) | (1ULL << 49) | (1ULL << 50) | (1ULL << 51) | 
                (1ULL << 52) | (1ULL << 53) | (1ULL << 54) | (1ULL << 55);
  }
}

char Pawn::getSymbol() const
{
  return 'P';
}

std::vector<std::pair<int,int>> Pawn::pseudoLegalMoves(uint64_t ownPieces, uint64_t opponentPieces) const
{
  std::array<std::pair<int,int>, 2> attackDirection;
  int direction;
  int startRank;
  if (color == WHITE)
  {
    direction = 1;
    startRank = 1;
    attackDirection = {{{1,1}, {1,-1}}};
  }
  else
  {
    direction = -1;
    startRank = 6;
    attackDirection = {{{-1,1}, {-1,-1}}};
  }

  std::vector<std::pair<int,int>> moves;
  std::vector<int> pawnIndices = BoardUtils::findPieces(bitboard_);

  for (int from : pawnIndices)
  {
    int fromRank = from / 8;
    int fromFile = from % 8;

    for (auto [dr, df] : attackDirection)
    {
      int curr_rank = fromRank + dr;
      int curr_file = fromFile + df;
      if (curr_rank >= 0 && curr_rank < 8 && curr_file >= 0 && curr_file < 8)
      {
        int to = curr_rank * 8 + curr_file;
        if ((opponentPieces >> to) & 1ULL)
        {
          moves.emplace_back(from, to);
        }
      }
    }

    int to = from + (direction*8);
    if (((ownPieces | opponentPieces) >> to) & 1ULL) continue;

    moves.emplace_back(from, to);

    to = from + (direction*16);
    if (((from / 8) == startRank) && !(((ownPieces | opponentPieces) >> to) & 1ULL))
    {
      moves.emplace_back(from, to);
    }
  }
  return moves;
}
  
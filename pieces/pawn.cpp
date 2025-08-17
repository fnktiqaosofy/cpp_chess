#include "pawn.h"
#include "../utils/board.h"


// Initialize bitboard_ based on color.
Pawn::Pawn(Color color, Board& b) : PiecePair(color, PAWN), board(b)
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

  uint8_t enPassant = board.getEnpassant();
  std::vector<std::pair<int,int>> moves;
  std::vector<int> pawnIndices = findPieces(bitboard_);

  for (int from : pawnIndices)
  {
    int fromRank = from / 8;
    int fromFile = from % 8;

    for (auto [dr, df] : attackDirection)
    {
      int currRank = fromRank + dr;
      int currFile = fromFile + df;
      if (currRank >= 0 && currRank < 8 && currFile >= 0 && currFile < 8)
      {

        int to = currRank * 8 + currFile;
        bool normalCapture = (opponentPieces >> to) & 1ULL;
        bool epCapture = false;

        // ugly ugly ugly ugly ugly
        if (fromRank == (color == WHITE ? 4 : 3))
        {
          if (currFile == fromFile + 1 && fromFile < 7)
          {
            epCapture = (enPassant >> (currFile)) & 1;
          }
          else if (currFile == fromFile - 1 && fromFile > 0)
          {
            epCapture = (enPassant >> (currFile)) & 1;
          }
        }
        if (normalCapture || epCapture) {moves.emplace_back(from, to);}
      }
    }

    int to = from + (direction*8);
    if (((ownPieces | opponentPieces) >> to) & 1ULL) continue;

    moves.emplace_back(from, to);

    to = from + (direction*16);
    if (((from / 8) == startRank) && !(((ownPieces | opponentPieces) >> to) & 1ULL) &&
      !(((ownPieces | opponentPieces) >> (from + 8*direction)) & 1ULL))
    {
      moves.emplace_back(from, to);
    }
  }
  return moves;
}
  
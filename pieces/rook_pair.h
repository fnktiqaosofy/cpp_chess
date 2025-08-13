#ifndef ROOK_PAIR
#define ROOK_PAIR

#include "piece_pair.h"


class RookPair : public PiecePair
{
  public:
      // Constructor
      RookPair(Color color);

      // Returns 'R' for rook
      char getSymbol() const override;

      // Returns list of pseudo-legal moves: (from, to)
      std::vector<std::pair<int, int>> pseudoLegalMoves(uint64_t ownPieces, uint64_t opponentPieces) const;
    
};


#endif
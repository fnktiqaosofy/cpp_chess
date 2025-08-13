#ifndef BISHOP_PAIR
#define BISHOP_PAIR

#include "piece_pair.h"


class BishopPair : public PiecePair
{
  public:
      // Constructor
      BishopPair(Color color);

      // Returns 'B' for bishop
      char getSymbol() const override;

      // Returns list of pseudo-legal moves: (from, to)
      std::vector<std::pair<int, int>> pseudoLegalMoves(uint64_t ownPieces, uint64_t opponentPieces) const;

};


#endif
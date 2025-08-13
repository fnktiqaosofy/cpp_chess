#ifndef PAWN_H
#define PAWN_H

#include "piece_pair.h"


class Pawn : public PiecePair
{
  public:
      // Constructor
      Pawn(Color color);

      char getSymbol() const override;

      // Returns list of pseudo-legal moves: (from, to)
      std::vector<std::pair<int, int>> pseudoLegalMoves(uint64_t ownPieces, uint64_t opponentPieces) const;

};


#endif
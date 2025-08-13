#ifndef QUEEN_H
#define QUEEN_H

#include "piece_pair.h"


class Queen : public PiecePair
{
  public:
      // Constructor
      Queen(Color color);

      // Returns 'Q' for queen
      char getSymbol() const override;

      // Returns list of pseudo-legal moves: (from, to)
      std::vector<std::pair<int, int>> pseudoLegalMoves(uint64_t ownPieces, uint64_t opponentPieces) const;

};


#endif
#ifndef KING_H
#define KING_H

#include "piece_pair.h"


class King : public PiecePair
{
  public:
      // Constructor
      King(Color color);

      // Returns 'K' for King
      char getSymbol() const override;

      // Returns list of pseudo-legal moves: (from, to)
      std::vector<std::pair<int, int>> pseudoLegalMoves(uint64_t ownPieces, uint64_t opponentPieces) const;

};


#endif
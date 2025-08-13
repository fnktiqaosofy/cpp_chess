#ifndef KNIGHT_PAIR
#define KNIGHT_PAIR

#include "piece_pair.h"


class KnightPair : public PiecePair
{
public:
    // Constructor
    KnightPair(Color color);

    // Returns 'N' for knight
    char getSymbol() const override;

    // Returns list of pseudo-legal moves: (from, to)
    std::vector<std::pair<int, int>> pseudoLegalMoves(uint64_t ownPieces, uint64_t opponentPieces) const;

private:
    static constexpr std::array<int, 8> move_deltas = {6, 10, 15, 17, -6, -10, -15, -17};
};


#endif
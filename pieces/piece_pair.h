#ifndef PIECE_H
#define PIECE_H

#include <cstdint>
#include <vector>
#include <array>
#include "../utils/board_utils.h"

class PiecePair
{
  public:
    enum Color {WHITE, BLACK};

    Color getColor() const {return color;}

    PiecePair(Color color) : color(color) {}

    virtual char getSymbol() const = 0;

    virtual ~PiecePair() = default;

    virtual std::vector<std::pair<int,int>> pseudoLegalMoves(uint64_t ownPieces, uint64_t opponentPieces) const = 0;

    uint64_t getBitboard()
    {
      return bitboard_;
    }

    void setBitboard(uint64_t board)
    {
      bitboard_ = board;
    }

    void clearSquare(int square) {
      bitboard_ &= ~(1ULL << square);
    }

    void flipBit(int square) {
      bitboard_ ^= (1ULL << square);
    }

    void applyMove(int from, int to)
    {
        bitboard_ &= ~(1ULL << from);
        bitboard_ |= (1ULL << to);
    }

  protected:
    Color color;
    uint64_t bitboard_;
};

#endif
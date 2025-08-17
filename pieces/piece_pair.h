#ifndef PIECE_H
#define PIECE_H

#include <cstdint>
#include <vector>
#include <array>
#include <cstdlib>

class PiecePair
{
  public:

    enum Color {WHITE, BLACK};

    enum PieceType {KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN};

    Color getColor() const {return color;}

    PiecePair(Color color, PieceType type) : color(color), type(type) {}

    virtual char getSymbol() const = 0;

    virtual ~PiecePair() = default;

    virtual std::vector<std::pair<int,int>> pseudoLegalMoves(uint64_t ownPieces, uint64_t opponentPieces) const = 0;

    uint64_t getBitboard() const
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
        bitboard_ ^= ((1ULL << from) | (1ULL << to));
    }

    std::vector<int> findPieces(uint64_t input) const
    {
        std::vector<int> indices;
        for (int i = 0; i < 64; ++i)
        {
            if ((input >> i) & 1ULL)
                indices.push_back(i);
        }
        return indices;
    }
  private:

    PieceType type;

  protected:
  
    Color color;
    uint64_t bitboard_;
};

#endif
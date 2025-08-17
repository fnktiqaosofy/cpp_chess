#ifndef BOARD_H
#define BOARD_H

#include "../pieces/pawn.h"
#include "../pieces/knight_pair.h"
#include "../pieces/bishop_pair.h"
#include "../pieces/rook_pair.h"
#include "../pieces/queen.h"
#include "../pieces/king.h"
#include "board_utils.h"

class Board
{
  public:

    enum class Turn {WHITE, BLACK};
  
    Board();

    /**
     * Updates whitePieces and blackPieces by bitwise OR (|)-ing all piece bitboards.
     */
    void updateBoardAggregates();

    /**
     * Performs a move. Use only after legality check.
     * @param piece Algebraic chess notation of piece making the move.
     * @param from Index of move origin.
     * @param to Index of move destination.
     * @param color Color of player making the move.
     */
    void moveCommand(char piece, int from, int to, Turn color);

    /**
     * Castles.
     */
    int castleCommand(std::string command, Turn color);

    /**
     * Handles promotions.
     * @param to Index of square where the promoted piece will be placed.
     * @param symbol Algebraic chess identifier corresponding to desired piece.
     */
    void promotion(int to, char symbol);

    /**
     * Checks whether a pseudolegal move is legal.
     * @param to Move destination.
     * @param from Move origin.
     * @returns Boolean indicating legality.
     */
    bool checkMoveLegality(Turn color, int to, int from);

    /**
    * Finds index of king. 
    * @param color Color of king to find.
    * @returns Position index of specified king.
    */
    int findKing(Turn color);

    /**
     * Mostly obsolete, but used in printing board to console. Identifies piece located on index. 
     * @param index Index of piece.
     * @returns Char corresponding to piece's identifier in algebraic chess notation.
     */
    char identifyPiece(int index);
    
    /**
     * Identifies enemy piece located on index.
     * @param index Index of piece.
     * @returns Char corresponding to piece's identifier in algebraic chess notation.
     */
    PiecePair* identifyEnemyPiece(int index, Turn color);

    /**
     * Identifies which piece is making a move.
     * @param color Color of player making the move.
     * @param symbol Piece identifier in algebraic chess notation.
     * @param to Index of square being moved to.
     * @returns Index of the move's origin.
     */
    int identifyMover(Turn color, char symbol, int to, std::string specifier);

    /**
     * Returns all pseudolegal moves possible for a player of a given color.
     * Used in checking if king is in check.
     * @param color Color of current active player.
     * @returns List of pseudolegal moves.
     */
    std::vector<std::pair<int,int>> getAllPseudoLegalMoves(Turn color);

    /**
     * Instruct board to switch currently active player.
     */
    void switchMover();

    /**
     * Check if square is threatened by opposing color.
     * @param color Color of current player
     * @returns Boolean signifying whether opposing player threatens square.
     */
    bool isSquareThreatened(int square);

    /**
     * Check if castling is blocked due to threatened squares.
     * @param side 'Q' or 'K', for queenside / kingside castling.
     * @returns Bool signifying whether castling is blocked for the given side.
     */
    bool isCastlingThreatened(char side);

    /**
     * Checks if board is in a checkmate state.
     * @returns Bool signifying whether no legal moves can be made that bring the king out of check.
     */
    bool checkMate();

    /**
     * Checks if material is insufficient for game not to end in a draw.
     * @returns Bool signifying whether material is insufficient.
     */
    bool insufficientMaterial();

    /**
     * Get current player.
     * @returns Color enum denoting current player.
     */
    Turn getToMove();

  private:
  
    Pawn whitePawns;
    Pawn blackPawns;
    KnightPair whiteKnights;
    KnightPair blackKnights;
    BishopPair whiteBishops;
    BishopPair blackBishops;
    RookPair whiteRooks;
    RookPair blackRooks;
    Queen whiteQueen;
    Queen blackQueen;
    King whiteKing;
    King blackKing;

    uint64_t whitePieces;
    uint64_t blackPieces;

    Turn toMove;
    
    bool whiteCastledK;
    bool whiteCastledQ;
    bool blackCastledK;
    bool blackCastledQ;

    PiecePair* pieceTable[2][6];
    
};

#endif
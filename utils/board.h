#ifndef BOARD_H
#define BOARD_H

#include "../pieces/pawn.h"
#include "../pieces/knight_pair.h"
#include "../pieces/bishop_pair.h"
#include "../pieces/rook_pair.h"
#include "../pieces/queen.h"
#include "../pieces/king.h"

class Board
{
  public:

    enum Color {WHITE, BLACK};

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

    Color toMove;
    bool whiteCastledK;
    bool whiteCastledQ;
    bool blackCastledK;
    bool blackCastledQ;
  
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
    void moveCommand(char piece, int from, int to, Color color);

    /**
     * Castles.
     */
    void castleCommand(std::string command, Color color);

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
    bool checkMoveLegality(Color color, int to, int from);

    /**
    * Finds index of king. 
    * @param color Color of king to find.
    * @returns Position index of specified king.
    */
    int findKing(Color color);

    /**
     * Identifies piece located on index.
     * @param index Index of piece.
     * @returns Char corresponding to piece's identifier in algebraic chess notation.
     */
    char identifyPiece(int index);

    /**
     * Identifies which piece is making a move.
     * @param color Color of player making the move.
     * @param piece Piece identifier in algebraic chess notation.
     * @param to Index of square being moved to.
     * @returns Index of the move's origin.
     */
    int identifyMover(Color color, char piece, int to);

    /**
     * Returns all pseudolegal moves possible for a player of a given color.
     * Used in checking if king is in check.
     * @param color Color of current active player.
     * @returns List of pseudolegal moves.
     */
    std::vector<std::pair<int,int>> getAllPseudoLegalMoves(Color color);

    /**
     * Instruct board to switch currently active player.
     */
    void switchMover();
    
};

#endif
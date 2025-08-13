#include "board.h"

Board::Board()
  : whitePawns(PiecePair::WHITE),
    blackPawns(PiecePair::BLACK),
    whiteKnights(PiecePair::WHITE),
    blackKnights(PiecePair::BLACK),
    whiteBishops(PiecePair::WHITE),
    blackBishops(PiecePair::BLACK),
    whiteRooks(PiecePair::WHITE),
    blackRooks(PiecePair::BLACK),
    whiteQueen(PiecePair::WHITE),
    blackQueen(PiecePair::BLACK),
    whiteKing(PiecePair::WHITE),
    blackKing(PiecePair::BLACK)
{
  updateBoardAggregates();
  toMove = Board::WHITE;
  whiteCastledK = false;
  whiteCastledQ = false;
  blackCastledK = false;
  blackCastledQ = false;
}

void Board::updateBoardAggregates()
{
  whitePieces = whitePawns.getBitboard() | whiteKnights.getBitboard()
                | whiteBishops.getBitboard() | whiteRooks.getBitboard()
                | whiteQueen.getBitboard() | whiteKing.getBitboard();
  blackPieces = blackPawns.getBitboard() | blackKnights.getBitboard()
                | blackBishops.getBitboard() | blackRooks.getBitboard()
                | blackQueen.getBitboard() | blackKing.getBitboard();
}

bool Board::checkMoveLegality(Board::Color color, int from, int to)
{
  uint64_t moveMask = 0;
  moveMask = (1ULL << to) | (1ULL << from);
  uint64_t tempBoard = (color == WHITE) ? whitePieces : blackPieces;
  uint64_t newBoardState = tempBoard ^ moveMask;

  std::vector<std::pair<int,int>> moveList;
  uint64_t attackingPieces = (color == WHITE) ? blackPieces : whitePieces;
  uint64_t defendingPieces = newBoardState;
  
  auto attackingPawns = (color == WHITE) ? blackPawns : whitePawns;
  auto attackingKnights = (color == WHITE) ? blackKnights : whiteKnights;
  auto attackingBishops = (color == WHITE) ? blackBishops : whiteBishops;
  auto attackingRooks = (color == WHITE) ? blackRooks : whiteRooks;
  auto attackingQueen = (color == WHITE) ? blackQueen : whiteQueen;
  auto attackingKing = (color == WHITE) ? blackKing : whiteKing;

  auto defendingKing = (color == WHITE) ? whiteKing : blackKing;

  if (defendingKing.getBitboard() >> from)
  {
    defendingKing.clearSquare(from);
    defendingKing.flipBit(to);
  }

  if ((attackingPieces >> to) & 1)
  {
    attackingPieces &= ~(1ULL << to);
    attackingPawns.clearSquare(to);
    attackingKnights.clearSquare(to);
    attackingBishops.clearSquare(to);
    attackingRooks.clearSquare(to);
    attackingQueen.clearSquare(to);
    attackingKing.clearSquare(to);
  }

  std::vector<std::vector<std::pair<int,int>>> allMoves = {
      attackingPawns.pseudoLegalMoves(attackingPieces, defendingPieces),
      attackingKnights.pseudoLegalMoves(attackingPieces, defendingPieces),
      attackingBishops.pseudoLegalMoves(attackingPieces, defendingPieces),
      attackingRooks.pseudoLegalMoves(attackingPieces, defendingPieces),
      attackingQueen.pseudoLegalMoves(attackingPieces, defendingPieces),
      attackingKing.pseudoLegalMoves(attackingPieces, defendingPieces)
  };

  int kingPosition = findKing(color);
  for (const auto& moveSet : allMoves)
  {
    for (const auto& [fromSq, toSq] : moveSet)
    {
      if (toSq == kingPosition)
        return false;
    }
  }
  return true;
}

void Board::moveCommand(char symbol, int from, int to, Color color)
{
  switch (symbol)
  {
    case 'P':
      if (color == WHITE) {whitePawns.clearSquare(from); whitePawns.flipBit(to);} else {blackPawns.clearSquare(from); blackPawns.flipBit(to);}
      break;
    case 'R':
      if (color == WHITE) {whiteRooks.clearSquare(from); whiteRooks.flipBit(to);} else {blackRooks.clearSquare(from); blackRooks.flipBit(to);}
      break;
    case 'N':
      if (color == WHITE) {whiteKnights.clearSquare(from); whiteKnights.flipBit(to);} else {blackKnights.clearSquare(from); blackKnights.flipBit(to);}
      break;
    case 'B':
      if (color == WHITE) {whiteBishops.clearSquare(from); whiteBishops.flipBit(to);} else {blackBishops.clearSquare(from); blackBishops.flipBit(to);}
      break;
    case 'Q':
      if (color == WHITE) {whiteQueen.clearSquare(from); whiteQueen.flipBit(to);} else {blackQueen.clearSquare(from); blackQueen.flipBit(to);}
      break;
    case 'K':
      if (color == WHITE)
      {
        whiteKing.clearSquare(from); whiteKing.flipBit(to);
        whiteCastledK = true;
        whiteCastledQ = true;
      } 
      else 
      {
        blackKing.clearSquare(from); blackKing.flipBit(to);
        blackCastledK = true;
        blackCastledQ = true;
      }
      break;
  }
  uint64_t attackedPieces = (color == WHITE) ? blackPieces : whitePieces;
  if ((attackedPieces >> to) & 1)
  {
    auto& attackingPawns = (color == WHITE) ? blackPawns : whitePawns;
    auto& attackingKnights = (color == WHITE) ? blackKnights : whiteKnights;
    auto& attackingBishops = (color == WHITE) ? blackBishops : whiteBishops;
    auto& attackingRooks = (color == WHITE) ? blackRooks : whiteRooks;
    auto& attackingQueen = (color == WHITE) ? blackQueen : whiteQueen;
    auto& attackingKing = (color == WHITE) ? blackKing : whiteKing;
    attackedPieces &= ~(1ULL << to);
    attackingPawns.clearSquare(to);
    attackingKnights.clearSquare(to);
    attackingBishops.clearSquare(to);
    attackingRooks.clearSquare(to);
    attackingQueen.clearSquare(to);
    attackingKing.clearSquare(to);
  }

  updateBoardAggregates();
}

void Board::castleCommand(std::string command, Color color)
{
  auto& targetKing = (color == WHITE) ? whiteKing : blackKing;
  switch (command.length())
  {
    case '5':

      break;
    case '3':
      break;
  }
}

void Board::promotion(int to, char symbol)
{
  auto& pawnBoard = (toMove == Board::WHITE) ? whitePawns : blackPawns;
  pawnBoard.clearSquare(to);
  switch (symbol)
  {
    case 'Q':
    {
      auto& targetBoard = (toMove == Board::WHITE) ? whiteQueen : blackQueen;
      targetBoard.flipBit(to);
      break;
    }
    case 'R':
    {
      auto& targetBoard = (toMove == Board::WHITE) ? whiteRooks : blackRooks;
      targetBoard.flipBit(to);
      break;
    }
    case 'N':
    {
      auto& targetBoard = (toMove == Board::WHITE) ? whiteKnights : blackKnights;
      targetBoard.flipBit(to);
      break;
    }
    case 'B':
    {
      auto& targetBoard = (toMove == Board::WHITE) ? whiteBishops : blackBishops;
      targetBoard.flipBit(to);
      break;
    }
  }
}

int Board::identifyMover(Color color, char piece, int to)
{
  std::vector<std::pair<int,int>> moves;
  switch (piece)
  {
    case 'P':
      moves = (color == WHITE) ? whitePawns.pseudoLegalMoves(whitePieces, blackPieces) : blackPawns.pseudoLegalMoves(blackPieces, whitePieces);
      break;
    case 'R':
      moves = (color == WHITE) ? whiteRooks.pseudoLegalMoves(whitePieces, blackPieces) : blackRooks.pseudoLegalMoves(blackPieces, whitePieces);
      break;
    case 'N':
      moves = (color == WHITE) ? whiteKnights.pseudoLegalMoves(whitePieces, blackPieces) : blackKnights.pseudoLegalMoves(blackPieces, whitePieces);
      break;
    case 'B':
      moves = (color == WHITE) ? whiteBishops.pseudoLegalMoves(whitePieces, blackPieces) : blackBishops.pseudoLegalMoves(blackPieces, whitePieces);
      break;
    case 'Q':
      moves = (color == WHITE) ? whiteQueen.pseudoLegalMoves(whitePieces, blackPieces) : blackQueen.pseudoLegalMoves(blackPieces, whitePieces);
      break;
    case 'K':
      return findKing(color);
  }

  int matchCount = 0;
  int ind = -1;

  for (const auto& [fromSq, toSq] : moves)
  {
      if (toSq == to)
      {
          ++matchCount;
          if (matchCount == 1)
              ind = fromSq;
          else
              return -1; // multiple matches
      }
  }

  return (matchCount == 1) ? ind : -1;
}

char Board::identifyPiece(int index)
{
  uint64_t mask = 1ULL << index;

  if (whitePawns.getBitboard() & mask)   return whitePawns.getSymbol();
  if (blackPawns.getBitboard() & mask)   return blackPawns.getSymbol();
  if (whiteKnights.getBitboard() & mask) return whiteKnights.getSymbol();
  if (blackKnights.getBitboard() & mask) return blackKnights.getSymbol();
  if (whiteBishops.getBitboard() & mask) return whiteBishops.getSymbol();
  if (blackBishops.getBitboard() & mask) return blackBishops.getSymbol();
  if (whiteRooks.getBitboard() & mask)   return whiteRooks.getSymbol();
  if (blackRooks.getBitboard() & mask)   return blackRooks.getSymbol();
  if (whiteQueen.getBitboard() & mask)   return whiteQueen.getSymbol();
  if (blackQueen.getBitboard() & mask)   return blackQueen.getSymbol();
  if (whiteKing.getBitboard() & mask)    return whiteKing.getSymbol();
  if (blackKing.getBitboard() & mask)    return blackKing.getSymbol();

  return ' ';
}

std::vector<std::pair<int,int>> Board::getAllPseudoLegalMoves(Board::Color color)
{
  std::vector<std::pair<int,int>> moveList;
  uint64_t ownPieces = (color == WHITE) ? whitePieces : blackPieces;
  uint64_t opponentPieces = (color == WHITE) ? blackPieces : whitePieces;

  const auto& pawns   = (color == WHITE) ? whitePawns   : blackPawns;
  const auto& knights = (color == WHITE) ? whiteKnights : blackKnights;
  const auto& bishops = (color == WHITE) ? whiteBishops : blackBishops;
  const auto& rooks   = (color == WHITE) ? whiteRooks   : blackRooks;
  const auto& queen   = (color == WHITE) ? whiteQueen   : blackQueen;
  const auto& king    = (color == WHITE) ? whiteKing    : blackKing;

  std::vector<std::vector<std::pair<int,int>>> allMoves = {
      pawns.pseudoLegalMoves(ownPieces, opponentPieces),
      knights.pseudoLegalMoves(ownPieces, opponentPieces),
      bishops.pseudoLegalMoves(ownPieces, opponentPieces),
      rooks.pseudoLegalMoves(ownPieces, opponentPieces),
      queen.pseudoLegalMoves(ownPieces, opponentPieces),
      king.pseudoLegalMoves(ownPieces, opponentPieces)
  };

  for (const auto& moves : allMoves)
    moveList.insert(moveList.end(), moves.begin(), moves.end());

  return moveList;
}

/**
 * __builtin_ctzll (calculate trailing zeroes for a long long) compiles to a single CPU command!
 * Due to bitboard representation, the number of trailing zeroes is equal to index of the set bit.
 * (Obviously only applicable to king)
 */
int Board::findKing(Board::Color color)
{
  if (color == Board::WHITE)
  {
    return __builtin_ctzll(whiteKing.getBitboard());
  }
  else
  {
    return __builtin_ctzll(blackKing.getBitboard());
  }

}

void Board::switchMover()
{
  toMove = (toMove == WHITE) ? BLACK : WHITE;
}

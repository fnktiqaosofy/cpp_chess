#include "board.h"
#include <iostream>

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
    blackKing(PiecePair::BLACK),
    whiteCastledK(false),
    whiteCastledQ(false),
    blackCastledK(false),
    blackCastledQ(false),
    toMove(Board::WHITE)
    //whitePieces(0x000000000000FFFF),
    //blackPieces(0xFFFF000000000000)
{
  updateBoardAggregates();
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

// TODO: Refactor
bool Board::checkMoveLegality(Board::Color color, int from, int to)
{
  uint64_t moveMask;
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

  int kingPosition;
  if ((defendingKing.getBitboard() >> from) & 1ULL)
  {
    defendingKing.clearSquare(from);
    defendingKing.flipBit(to);
    kingPosition = to;
  }
  else
  {
    kingPosition = findKing(color);
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
  if(from == 0) {whiteCastledQ = true;}
  if(from == 7) {whiteCastledK = true;}
  if(from == 4) {whiteCastledK = true; whiteCastledQ = true;}
  if(from == 60) {blackCastledK = true; blackCastledQ = true;}
  if(from == 63) {blackCastledK = true;}
  if(from == 56) {blackCastledQ = true;}
  switch (symbol)
  {
    case 'P':
      if (color == WHITE) {whitePawns.applyMove(from, to);} else {blackPawns.applyMove(from, to);}
      break;
    case 'R':
      if (color == WHITE) {whiteRooks.applyMove(from, to);} else {blackRooks.applyMove(from, to);}
      break;
    case 'N':
      if (color == WHITE) {whiteKnights.applyMove(from, to);} else {blackKnights.applyMove(from, to);}
      break;
    case 'B':
      if (color == WHITE) {whiteBishops.applyMove(from, to);} else {blackBishops.applyMove(from, to);}
      break;
    case 'Q':
      if (color == WHITE) {whiteQueen.applyMove(from, to);} else {blackQueen.applyMove(from, to);}
      break;
    case 'K':
      if (color == WHITE)
      {
        whiteKing.applyMove(from, to);
        //whiteCastledK = true;
        //whiteCastledQ = true;
      } 
      else 
      {
        blackKing.applyMove(from, to);
        //blackCastledK = true;
        //blackCastledQ = true;
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


int Board::castleCommand(std::string command, Color color)
{
  switch (command.length())
  {
    // TODO: Write separate threatened function for castling, as this is a bit inefficient.
    case (5):
      if (color == WHITE && !whiteCastledQ && !isSquareThreatened(4) && !isSquareThreatened(3) && !isSquareThreatened(2) &&
          (whitePieces & ((1ULL << 1) | (1ULL << 2) | (1ULL << 3)) == 0))
      {
        whiteKing.applyMove(4, 2);
        whiteRooks.applyMove(0, 3);
        whitePieces ^= ((1ULL << 0) | (1ULL << 2) | (1ULL << 3) | (1ULL << 4));
        return 0;
      }
      else if (color == BLACK && !blackCastledQ && !isSquareThreatened(60) && !isSquareThreatened(59) && !isSquareThreatened(58) &&
          (blackPieces & ((1ULL << 57) | (1ULL << 58) | (1ULL << 59)) == 0))
      {
        blackKing.applyMove(60, 58);
        blackRooks.applyMove(56, 59);
        blackPieces ^= ((1ULL << 56) | (1ULL << 58) | (1ULL << 59) | (1ULL << 60));
        return 0;
      }
      break;
    case (3):
      if (color == WHITE && !whiteCastledK && !isSquareThreatened(4) && !isSquareThreatened(5) && !isSquareThreatened(6) &&
          ((whitePieces & ((1ULL << 5) | (1ULL << 6))) == 0))
      {
        whiteKing.applyMove(4, 6);
        whiteRooks.applyMove(7, 5);
        whitePieces ^= ((1ULL << 4) | (1ULL << 5) | (1ULL << 6) | (1ULL << 7));
        return 0;
      }
      else if (color == BLACK && !blackCastledK && !isSquareThreatened(60) && !isSquareThreatened(61) && !isSquareThreatened(62) &&
          (whitePieces & ((1ULL << 62) | (1ULL << 63)) == 0))
      {
        blackKing.applyMove(60, 62);
        blackRooks.applyMove(63, 61);
        blackPieces ^= ((1ULL << 60) | (1ULL << 61) | (1ULL << 62) | (1ULL << 63));
        return 0;
      }
      break;
  }
  return -1;
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

bool Board::isSquareThreatened(int square)
{
  std::vector<std::vector<std::pair<int,int>>> allMoves;
  if (toMove == WHITE)
  {
  allMoves = {
      blackPawns.pseudoLegalMoves(blackPieces, whitePieces),
      blackKnights.pseudoLegalMoves(blackPieces, whitePieces),
      blackBishops.pseudoLegalMoves(blackPieces, whitePieces),
      blackRooks.pseudoLegalMoves(blackPieces, whitePieces),
      blackQueen.pseudoLegalMoves(blackPieces, whitePieces),
      blackKing.pseudoLegalMoves(blackPieces, whitePieces)
  };
  }
  else
  {
  allMoves = {
      whitePawns.pseudoLegalMoves(whitePieces, blackPieces),
      whiteKnights.pseudoLegalMoves(whitePieces, blackPieces),
      whiteBishops.pseudoLegalMoves(whitePieces, blackPieces),
      whiteRooks.pseudoLegalMoves(whitePieces, blackPieces),
      whiteQueen.pseudoLegalMoves(whitePieces, blackPieces),
      whiteKing.pseudoLegalMoves(whitePieces, blackPieces)
  };
  }
  for (const auto& moveSet : allMoves)
  {
    for (const auto& [fromSq, toSq] : moveSet)
    {
      if (toSq == square)
        return true;
    }
  }
  return false;
}

bool Board::checkMate()
{
  auto allmoves = getAllPseudoLegalMoves(Board::toMove);
  int legalMoves = 0;
  for (const auto& [fromSq, toSq] : allmoves)
  {
      if (checkMoveLegality(toMove, fromSq, toSq)) {++legalMoves;}
  }
  return (legalMoves == 0) ? true : false;
}

bool Board::insufficientMaterial()
{
  std::array<uint64_t, 10> boards = {
    whiteQueen.getBitboard(),
    whiteBishops.getBitboard(),
    whiteKnights.getBitboard(),
    whiteRooks.getBitboard(),
    whitePawns.getBitboard(),
    blackQueen.getBitboard(),
    blackBishops.getBitboard(),
    blackKnights.getBitboard(),
    blackRooks.getBitboard(),
    blackPawns.getBitboard()};
  std::string materialString = BoardUtils::buildRemainingMaterial(boards);
  return std::find(BoardUtils::drawnPieceSetups.begin(), BoardUtils::drawnPieceSetups.end(), materialString) != BoardUtils::drawnPieceSetups.end();
}
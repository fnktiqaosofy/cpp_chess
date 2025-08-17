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
    blackKing(PiecePair::BLACK),
    whiteCastledK(false),
    whiteCastledQ(false),
    blackCastledK(false),
    blackCastledQ(false),
    toMove(Board::Turn::WHITE),
    whitePieces(0x000000000000FFFF),
    blackPieces(0xFFFF000000000000)
{
  pieceTable[PiecePair::WHITE][PiecePair::PAWN] = &whitePawns;
  pieceTable[PiecePair::BLACK][PiecePair::PAWN] = &blackPawns;
  pieceTable[PiecePair::WHITE][PiecePair::ROOK] = &whiteRooks;
  pieceTable[PiecePair::BLACK][PiecePair::ROOK] = &blackRooks;
  pieceTable[PiecePair::WHITE][PiecePair::KNIGHT] = &whiteKnights;
  pieceTable[PiecePair::BLACK][PiecePair::KNIGHT] = &blackKnights;
  pieceTable[PiecePair::WHITE][PiecePair::BISHOP] = &whiteBishops;
  pieceTable[PiecePair::BLACK][PiecePair::BISHOP] = &blackBishops;
  pieceTable[PiecePair::WHITE][PiecePair::QUEEN] = &whiteQueen;
  pieceTable[PiecePair::BLACK][PiecePair::QUEEN] = &blackQueen;
  pieceTable[PiecePair::WHITE][PiecePair::KING] = &whiteKing;
  pieceTable[PiecePair::BLACK][PiecePair::KING] = &blackKing;
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


bool Board::checkMoveLegality(Turn color, int from, int to)
{
  uint64_t moveMask;
  moveMask = (1ULL << to) | (1ULL << from);
  uint64_t tempBoard = (color == Turn::WHITE) ? whitePieces : blackPieces;
  uint64_t newBoardState = tempBoard ^ moveMask;

  std::vector<std::pair<int,int>> moveList;
  uint64_t attackingPieces = (color == Turn::WHITE) ? blackPieces : whitePieces;
  uint64_t defendingPieces = newBoardState;
  
  auto attackingPawns = (color == Turn::WHITE) ? blackPawns : whitePawns;
  auto attackingKnights = (color == Turn::WHITE) ? blackKnights : whiteKnights;
  auto attackingBishops = (color == Turn::WHITE) ? blackBishops : whiteBishops;
  auto attackingRooks = (color == Turn::WHITE) ? blackRooks : whiteRooks;
  auto attackingQueen = (color == Turn::WHITE) ? blackQueen : whiteQueen;
  auto attackingKing = (color == Turn::WHITE) ? blackKing : whiteKing;

  auto defendingKing = (color == Turn::WHITE) ? whiteKing : blackKing;

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

void Board::moveCommand(char symbol, int from, int to, Turn color)
{
  if(from == 0) {whiteCastledQ = true;}
  if(from == 7) {whiteCastledK = true;}
  if(from == 4) {whiteCastledK = true; whiteCastledQ = true;}
  if(from == 60) {blackCastledK = true; blackCastledQ = true;}
  if(from == 63) {blackCastledK = true;}
  if(from == 56) {blackCastledQ = true;}

  PiecePair::PieceType piece = BoardUtils::charToPieceType(symbol);
  PiecePair::Color colorP = (toMove == Turn::WHITE) ? PiecePair::WHITE : PiecePair::BLACK;
  
  pieceTable[colorP][piece]->applyMove(from, to);

  
  uint64_t attackedPieces = (color == Turn::WHITE) ? blackPieces : whitePieces;
  if ((attackedPieces >> to) & 1)
  {
    auto& attackingPawns = (color == Turn::WHITE) ? blackPawns : whitePawns;
    auto& attackingKnights = (color == Turn::WHITE) ? blackKnights : whiteKnights;
    auto& attackingBishops = (color == Turn::WHITE) ? blackBishops : whiteBishops;
    auto& attackingRooks = (color == Turn::WHITE) ? blackRooks : whiteRooks;
    auto& attackingQueen = (color == Turn::WHITE) ? blackQueen : whiteQueen;
    auto& attackingKing = (color == Turn::WHITE) ? blackKing : whiteKing;
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


int Board::castleCommand(std::string command, Turn color)
{
  if (command.length() == 5)
  {
    if (isCastlingThreatened('Q')) {return -1;}

    if (color == Turn::WHITE && !whiteCastledQ && (whitePieces & ((1ULL << 1) | (1ULL << 2) | (1ULL << 3)) == 0))
    {
      whiteKing.applyMove(4, 2);
      whiteRooks.applyMove(0, 3);
      whitePieces ^= ((1ULL << 0) | (1ULL << 2) | (1ULL << 3) | (1ULL << 4));
      return 0;
    }
    else if (color == Turn::BLACK && !blackCastledQ && (blackPieces & ((1ULL << 57) | (1ULL << 58) | (1ULL << 59)) == 0))
    {
      blackKing.applyMove(60, 58);
      blackRooks.applyMove(56, 59);
      blackPieces ^= ((1ULL << 56) | (1ULL << 58) | (1ULL << 59) | (1ULL << 60));
      return 0;
    }
  } 
  else if (command.length() == 3)
  {
    if (isCastlingThreatened('K')) {return -1;}

    if (color == Turn::WHITE && !whiteCastledK && ((whitePieces & ((1ULL << 5) | (1ULL << 6))) == 0))
    {
      whiteKing.applyMove(4, 6);
      whiteRooks.applyMove(7, 5);
      whitePieces ^= ((1ULL << 4) | (1ULL << 5) | (1ULL << 6) | (1ULL << 7));
      return 0;
    }
    else if (color == Turn::BLACK && !blackCastledK && (whitePieces & ((1ULL << 62) | (1ULL << 63)) == 0))
    {
      blackKing.applyMove(60, 62);
      blackRooks.applyMove(63, 61);
      blackPieces ^= ((1ULL << 60) | (1ULL << 61) | (1ULL << 62) | (1ULL << 63));
      return 0;
    }
  }
  return -1;
}

void Board::promotion(int to, char symbol)
{
  auto& pawnBoard = (toMove == Turn::WHITE) ? whitePawns : blackPawns;
  pawnBoard.clearSquare(to);
  switch (symbol)
  {
    case 'Q':
    {
      auto& targetBoard = (toMove == Turn::WHITE) ? whiteQueen : blackQueen;
      targetBoard.flipBit(to);
      break;
    }
    case 'R':
    {
      auto& targetBoard = (toMove == Turn::WHITE) ? whiteRooks : blackRooks;
      targetBoard.flipBit(to);
      break;
    }
    case 'N':
    {
      auto& targetBoard = (toMove == Turn::WHITE) ? whiteKnights : blackKnights;
      targetBoard.flipBit(to);
      break;
    }
    case 'B':
    {
      auto& targetBoard = (toMove == Turn::WHITE) ? whiteBishops : blackBishops;
      targetBoard.flipBit(to);
      break;
    }
  }
}

int Board::identifyMover(Turn color, char piece, int to)
{
  std::vector<std::pair<int,int>> moves;
  switch (piece)
  {
    case 'P':
      moves = (color == Turn::WHITE) ? whitePawns.pseudoLegalMoves(whitePieces, blackPieces) : blackPawns.pseudoLegalMoves(blackPieces, whitePieces);
      break;
    case 'R':
      moves = (color == Turn::WHITE) ? whiteRooks.pseudoLegalMoves(whitePieces, blackPieces) : blackRooks.pseudoLegalMoves(blackPieces, whitePieces);
      break;
    case 'N':
      moves = (color == Turn::WHITE) ? whiteKnights.pseudoLegalMoves(whitePieces, blackPieces) : blackKnights.pseudoLegalMoves(blackPieces, whitePieces);
      break;
    case 'B':
      moves = (color == Turn::WHITE) ? whiteBishops.pseudoLegalMoves(whitePieces, blackPieces) : blackBishops.pseudoLegalMoves(blackPieces, whitePieces);
      break;
    case 'Q':
      moves = (color == Turn::WHITE) ? whiteQueen.pseudoLegalMoves(whitePieces, blackPieces) : blackQueen.pseudoLegalMoves(blackPieces, whitePieces);
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

std::vector<std::pair<int,int>> Board::getAllPseudoLegalMoves(Turn color)
{
  std::vector<std::pair<int,int>> moveList;
  uint64_t ownPieces = (color == Turn::WHITE) ? whitePieces : blackPieces;
  uint64_t opponentPieces = (color == Turn::WHITE) ? blackPieces : whitePieces;

  const auto& pawns   = (color == Turn::WHITE) ? whitePawns   : blackPawns;
  const auto& knights = (color == Turn::WHITE) ? whiteKnights : blackKnights;
  const auto& bishops = (color == Turn::WHITE) ? whiteBishops : blackBishops;
  const auto& rooks   = (color == Turn::WHITE) ? whiteRooks   : blackRooks;
  const auto& queen   = (color == Turn::WHITE) ? whiteQueen   : blackQueen;
  const auto& king    = (color == Turn::WHITE) ? whiteKing    : blackKing;

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
int Board::findKing(Turn color)
{
  if (color == Turn::WHITE)
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
  toMove = (toMove == Turn::WHITE) ? Turn::BLACK : Turn::WHITE;
}

bool Board::isSquareThreatened(int square)
{
  std::vector<std::vector<std::pair<int,int>>> allMoves;
  if (toMove == Turn::WHITE)
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

bool Board::isCastlingThreatened(char side)
{
  std::vector<std::vector<std::pair<int,int>>> allMoves;
  std::array<int,3> checkSquares;
  if (toMove == Turn::WHITE)
  {
    if (side == 'Q') {checkSquares = {2,3,4};}
    if (side == 'K') {checkSquares = {4,5,6};}
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
    if (side == 'Q') {checkSquares = {58,59,60};}
    if (side == 'K') {checkSquares = {60,61,62};}
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
      if (toSq == checkSquares[0] || toSq == checkSquares[1] || toSq == checkSquares[2])
        return true;
    }
  }
  return false;
}

bool Board::checkMate()
{
  auto allmoves = getAllPseudoLegalMoves(Board::toMove);
  for (const auto& [fromSq, toSq] : allmoves)
  {
      if (checkMoveLegality(toMove, fromSq, toSq)) {return false;}
  }
  return true;
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

Board::Turn Board::getToMove()
{
  return toMove;
}
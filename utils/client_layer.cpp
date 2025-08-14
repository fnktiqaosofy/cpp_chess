#include "client_layer.h"


Client::Client() {}

void Client::runProgram()
{
  while(true)
  {
    std::cout << '\n';
    printBoard();
    std::cout << '\n';
    if (gameBoard.checkMate())
    {
      std::string winner = (gameBoard.toMove == 0) ? "Black" : "White";
      std::cout << "Checkmate! " << winner << " has won." << '\n';
    }
    std::smatch match;
    std::string move;
    std::smatch parsed_move = getMove(match, move);
    processMoveCommand(parsed_move);
  }
}

std::smatch Client::getMove(std::smatch& match, std::string& move)
{
  while (true)
  {
    std::getline(std::cin, move);
    if (parse_move(move, match)) 
    {
        return match;
    } 
    else 
    {
        std::cout << "Invalid move\n";
    }
  }
}

void Client::printBoard()
{
  int g = 0;
  for (int i = 0; i < 64; i++)
  {
    std::cout << gameBoard.identifyPiece(i);
    ++g;
    if ((g % 8) == 0) {std::cout << '\n';}
  }
}
 // char symbol, int to, 
void Client::processMoveCommand(std::smatch& move)
{
  std::string castle = move[7].str();
  if (!castle.empty())
  {
    int check;
    check = gameBoard.castleCommand(castle, gameBoard.toMove);
    if (check == -1)
    {
      std::cout << "Cannot castle." << '\n';
      return;
    }
    gameBoard.switchMover();
  }
  else
  {
    std::string symbol_ext = move[1].str();
    char symbol = (symbol_ext.empty()) ? 'P' : symbol_ext[0];
    std::string specifier = move[2].str();
    std::string capture_mod = move[3].str();
    int to = BoardUtils::coordToIndex(move[4].str());
    std::string promo = move[5].str();

    if ((symbol == 'P') && (((to / 8) == 7) || ((to / 8) == 0)) && (promo.empty()))
    {
      std::cout << "Pawn will promote. Please indicate desired piece." << '\n';
      return;
    }
    
    int from = gameBoard.identifyMover(gameBoard.toMove, symbol, to);
    if (from == -1) {
      std::cout << "Illegal or ambiguous move." << '\n';
      return;
    }

    if (gameBoard.checkMoveLegality(gameBoard.toMove, from, to))
    {
      gameBoard.moveCommand(symbol, from, to, gameBoard.toMove);
      if ((symbol == 'P') && (((to / 8) == 7) || ((to / 8) == 0)))
      {
        gameBoard.promotion(to, promo[0]);
      }
      gameBoard.switchMover();
      return;
    }
    else
    {
      std::cout << "Move is not allowed as it leaves king in check." << '\n';
      return;
    }
  }
}


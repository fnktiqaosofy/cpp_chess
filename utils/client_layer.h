#ifndef CLIENT_LAYER_H
#define CLIENT_LAYER_H

#include "board.h"
#include "move_parser.h"
#include <iostream>

class Client
{
  public:

    Client();

    void runProgram();

    void printBoard();

    void processMoveCommand(std::smatch& move);

    std::smatch getMove(std::smatch& match, std::string& move);

  private:

    Board gameBoard;

};
#endif
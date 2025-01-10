#include <iostream>
#include <string>
#include "bitboards.h"
#include "moves.h"
#include "searcher.h"

int main()
{
  // We repeatedly read a FEN, parse it, search for the best move, and print it.
  while (true)
  {
    std::string fen;
    std::cout << "Enter FEN (or 'quit' to exit): ";
    if (!std::getline(std::cin, fen))
    {
      // End of file or stream error
      break;
    }

    if (fen == "quit" || fen == "exit")
    {
      break;
    }

    // Initialize the board from this FEN
    Bitboards board;
    board.initialize(fen);
    board.updateAttacks(); // Make sure the attacks are up to date

    // Let's pick a search depth. You can change to 4, 5, 6, etc.
    int depth = 4;

    // Find the best move
    Move bestMove = findBestMove(board, depth);

    // Convert the best move to a string
    // For demonstration, we'll do something like: "e2e4" or "e7e8Q" etc.
    std::string sourceStr = squareToString(bestMove.sourceSquare);
    std::string targetStr = squareToString(bestMove.targetSquare);
    char promoType = bestMove.moveType;

    // If it's a promotion, we'd typically do something like e7e8Q. Otherwise, e2e4.
    std::string moveNotation = sourceStr + targetStr;
    if (promoType == 'Q' || promoType == 'R' || promoType == 'B' || promoType == 'N')
    {
      moveNotation += promoType;
    }

    std::cout << "Best move: " << moveNotation << std::endl;
  }

  return 0;
}

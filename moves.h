#ifndef MOVES_H
#define MOVES_H

#include <vector>
#include <cstdint>
#include <string>

class Bitboards;

// Move representation
struct Move
{
  int sourceSquare; // Source square (0-63)
  int targetSquare; // Target square (0-63)
  char moveType;    // 'Q' for queen promotion, 'N' for knight, etc.
  bool isCapture;   // True if the move captures an opponent's piece
};

inline bool operator==(const Move &a, const Move &b)
{
  return a.sourceSquare == b.sourceSquare && a.targetSquare == b.targetSquare && a.moveType == b.moveType && a.isCapture == b.isCapture;
}

// Move generation functions
std::vector<Move> generateLegalMoves(const Bitboards &board, bool isWhite);
void generatePawnMoves(Bitboards board, bool isWhite, std::vector<Move> &moves);
void generateKnightMoves(Bitboards board, bool isWhite, std::vector<Move> &moves);
void generateBishopMoves(Bitboards board, bool isWhite, std::vector<Move> &moves);
void generateRookMoves(Bitboards board, bool isWhite, std::vector<Move> &moves);
void generateQueenMoves(Bitboards board, bool isWhite, std::vector<Move> &moves);
void generateKingMoves(Bitboards board, bool isWhite, std::vector<Move> &moves);
void printMoves(const std::vector<Move> &moves);
std::string squareToString(int square);

#endif // MOVES_H

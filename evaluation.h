// evaluation.h
#ifndef EVALUATION_H
#define EVALUATION_H

#include <cstdint>

enum PieceType
{
  PAWN,
  KNIGHT,
  BISHOP,
  ROOK,
  QUEEN,
  KING,
};

enum Color
{
  WHITE,
  BLACK,
};

double pieceEvaluation(uint64_t bitboard, const double PSQ[64]);

double evaluateBoard(Bitboards bitboard);

#endif

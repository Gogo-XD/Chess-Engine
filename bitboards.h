
#ifndef BITBOARDS_H
#define BITBOARDS_H

#include <cstdint>
#include <string>

class Move;

using namespace std;

class Bitboards
{
public:
  uint64_t whitePawns, whiteRooks, whiteKnights, whiteBishops, whiteQueens, whiteKings;
  uint64_t blackPawns, blackRooks, blackKnights, blackBishops, blackQueens, blackKings;
  uint64_t whitePieces, blackPieces;

  uint64_t whitePawnAttacks, whiteRookAttacks, whiteKnightAttacks, whiteBishopAttacks, whiteQueenAttacks, whiteKingAttacks;
  uint64_t blackPawnAttacks, blackRookAttacks, blackKnightAttacks, blackBishopAttacks, blackQueenAttacks, blackKingAttacks;
  uint64_t whitePieceAttacks, blackPieceAttacks;

  int enPassantSquare;
  bool whiteToMove;

  bool whiteKingCastle;
  bool whiteQueenCastle;
  bool blackKingCastle;
  bool blackQueenCastle;

  Bitboards();

  void initialize(string fen);

  void updateAttacks();

  void printBitboards();

  Bitboards simulateMove(const Move &move);

  uint64_t generatePawnAttacks(uint64_t pawns, bool isWhite);
  uint64_t generateKnightAttacks(uint64_t knights, bool isWhite);
  uint64_t generateBishopAttacks(uint64_t bishops, uint64_t occupied, bool isWhite);
  uint64_t generateRookAttacks(uint64_t rooks, uint64_t occupied, bool isWhite);
  uint64_t generateQueenAttacks(uint64_t queens, uint64_t occupied, bool isWhite);
  uint64_t generateKingAttacks(uint64_t king, bool isWhite);
  uint64_t generateSlidingAttacks(uint64_t piece, uint64_t occupied, uint64_t friendlies, bool diagonal);
  bool checkmate;
  bool stalemate;

private:
  void setBit(uint64_t &bitboard, int square);
};

bool checks(uint64_t &bitboard);

#endif // BITBOARDS_H

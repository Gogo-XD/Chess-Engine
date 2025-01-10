#include "moves.h"
#include "bitboards.h"
#include <iostream>

void generatePawnMoves(Bitboards board, bool isWhite, std::vector<Move> &moves)
{
  uint64_t pawns = isWhite ? board.whitePawns : board.blackPawns;
  uint64_t opponentPieces = isWhite ? board.blackPieces : board.whitePieces;
  uint64_t emptySquares = ~(board.whitePieces | board.blackPieces);

  // Single push: Shift pawns forward by 8 squares, ensuring the target square is empty
  uint64_t singlePush = isWhite ? (pawns >> 8) & emptySquares : (pawns << 8) & emptySquares;

  // Double push: Only for pawns on their starting rank, ensuring both intermediate and target squares are empty
  uint64_t doublePush = isWhite
                            ? (((singlePush >> 8) & emptySquares) & 0x00000000FF00000000ULL)  // White pawns on rank 2
                            : (((singlePush << 8) & emptySquares) & 0x0000000000FF000000ULL); // Black pawns on rank 7

  // Add single pushes
  while (singlePush)
  {
    int targetSquare = __builtin_ctzll(singlePush);                   // Target square index
    int sourceSquare = isWhite ? targetSquare + 8 : targetSquare - 8; // Source square index

    // Promotion check
    if (targetSquare / 8 == (isWhite ? 7 : 0))
    {
      // Add promotion moves
      moves.push_back({sourceSquare, targetSquare, 'Q', false});
      moves.push_back({sourceSquare, targetSquare, 'N', false});
      moves.push_back({sourceSquare, targetSquare, 'R', false});
      moves.push_back({sourceSquare, targetSquare, 'B', false});
    }
    else
    {
      // Regular single push
      moves.push_back({sourceSquare, targetSquare, ' ', false});
    }

    singlePush &= singlePush - 1; // Remove the processed bit
  }

  // Add double pushes
  while (doublePush)
  {
    int targetSquare = __builtin_ctzll(doublePush);                     // Target square index
    int sourceSquare = isWhite ? targetSquare + 16 : targetSquare - 16; // Source square index

    moves.push_back({sourceSquare, targetSquare, 'D', false});
    doublePush &= doublePush - 1; // Remove the processed bit
  }

  // Add captures (diagonal attacks)
  uint64_t attacks = isWhite ? board.generatePawnAttacks(pawns, true)
                             : board.generatePawnAttacks(pawns, false);
  uint64_t captures = attacks & opponentPieces;
  while (captures)
  {
    int targetSquare = __builtin_ctzll(captures); // Target square index
    uint64_t sourceMask = isWhite
                              ? ((pawns & ((1ULL << targetSquare) >> 7)) ? ((1ULL << targetSquare) >> 7) : ((1ULL << targetSquare) >> 9))
                              : ((pawns & ((1ULL << targetSquare) << 7)) ? ((1ULL << targetSquare) << 7) : ((1ULL << targetSquare) << 9));
    int sourceSquare = __builtin_ctzll(sourceMask); // Source square index

    moves.push_back({sourceSquare, targetSquare, ' ', true});
    captures &= captures - 1; // Remove the processed bit
  }

  if (board.enPassantSquare != -1)
  {
    uint64_t enPassantMask = 1ULL << board.enPassantSquare;
    uint64_t enPassantPawns = isWhite
                                  ? ((pawns & ((enPassantMask << 7) | (enPassantMask << 9))) & ~0x0101010101010101ULL & ~0x8080808080808080ULL)
                                  : ((pawns & ((enPassantMask >> 7) | (enPassantMask >> 9))) & ~0x0101010101010101ULL & ~0x8080808080808080ULL);

    while (enPassantPawns)
    {
      int sourceSquare = __builtin_ctzll(enPassantPawns);
      moves.push_back({sourceSquare, board.enPassantSquare, ' ', true});
      enPassantPawns &= enPassantPawns - 1;
    }
  }
}

void generateKnightMoves(Bitboards board, bool isWhite, std::vector<Move> &moves)
{
  uint64_t knights = isWhite ? board.whiteKnights : board.blackKnights;
  uint64_t friendlies = isWhite ? board.whitePieces : board.blackPieces;

  while (knights)
  {
    int sourceSquare = __builtin_ctzll(knights);
    uint64_t attacks = board.generateKnightAttacks(1ULL << sourceSquare, isWhite) & ~friendlies;

    while (attacks)
    {
      int targetSquare = __builtin_ctzll(attacks);
      bool isCapture = ((1ULL << targetSquare) & (isWhite ? board.blackPieces : board.whitePieces));
      moves.push_back({sourceSquare, targetSquare, ' ', isCapture});
      attacks &= attacks - 1;
    }
    knights &= knights - 1;
  }
}

void generateBishopMoves(Bitboards board, bool isWhite, std::vector<Move> &moves)
{
  uint64_t bishops = isWhite ? board.whiteBishops : board.blackBishops;
  uint64_t attacks = isWhite ? board.generateBishopAttacks(bishops, board.whitePieces | board.blackPieces, true)
                             : board.generateBishopAttacks(bishops, board.whitePieces | board.blackPieces, false);
  uint64_t friendlies = isWhite ? board.whitePieces : board.blackPieces;

  while (bishops)
  {
    int sourceSquare = __builtin_ctzll(bishops);
    uint64_t pieceAttacks = board.generateBishopAttacks(1ULL << sourceSquare, board.whitePieces | board.blackPieces, isWhite) & ~friendlies;

    while (pieceAttacks)
    {
      int targetSquare = __builtin_ctzll(pieceAttacks);
      bool isCapture = ((1ULL << targetSquare) & (isWhite ? board.blackPieces : board.whitePieces));
      moves.push_back({sourceSquare, targetSquare, ' ', isCapture});
      pieceAttacks &= pieceAttacks - 1;
    }
    bishops &= bishops - 1;
  }
}

void generateRookMoves(Bitboards board, bool isWhite, std::vector<Move> &moves)
{
  uint64_t rooks = isWhite ? board.whiteRooks : board.blackRooks;
  uint64_t attacks = isWhite ? board.generateRookAttacks(rooks, board.whitePieces | board.blackPieces, true)
                             : board.generateRookAttacks(rooks, board.whitePieces | board.blackPieces, false);
  uint64_t friendlies = isWhite ? board.whitePieces : board.blackPieces;

  while (rooks)
  {
    int sourceSquare = __builtin_ctzll(rooks);
    uint64_t pieceAttacks = board.generateRookAttacks(1ULL << sourceSquare, board.whitePieces | board.blackPieces, isWhite) & ~friendlies;

    while (pieceAttacks)
    {
      int targetSquare = __builtin_ctzll(pieceAttacks);
      bool isCapture = ((1ULL << targetSquare) & (isWhite ? board.blackPieces : board.whitePieces));
      moves.push_back({sourceSquare, targetSquare, ' ', isCapture});
      pieceAttacks &= pieceAttacks - 1;
    }
    rooks &= rooks - 1;
  }
}

void generateQueenMoves(Bitboards board, bool isWhite, std::vector<Move> &moves)
{
  uint64_t queens = isWhite ? board.whiteQueens : board.blackQueens;
  uint64_t friendlies = isWhite ? board.whitePieces : board.blackPieces;

  while (queens)
  {
    int sourceSquare = __builtin_ctzll(queens);
    uint64_t diagonalAttacks = board.generateBishopAttacks(1ULL << sourceSquare, board.whitePieces | board.blackPieces, isWhite) & ~friendlies;
    uint64_t straightAttacks = board.generateRookAttacks(1ULL << sourceSquare, board.whitePieces | board.blackPieces, isWhite) & ~friendlies;

    // Combine diagonal and straight attacks
    uint64_t pieceAttacks = diagonalAttacks | straightAttacks;

    while (pieceAttacks)
    {
      int targetSquare = __builtin_ctzll(pieceAttacks);
      bool isCapture = ((1ULL << targetSquare) & (isWhite ? board.blackPieces : board.whitePieces));
      moves.push_back({sourceSquare, targetSquare, ' ', isCapture});
      pieceAttacks &= pieceAttacks - 1;
    }
    queens &= queens - 1;
  }
}

void generateKingMoves(Bitboards board, bool isWhite, std::vector<Move> &moves)
{
  uint64_t king = isWhite ? board.whiteKings : board.blackKings;
  uint64_t friendlies = isWhite ? board.whitePieces : board.blackPieces;

  int sourceSquare = __builtin_ctzll(king);
  uint64_t attacks = board.generateKingAttacks(king, isWhite) & ~friendlies;

  while (attacks)
  {
    int targetSquare = __builtin_ctzll(attacks);
    bool isCapture = ((1ULL << targetSquare) & (isWhite ? board.blackPieces : board.whitePieces));
    moves.push_back({sourceSquare, targetSquare, ' ', isCapture});
    attacks &= attacks - 1;
  }
  // Add castling moves
  if (isWhite)
  {
    // Kingside Castling
    if (board.whiteKingCastle &&
        !(board.whitePieces & 0x6000000000000000ULL) &&
        !(board.blackPieceAttacks & 0x7000000000000000ULL))
    {
      moves.push_back({sourceSquare, 62, 'O', false});
    }

    // Queenside Castling
    if (board.whiteQueenCastle &&
        !(board.whitePieces & 0x0C00000000000000ULL) &&
        !(board.blackPieceAttacks & 0x1C00000000000000ULL))
    {
      moves.push_back({sourceSquare, 58, 'O', false});
    }
  }
  else
  {
    // Kingside Castling
    if (board.blackKingCastle &&
        !(board.blackPieces & 0x0000000000000060ULL) &&
        !(board.whitePieceAttacks & 0x0000000000000070ULL))
    {
      moves.push_back({sourceSquare, 6, 'O', false});
    }

    // Queenside Castling
    if (board.blackQueenCastle &&
        !(board.blackPieces & 0x000000000000000CULL) &&
        !(board.whitePieceAttacks & 0x000000000000001CULL))
    {
      moves.push_back({sourceSquare, 2, 'O', false});
    }
  }
}

std::vector<Move> generateLegalMoves(const Bitboards &board, bool isWhite)
{
  std::vector<Move> moves;

  generatePawnMoves(board, isWhite, moves);
  generateKnightMoves(board, isWhite, moves);
  generateBishopMoves(board, isWhite, moves);
  generateRookMoves(board, isWhite, moves);
  generateQueenMoves(board, isWhite, moves);
  generateKingMoves(board, isWhite, moves);

  return moves;
}

std::string squareToString(int square)
{
  char file = 'a' + (square % 8); // File (a-h)
  char rank = '8' - (square / 8); // Rank (1-8)
  return std::string(1, file) + std::string(1, rank);
}

void printMoves(const std::vector<Move> &moves)
{
  for (const auto &move : moves)
  {
    std::string moveStr = squareToString(move.sourceSquare) +
                          "-" +
                          squareToString(move.targetSquare);

    if (move.isCapture)
    {
      moveStr += " (capture)";
    }

    if (move.moveType != ' ')
    {
      moveStr += " [" + std::string(1, move.moveType) + "]";
    }

    std::cout << moveStr << "\n";
  }
}

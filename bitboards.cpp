// Example method implementation
#include "bitboards.h"
#include "moves.h"
#include <sstream>
#include <cctype>
#include <iostream>
#include <iomanip>
#include <cstdint>

Bitboards::Bitboards()
    : whitePawns(0), whiteRooks(0), whiteKnights(0), whiteBishops(0),
      whiteQueens(0), whiteKings(0), blackPawns(0), blackRooks(0),
      blackKnights(0), blackBishops(0), blackQueens(0), blackKings(0),
      whitePieces(0), blackPieces(0),
      whitePawnAttacks(0), whiteRookAttacks(0), whiteKnightAttacks(0), whiteBishopAttacks(0),
      whiteQueenAttacks(0), whiteKingAttacks(0), blackPawnAttacks(0), blackRookAttacks(0),
      blackKnightAttacks(0), blackBishopAttacks(0), blackQueenAttacks(0), blackKingAttacks(0),
      whitePieceAttacks(0), blackPieceAttacks(0),
      whiteToMove(true), enPassantSquare(-1),
      whiteKingCastle(false), whiteQueenCastle(false), blackKingCastle(false), blackQueenCastle(false),
      checkmate(false), stalemate(false)
{
}

void Bitboards::setBit(uint64_t &bitboard, int square)
{
  bitboard |= (1ULL << square);
}

void Bitboards::initialize(string fen)
{
  whitePawns = whiteRooks = whiteKnights = whiteBishops = whiteQueens = whiteKings = 0;
  blackPawns = blackRooks = blackKnights = blackBishops = blackQueens = blackKings = 0;
  whitePieces = blackPieces = 0;

  whitePawnAttacks = whiteRookAttacks = whiteKnightAttacks = whiteBishopAttacks = whiteQueenAttacks = whiteKingAttacks = 0;
  blackPawnAttacks = blackRookAttacks = blackKnightAttacks = blackBishopAttacks = blackQueenAttacks = blackKingAttacks = 0;
  whitePieceAttacks = blackPieceAttacks = 0;

  checkmate = false;
  stalemate = false;

  int length = fen.length();
  int index = 0;
  int square = 0;

  for (; index < length; index++)
  {
    char c = fen[index];
    if (c == ' ')
    {
      break;
    }
    if (c == '/')
    {
      continue;
    }
    else if (std::isdigit(c))
    {
      square += c - '0';
    }
    else
    {
      switch (c)
      {
      case 'P':
        setBit(whitePawns, square);
        break;
      case 'N':
        setBit(whiteKnights, square);
        break;
      case 'B':
        setBit(whiteBishops, square);
        break;
      case 'R':
        setBit(whiteRooks, square);
        break;
      case 'Q':
        setBit(whiteQueens, square);
        break;
      case 'K':
        setBit(whiteKings, square);
        break;
      case 'p':
        setBit(blackPawns, square);
        break;
      case 'n':
        setBit(blackKnights, square);
        break;
      case 'b':
        setBit(blackBishops, square);
        break;
      case 'r':
        setBit(blackRooks, square);
        break;
      case 'q':
        setBit(blackQueens, square);
        break;
      case 'k':
        setBit(blackKings, square);
        break;
      default:
        break;
      }

      square++;
    }
  }

  index++;
  if (fen[index] == 'w')
  {
    whiteToMove = true;
  }
  else if (fen[index == 'b'])
  {
    whiteToMove = false;
  }

  for (index += 2; index < length; index++)
  {
    char c = fen[index];
    if (c == ' ')
    {
      break;
    }

    switch (c)
    {
    case 'K':
      whiteKingCastle = true;
      break;
    case 'Q':
      whiteQueenCastle = true;
      break;
    case 'k':
      blackKingCastle = true;
      break;
    case 'q':
      blackQueenCastle = true;
      break;
    default:
      break;
    }
  }

  index++;
  if (fen[index] != '-')
  {
    int file = fen[index++] - 'a';
    int rank = fen[index++] - '0';
    enPassantSquare = (8 - rank) * 8 + file;
  }

  whitePieces = whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKings;
  blackPieces = blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKings;
}

void Bitboards::updateAttacks()
{
  uint64_t occupied = whitePieces | blackPieces;

  whitePawnAttacks = generatePawnAttacks(whitePawns, true);
  blackPawnAttacks = generatePawnAttacks(blackPawns, false);

  whiteKnightAttacks = generateKnightAttacks(whiteKnights, true);
  blackKnightAttacks = generateKnightAttacks(blackKnights, false);

  whiteBishopAttacks = generateBishopAttacks(whiteBishops, occupied, true);
  blackBishopAttacks = generateBishopAttacks(blackBishops, occupied, false);

  whiteRookAttacks = generateRookAttacks(whiteRooks, occupied, true);
  blackRookAttacks = generateRookAttacks(blackRooks, occupied, false);

  whiteQueenAttacks = generateQueenAttacks(whiteQueens, occupied, true);
  blackQueenAttacks = generateQueenAttacks(blackQueens, occupied, false);

  whiteKingAttacks = generateKingAttacks(whiteKings, true);
  blackKingAttacks = generateKingAttacks(blackKings, false);

  whitePieceAttacks = whitePawnAttacks | whiteRookAttacks | whiteKnightAttacks | whiteBishopAttacks | whiteQueenAttacks | whiteKingAttacks;
  blackPieceAttacks = blackPawnAttacks | blackRookAttacks | blackKnightAttacks | blackBishopAttacks | blackQueenAttacks | blackKingAttacks;
}

uint64_t Bitboards::generatePawnAttacks(uint64_t pawns, bool isWhite)
{
  uint64_t attacks = 0;
  uint64_t friendlies = isWhite ? whitePieces : blackPieces;

  if (isWhite)
  {
    // White pawns attack diagonally downwards (to lower ranks)
    attacks |= (pawns >> 7) & ~0x0101010101010101ULL; // Capture left
    attacks |= (pawns >> 9) & ~0x8080808080808080ULL; // Capture right
  }
  else
  {
    // Black pawns attack diagonally upwards (to higher ranks)
    attacks |= (pawns << 9) & ~0x0101010101010101ULL; // Capture left
    attacks |= (pawns << 7) & ~0x8080808080808080ULL; // Capture right
  }

  // Remove attacks on friendly pieces
  attacks &= ~friendlies;

  return attacks;
}

uint64_t Bitboards::generateKnightAttacks(uint64_t knights, bool isWhite)
{
  uint64_t attacks = 0;
  uint64_t friendlies = isWhite ? whitePieces : blackPieces;
  while (knights)
  {
    int square = __builtin_ctzll(knights);
    uint64_t singleKnight = 1ULL << square;

    uint64_t knightMoves =
        ((singleKnight << 17) & ~0x0101010101010101ULL) |
        ((singleKnight << 15) & ~0x8080808080808080ULL) |
        ((singleKnight << 10) & ~0x0303030303030303ULL) |
        ((singleKnight << 6) & ~0xC0C0C0C0C0C0C0C0ULL) |
        ((singleKnight >> 17) & ~0x8080808080808080ULL) |
        ((singleKnight >> 15) & ~0x0101010101010101ULL) |
        ((singleKnight >> 10) & ~0xC0C0C0C0C0C0C0C0ULL) |
        ((singleKnight >> 6) & ~0x0303030303030303ULL);

    // Remove attacks on friendly pieces
    knightMoves &= ~friendlies;

    attacks |= knightMoves;
    knights &= knights - 1;
  }
  return attacks;
}

uint64_t Bitboards::generateSlidingAttacks(uint64_t piece, uint64_t occupied, uint64_t friendlies, bool diagonal)
{
  uint64_t attacks = 0;
  int directions[4];
  if (diagonal)
  {
    directions[0] = 9;
    directions[1] = 7;
    directions[2] = -7;
    directions[3] = -9;
  }
  else
  {
    directions[0] = 8;
    directions[1] = -8;
    directions[2] = 1;
    directions[3] = -1;
  }

  for (int dir : directions)
  {
    uint64_t attackRay = piece;
    while (true)
    {
      // Check if moving in this direction would wrap around
      if ((dir == 1 || dir == -7 || dir == 9) && (attackRay & 0x8080808080808080ULL))
        break; // H-file
      if ((dir == -1 || dir == -9 || dir == 7) && (attackRay & 0x0101010101010101ULL))
        break; // A-file

      // Apply the shift
      if (dir > 0)
      {
        attackRay = attackRay << dir;
      }
      else
      {
        attackRay = attackRay >> -dir;
      }

      // Stop if the shift goes out of bounds
      if (attackRay == 0)
        break;

      // Add the attack ray to the attacks
      attacks |= attackRay;

      // Stop extending if the ray hits an occupied square
      if (attackRay & occupied)
        break;
    }
  }

  // Remove attacks on friendly pieces
  attacks &= ~friendlies;

  return attacks;
}

uint64_t Bitboards::generateBishopAttacks(uint64_t bishops, uint64_t occupied, bool isWhite)
{
  uint64_t attacks = 0;
  uint64_t friendlies = isWhite ? whitePieces : blackPieces;

  while (bishops)
  {
    int square = __builtin_ctzll(bishops);
    uint64_t singleBishop = 1ULL << square;
    attacks |= generateSlidingAttacks(singleBishop, occupied, friendlies, true); // Diagonals
    bishops &= bishops - 1;
  }
  return attacks;
}

uint64_t Bitboards::generateRookAttacks(uint64_t rooks, uint64_t occupied, bool isWhite)
{
  uint64_t attacks = 0;
  uint64_t friendlies = isWhite ? whitePieces : blackPieces;

  while (rooks)
  {
    int square = __builtin_ctzll(rooks);
    uint64_t singleRook = 1ULL << square;
    attacks |= generateSlidingAttacks(singleRook, occupied, friendlies, false); // Straights
    rooks &= rooks - 1;
  }
  return attacks;
}

uint64_t Bitboards::generateQueenAttacks(uint64_t queens, uint64_t occupied, bool isWhite)
{
  uint64_t attacks = 0;
  uint64_t friendlies = isWhite ? whitePieces : blackPieces;

  while (queens)
  {
    int square = __builtin_ctzll(queens);
    uint64_t singleQueen = 1ULL << square;
    attacks |= generateSlidingAttacks(singleQueen, occupied, friendlies, true);  // Diagonals
    attacks |= generateSlidingAttacks(singleQueen, occupied, friendlies, false); // Straights
    queens &= queens - 1;
  }
  return attacks;
}

uint64_t Bitboards::generateKingAttacks(uint64_t king, bool isWhite)
{
  uint64_t friendlies = isWhite ? whitePieces : blackPieces;
  uint64_t attacks = 0;
  if (king)
  {
    int square = __builtin_ctzll(king);
    uint64_t singleKing = 1ULL << square;

    uint64_t kingMoves =
        (singleKing << 8) |
        (singleKing >> 8) |
        (singleKing << 1) |
        (singleKing >> 1) |
        (singleKing << 9) |
        (singleKing << 7) |
        (singleKing >> 7) |
        (singleKing >> 9);

    // Remove attacks on friendly pieces
    kingMoves &= ~friendlies;

    attacks |= kingMoves;
  }
  return attacks;
}

void Bitboards::printBitboards()
{
  struct BitboardInfo
  {
    std::string label;
    uint64_t bitboard;
    uint64_t attacks;
  } bitboardsInfo[] = {
      {"White Pawns", whitePawns, whitePawnAttacks},
      {"White Rooks", whiteRooks, whiteRookAttacks},
      {"White Knights", whiteKnights, whiteKnightAttacks},
      {"White Bishops", whiteBishops, whiteBishopAttacks},
      {"White Queens", whiteQueens, whiteQueenAttacks},
      {"White Kings", whiteKings, whiteKingAttacks},
      {"White Pieces", whitePieces, whitePieceAttacks},
      {"Black Pawns", blackPawns, blackPawnAttacks},
      {"Black Rooks", blackRooks, blackRookAttacks},
      {"Black Knights", blackKnights, blackKnightAttacks},
      {"Black Bishops", blackBishops, blackBishopAttacks},
      {"Black Queens", blackQueens, blackQueenAttacks},
      {"Black Kings", blackKings, blackKingAttacks},
      {"Black Pieces", blackPieces, blackPieceAttacks},
  };

  for (int rank = 7; rank >= 0; rank--)
  {
    for (int file = 0; file < 8; file++)
    {
      std::cout << ((7 - rank) * 8 + file) << " ";
    }
    std::cout << "\n";
  }

  for (const auto &bb : bitboardsInfo)
  {
    std::cout << bb.label << ": " << bb.bitboard << "\n";

    for (int rank = 7; rank >= 0; --rank)
    {
      for (int file = 0; file < 8; ++file)
      {
        int square = (7 - rank) * 8 + file;
        bool isSet = (bb.bitboard >> square) & 1ULL;
        std::cout << (isSet ? "1 " : ". ");
      }

      std::cout << "  |  ";
      for (int file = 0; file < 8; ++file)
      {
        int square = (7 - rank) * 8 + file;
        bool isSet = (bb.attacks >> square) & 1ULL;
        std::cout << (isSet ? "1 " : ". ");
      }

      std::cout << "\n";
    }
    std::cout << "\n";
  }
}

Bitboards Bitboards::simulateMove(const Move &move)
{
  Bitboards newBoard = *this;

  uint64_t sourceMask = (1ULL << move.sourceSquare);
  uint64_t targetMask = (1ULL << move.targetSquare);

  // Handle captures: Remove the captured piece from its bitboard
  if (move.isCapture)
  {
    if (whiteToMove)
    {
      // Remove the captured piece from black's bitboards
      if (newBoard.blackPawns & targetMask)
        newBoard.blackPawns &= ~targetMask;
      else if (newBoard.blackKnights & targetMask)
        newBoard.blackKnights &= ~targetMask;
      else if (newBoard.blackBishops & targetMask)
        newBoard.blackBishops &= ~targetMask;
      else if (newBoard.blackRooks & targetMask)
        newBoard.blackRooks &= ~targetMask;
      else if (newBoard.blackQueens & targetMask)
        newBoard.blackQueens &= ~targetMask;
      else if (newBoard.blackKings & targetMask)
        newBoard.blackKings &= ~targetMask;
    }
    else
    {
      // Remove the captured piece from white's bitboards
      if (newBoard.whitePawns & targetMask)
        newBoard.whitePawns &= ~targetMask;
      else if (newBoard.whiteKnights & targetMask)
        newBoard.whiteKnights &= ~targetMask;
      else if (newBoard.whiteBishops & targetMask)
        newBoard.whiteBishops &= ~targetMask;
      else if (newBoard.whiteRooks & targetMask)
        newBoard.whiteRooks &= ~targetMask;
      else if (newBoard.whiteQueens & targetMask)
        newBoard.whiteQueens &= ~targetMask;
      else if (newBoard.whiteKings & targetMask)
        newBoard.whiteKings &= ~targetMask;
    }
  }

  if (move.moveType == 'O')
  {
    if (whiteToMove)
    {
      newBoard.whiteKingCastle = false;
      newBoard.whiteQueenCastle = false;
      if (move.targetSquare == 62)
      { // Kingside castling for white (e1 to g1)
        // Move the king
        newBoard.whiteKings &= sourceMask;
        newBoard.whiteKings |= targetMask;

        // Move the rook (h1 to f1)
        newBoard.whiteRooks &= ~(1ULL << 63);
        newBoard.whiteRooks |= (1ULL << 61);
      }
      else if (move.targetSquare == 58)
      { // Queenside castling for white (e1 to c1)
        // Move the king
        newBoard.whiteKings &= sourceMask;
        newBoard.whiteKings |= targetMask;

        // Move the rook (a1 to d1)
        newBoard.whiteRooks &= ~(1ULL << 56);
        newBoard.whiteRooks |= (1ULL << 59);
      }
    }
    else
    {
      newBoard.blackKingCastle = false;
      newBoard.blackQueenCastle = false;
      if (move.targetSquare == 2)
      { // Kingside castling for black (e8 to g8)
        // Move the king
        newBoard.blackKings &= sourceMask;
        newBoard.blackKings |= targetMask;

        // Move the rook (h8 to f8)
        newBoard.blackRooks &= ~(1ULL >> 0);
        newBoard.blackRooks |= (1ULL >> 3);
      }
      else if (move.targetSquare == 6)
      { // Queenside castling for black (e8 to c8)
        // Move the king
        newBoard.blackKings &= sourceMask;
        newBoard.blackKings |= targetMask;

        // Move the rook (a8 to d8)
        newBoard.blackRooks &= ~(1ULL >> 7);
        newBoard.blackRooks |= (1ULL >> 5);
      }
    }
  }

  // Update the moving piece
  else if (whiteToMove)
  {
    if (newBoard.whitePawns & (1ULL << move.sourceSquare))
    {
      newBoard.whitePawns &= sourceMask;
      char c = move.moveType;
      if (c != ' ')
      {
        switch (c)
        {
        case 'Q':
          newBoard.whiteQueens |= targetMask;
          break;
        case 'N':
          newBoard.whiteKnights |= targetMask;
          break;
        case 'B':
          newBoard.whiteBishops |= targetMask;
          break;
        case 'R':
          newBoard.whiteRooks |= targetMask;
          break;
        case 'D':
          enPassantSquare = move.targetSquare + 8;
        default:
          break;
        }
      }
      else
      {
        newBoard.whitePawns |= targetMask;
      }
    }
    else if (newBoard.whiteKnights & (1ULL << move.sourceSquare))
    {
      newBoard.whiteKnights &= sourceMask;
      newBoard.whiteKnights |= targetMask;
    }
    else if (newBoard.whiteBishops & (1ULL << move.sourceSquare))
    {
      newBoard.whiteBishops &= sourceMask;
      newBoard.whiteBishops |= targetMask;
    }
    else if (newBoard.whiteRooks & (1ULL << move.sourceSquare))
    {
      newBoard.whiteRooks &= sourceMask;
      newBoard.whiteRooks |= targetMask;
      if (move.sourceSquare == 56)
      {
        newBoard.whiteQueenCastle = false;
      }
      else if (move.sourceSquare == 63)
      {
        newBoard.whiteKingCastle = false;
      }
    }
    else if (newBoard.whiteQueens & (1ULL << move.sourceSquare))
    {
      newBoard.whiteQueens &= sourceMask;
      newBoard.whiteQueens |= targetMask;
    }
    else if (newBoard.whiteKings & (1ULL << move.sourceSquare))
    {
      newBoard.whiteKings &= sourceMask;
      newBoard.whiteKings |= targetMask;
    }
  }
  else
  {
    if (newBoard.blackPawns & (1ULL << move.sourceSquare))
    {
      newBoard.blackPawns &= sourceMask;
      char c = move.moveType;
      if (c != ' ')
      {
        switch (c)
        {
        case 'Q':
          newBoard.blackQueens |= targetMask;
          break;
        case 'N':
          newBoard.blackKnights |= targetMask;
          break;
        case 'B':
          newBoard.blackBishops |= targetMask;
          break;
        case 'R':
          newBoard.blackRooks |= targetMask;
          break;
        case 'D':
          enPassantSquare = move.targetSquare - 8;
        default:
          break;
        }
      }
      else
      {
        newBoard.blackPawns |= targetMask;
      }
    }
    else if (newBoard.blackKnights & (1ULL << move.sourceSquare))
    {
      newBoard.blackKnights &= sourceMask;
      newBoard.blackKnights |= targetMask;
    }
    else if (newBoard.blackBishops & (1ULL << move.sourceSquare))
    {
      newBoard.blackBishops &= sourceMask;
      newBoard.blackBishops |= targetMask;
    }
    else if (newBoard.blackRooks & (1ULL << move.sourceSquare))
    {
      newBoard.blackRooks &= sourceMask;
      newBoard.blackRooks |= targetMask;
      if (move.sourceSquare == 0)
      {
        newBoard.blackQueenCastle = false;
      }
      else if (move.sourceSquare == 7)
      {
        newBoard.blackKingCastle = false;
      }
    }
    else if (newBoard.blackQueens & (1ULL << move.sourceSquare))
    {
      newBoard.blackQueens &= sourceMask;
      newBoard.blackQueens |= targetMask;
    }
    else if (newBoard.blackKings & (1ULL << move.sourceSquare))
    {
      newBoard.blackKings &= sourceMask;
      newBoard.blackKings |= targetMask;
    }
  }

  // Update general piece bitboards
  newBoard.whitePieces = newBoard.whitePawns | newBoard.whiteKnights | newBoard.whiteBishops |
                         newBoard.whiteRooks | newBoard.whiteQueens | newBoard.whiteKings;

  newBoard.blackPieces = newBoard.blackPawns | newBoard.blackKnights | newBoard.blackBishops |
                         newBoard.blackRooks | newBoard.blackQueens | newBoard.blackKings;

  newBoard.whiteToMove = !whiteToMove;
  return newBoard;
}

bool checks()
{

  return true;
}
#ifndef SEARCHER_H
#define SEARCHER_H

#include <vector>
#include <limits>
#include "bitboards.h"
#include "moves.h"

// Holds a move and its evaluation score (for convenience)
struct ScoredMove
{
  Move move;
  double score;
};

// The main interface to find the best move from a given board state.
// 'depth' is measured in plies. The function will return the best move
// for the side to move in the given 'board' state.
Move findBestMove(Bitboards board, int depth);

// Internal minimax with alpha-beta pruning.
double alphaBeta(Bitboards &board, int depth, double alpha, double beta, bool maximizingPlayer, Move &bestMove);

#endif // SEARCHER_H

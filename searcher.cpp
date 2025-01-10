#include "searcher.h"
#include "evaluation.h"

// Forward declaration of alphaBeta or any helper function if needed.

// Public function that your engine (or main program) calls to get the best move.
Move findBestMove(Bitboards board, int depth)
{
  // If depth <= 0, return an empty move (or some default).
  if (depth <= 0)
  {
    return Move{0, 0, ' ', false};
  }

  // We call alphaBeta on the board. The side to move is determined by board.whiteToMove.
  Move bestMove;
  // If it's white to move, we are maximizing from white's perspective.
  // If it's black to move, we are maximizing from black's perspective but we can unify logic
  // by simply setting maximizingPlayer = (board.whiteToMove).
  double bestScore = alphaBeta(board, depth, -std::numeric_limits<double>::infinity(),
                               std::numeric_limits<double>::infinity(),
                               board.whiteToMove, bestMove);

  // bestMove is filled by alphaBeta
  return bestMove;
}

// Minimax with alpha-beta pruning
double alphaBeta(Bitboards &board, int depth, double alpha, double beta, bool maximizingPlayer, Move &outBestMove)
{
  // Base case: if depth = 0, return the static evaluation of this position.
  // 'outBestMove' need not be changed, because at depth 0 there's no move to make.
  if (depth == 0)
  {
    return evaluateBoard(board);
  }

  // Generate all moves for side to move (whiteToMove).
  std::vector<Move> moves = generateLegalMoves(board, board.whiteToMove);

  // If there are no moves, it could be checkmate or stalemate. Let's do a basic check:
  if (moves.empty())
  {
    // If it's check, let's return large negative or large positive score
    //   but currently we do not fully check for "in-check". We'll assume "checkmate" = losing.
    //   If you have a method to detect check or checkmate, you can refine this.
    // In your Bitboards class, you have `checkmate` and `stalemate` flags, but they might not be reliable
    //   if you haven't implemented them fully.
    // For demonstration, let's simply penalize the side to move if it has no moves:
    return maximizingPlayer ? -99999.0 : 99999.0;
  }

  // We will store the best move found so far in a local variable.
  Move bestMoveLocal = moves[0];
  double bestEval = maximizingPlayer ? -std::numeric_limits<double>::infinity()
                                     : std::numeric_limits<double>::infinity();

  for (const auto &m : moves)
  {
    // Simulate the move
    Bitboards newBoard = board.simulateMove(m);
    // Recursively call alphaBeta with depth-1
    Move dummyChildMove; // This will hold the best move for the child call, but we don't need it here.
    double score = alphaBeta(newBoard, depth - 1, alpha, beta, !maximizingPlayer, dummyChildMove);

    // If we are maximizing, we look for the highest score.
    if (maximizingPlayer)
    {
      if (score > bestEval)
      {
        bestEval = score;
        bestMoveLocal = m;
      }
      // Update alpha
      alpha = std::max(alpha, bestEval);
      // Alpha-beta cutoff
      if (beta <= alpha)
      {
        break;
      }
    }
    // Minimizing
    else
    {
      if (score < bestEval)
      {
        bestEval = score;
        bestMoveLocal = m;
      }
      // Update beta
      beta = std::min(beta, bestEval);
      // Alpha-beta cutoff
      if (beta <= alpha)
      {
        break;
      }
    }
  }

  // Write out the bestMove found in this node
  outBestMove = bestMoveLocal;
  return bestEval;
}

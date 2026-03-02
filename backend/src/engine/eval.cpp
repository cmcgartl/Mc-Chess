#include "eval.h"
#include <limits>

int Eval::EvaluatePosition(const std::array<Piece, 64>& squares, Color c){
        int eval = 0;
        for(size_t i = 0; i < squares.size(); i++){
            int value = getValueForPiece(squares[i], i);
            squares[i].color == c ? eval += value : eval -= value;
        }
        return eval;
}

int Eval::getValueForPiece(const Piece& piece, int square){
    int idx = (piece.color == Color::b) ? (square ^ 56) : square;

    switch(piece.type){
        case PieceType::P:
            return PieceValues::P + pawnTable[idx];
            break;
        case PieceType::N:
            return PieceValues::N + knightTable[idx];
            break;
        case PieceType::B:
            return PieceValues::B + bishopTable[idx];
            break;
        case PieceType::R:
            return PieceValues::R + rookTable[idx];
            break;
        case PieceType::Q:
            return PieceValues::Q + queenTable[idx];
            break;
        case PieceType::K:
            return PieceValues::K + kingMidGameTable[idx];
            break;
        default:
            return 0;
    }
}

MiniMaxResult Eval::MiniMax(Position& p, int depth, bool isMaximizer, MoveGenResult& moveState, Color c, int alpha, int beta){
    nodes++;
    UndoInfo undo;
    MoveGenResult res;
    const auto squares = p.getBoard().getSquares();
    if(depth == 0 || moveState.moves.empty()){
        Move dummy(0, 0);
        return {dummy, EvaluatePosition(squares, c)};
    }

    // Probe transposition table
    uint64_t posHash = p.getHash();
    TTEntry& ttEntry = tt[posHash % TT_SIZE];
    bool ttMovePlacedAtFront = false;
    if(ttEntry.hash == posHash){
        if(ttEntry.depth >= depth){
            if(ttEntry.flag == TTFlag::EXACT) return {ttEntry.bestMove, ttEntry.score};
            if(ttEntry.flag == TTFlag::LOWER_BOUND) alpha = std::max(alpha, ttEntry.score);
            if(ttEntry.flag == TTFlag::UPPER_BOUND) beta = std::min(beta, ttEntry.score);
            if(alpha >= beta) return {ttEntry.bestMove, ttEntry.score};
        }
        else{
            auto it = std::find(moveState.moves.begin(), moveState.moves.end(), ttEntry.bestMove);
            if(it != moveState.moves.end()){
                std::rotate(moveState.moves.begin(), it, it + 1);
                ttMovePlacedAtFront = true;
            }
        }
    }
    int originalAlpha = alpha;

    auto start = ttMovePlacedAtFront? moveState.moves.begin() + 1 : moveState.moves.begin();
    std::sort(start, moveState.moves.end(), [&squares, this](const Move& a, const Move& b){
        int scoreA = getValueForPiece(squares[a.to], a.to) - getValueForPiece(squares[a.from], a.from);
        int scoreB = getValueForPiece(squares[b.to], b.to) - getValueForPiece(squares[b.from], b.from);
        return scoreA > scoreB;
    });

    Move bestMove = moveState.moves[0];

    if(isMaximizer){
        int max_eval = negInf;
        for(const auto& move : moveState.moves){
            undo = p.applyMove(move);
            p.generatePieceLists();
            res = p.generateAllValidMovesForSide(p.getSideToMove());
            int eval = MiniMax(p, depth - 1, false, res, c, alpha, beta).score;
            if(eval > max_eval){
                max_eval = eval;
                bestMove = move;
            }
            alpha = std::max(alpha, eval);
            p.undoMove(move, undo);
            p.generatePieceLists();
            if(beta <= alpha) break;
        }

        // Store in transposition table
        TTFlag flag;
        if(max_eval <= originalAlpha) flag = TTFlag::UPPER_BOUND;
        else if(max_eval >= beta) flag = TTFlag::LOWER_BOUND;
        else flag = TTFlag::EXACT;
        ttEntry = {posHash, depth, max_eval, flag, bestMove};

        return {bestMove, max_eval};
    }
    else{
        int min_eval = posInf;
        for(const auto& move : moveState.moves){
            undo = p.applyMove(move);
            p.generatePieceLists();
            res = p.generateAllValidMovesForSide(p.getSideToMove());
            int eval = MiniMax(p, depth - 1, true, res, c, alpha, beta).score;
            if(eval < min_eval){
                min_eval = eval;
                bestMove = move;
            }
            beta = std::min(beta, eval);
            p.undoMove(move, undo);
            p.generatePieceLists();
            if(beta <= alpha) break;
        }

        // Store in transposition table
        TTFlag flag;
        if(min_eval <= originalAlpha) flag = TTFlag::UPPER_BOUND;
        else if(min_eval >= beta) flag = TTFlag::LOWER_BOUND;
        else flag = TTFlag::EXACT;
        ttEntry = {posHash, depth, min_eval, flag, bestMove};

        return {bestMove, min_eval};
    }
}
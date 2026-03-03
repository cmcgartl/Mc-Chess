#include "eval.h"
#include <limits>
#include <cstring>
#include <random>

void Eval::clearKillers(){
    std::memset(killers, 0, sizeof(killers));
}

void Eval::clearHistory(){
    std::memset(historyTable, 0, sizeof(historyTable));
}

void Eval::updateHistoryTable(const std::array<Piece, 64>& squares, const Move& move, int depth, int ply, int sideToMove){
    int sideIdx = static_cast<int>(sideToMove);
    if(squares[move.to].type == PieceType::None){
        if(!(move == killers[ply][0]) && !(move == killers[ply][1])){
            killers[ply][1] = killers[ply][0];
            killers[ply][0] = move;
        }
        historyTable[sideIdx][move.from][move.to] += depth * depth;
    }
}

int Eval::getMVVLVAScore(int from, int to, const std::array<Piece, 64>& squares){
    return getValueForPiece(squares[to], to) * 10 - getValueForPiece(squares[from], from);
}

int Eval::EvaluatePosition(const std::array<Piece, 64>& squares, Color c, PositionStatus status){
        if(status == PositionStatus::CheckMateWhite) return c == Color::w ? posInf : negInf; 
        else if(status == PositionStatus::CheckMateBlack) return c == Color::b ? posInf : negInf;
        else if(status == PositionStatus::Stalemate) return 0;
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

MiniMaxResult Eval::MiniMax(Position& p, int depth, int ply, bool isMaximizer, MoveGenResult& moveState, Color c, int alpha, int beta){
    nodes++;
    UndoInfo undo;
    MoveGenResult res;
    const auto squares = p.getBoard().getSquares();
    if(moveState.moves.empty()){
        Move dummy(0, 0);
        return {dummy, EvaluatePosition(squares, c, moveState.status)};
    }
    if(depth == 0){
        return quiescence(p, isMaximizer, moveState, c, alpha, beta);
    }

    // Probe transposition table
    uint64_t posHash = p.getHash();
    TTEntry& ttEntry = tt[posHash % TT_SIZE];
    Move ttMove(0, 0);
    bool hasTTMove = false;
    //if we find a match in the transposition table
    if(ttEntry.hash == posHash){
        //if that has is AT LEAST as good of info as we are about to search for
        if(ttEntry.depth >= depth){
            if(ttEntry.flag == TTFlag::EXACT){
                return {ttEntry.bestMove, ttEntry.score};
            }
            if(ttEntry.flag == TTFlag::LOWER){
                alpha = std::max(alpha, ttEntry.score);
            }
            if(ttEntry.flag == TTFlag::UPPER) {
                beta = std::min(beta, ttEntry.score);
            }
            if(alpha >= beta) {
                return {ttEntry.bestMove, ttEntry.score};
            }
        }
        auto it = std::find(moveState.moves.begin(), moveState.moves.end(), ttEntry.bestMove);
        if(it != moveState.moves.end()){
            ttMove = ttEntry.bestMove;
            hasTTMove = true;
        }
    }
    int originalAlpha = alpha;
    int originalBeta = beta;

    // search the TT entry move first, then the captures, then the killers, then the history table
    int sideIdx = static_cast<int>(p.getSideToMove());
    std::sort(moveState.moves.begin(), moveState.moves.end(), 
        [&hasTTMove, &ttMove, &squares, this, &ply, &sideIdx](const Move& a, const Move& b){
            auto getMoveScore = [&](const Move& m) -> int {
                if(hasTTMove && m == ttMove){
                    return 20000000;
                }
                if(m.type == MoveType::P){
                    return 15000000;
                }
                if(squares[m.to].type != PieceType::None){
                    return 10000000 + getMVVLVAScore(m.from, m.to, squares);
                }
                if(m == killers[ply][0]){
                    return 9000001;
                }
                if(m == killers[ply][1]){
                    return 9000000;
                }
                return historyTable[sideIdx][m.from][m.to];
            };
        return getMoveScore(a) > getMoveScore(b);
    });
    
    Move bestMove = moveState.moves[0];

    if(isMaximizer){
        int max_eval = negInf;
        for(const auto& move : moveState.moves){
            undo = p.applyMove(move);
            p.generatePieceLists();
            res = p.generateAllValidMovesForSide(p.getSideToMove());
            int eval = MiniMax(p, depth - 1, ply + 1, false, res, c, alpha, beta).score;
            if(eval > max_eval){
                max_eval = eval;
                bestMove = move;
            }
            alpha = std::max(alpha, eval);
            p.undoMove(move, undo);
            p.generatePieceLists();
            if(beta <= alpha){
                updateHistoryTable(squares, move, depth, ply, sideIdx);
                break;
            }
        }

        // Store in transposition table
        if(max_eval <= originalAlpha){
            ttEntry.flag = TTFlag::UPPER;
        } 
        else if(max_eval >= originalBeta) {
            ttEntry.flag = TTFlag::LOWER;
        }
        else {
            ttEntry.flag = TTFlag::EXACT;
        }

        ttEntry.hash = posHash;
        ttEntry.depth = depth;
        ttEntry.score = max_eval;
        ttEntry.bestMove = bestMove;

        return {bestMove, max_eval};
    }
    else{
        int min_eval = posInf;
        for(const auto& move : moveState.moves){
            undo = p.applyMove(move);
            p.generatePieceLists();
            res = p.generateAllValidMovesForSide(p.getSideToMove());
            int eval = MiniMax(p, depth - 1, ply + 1, true, res, c, alpha, beta).score;
            if(eval < min_eval){
                min_eval = eval;
                bestMove = move;
            }
            beta = std::min(beta, eval);
            p.undoMove(move, undo);
            p.generatePieceLists();
            if(beta <= alpha){
                updateHistoryTable(squares, move, depth, ply, sideIdx);
                break;
            }
        }

        if(min_eval <= originalAlpha){
            ttEntry.flag = TTFlag::UPPER;
        }
        else if(min_eval >= originalBeta){
            ttEntry.flag = TTFlag::LOWER;
        }
        else {
            ttEntry.flag = TTFlag::EXACT;
        }

        ttEntry.hash = posHash;
        ttEntry.depth = depth;
        ttEntry.score = min_eval;
        ttEntry.bestMove = bestMove;

        return {bestMove, min_eval};
    }
}


MiniMaxResult Eval::quiescence(Position& p, bool isMaximizer, MoveGenResult& moveState, Color c, int alpha, int beta){
    nodes++;
    const auto squares = p.getBoard().getSquares();
    Move dummy(0, 0);

    // Standing pat: the side to move can always choose not to capture
    int standPat = EvaluatePosition(squares, c, moveState.status);

    if(isMaximizer){
        if(standPat >= beta) return {dummy, standPat};
        alpha = std::max(alpha, standPat);
        int max_eval = standPat;
        Move bestMove = dummy;

        for(const auto& move : moveState.moves){
            if(squares[move.to].type == PieceType::None) continue;

            UndoInfo undo = p.applyMove(move);
            p.generatePieceLists();
            MoveGenResult res = p.generateAllValidMovesForSide(p.getSideToMove());
            int eval = quiescence(p, false, res, c, alpha, beta).score;
            p.undoMove(move, undo);
            p.generatePieceLists();

            if(eval > max_eval){
                max_eval = eval;
                bestMove = move;
            }
            alpha = std::max(alpha, eval);
            if(beta <= alpha) break;
        }
        return {bestMove, max_eval};
    }
    else{
        if(standPat <= alpha) return {dummy, standPat};
        beta = std::min(beta, standPat);
        int min_eval = standPat;
        Move bestMove = dummy;

        for(const auto& move : moveState.moves){
            if(squares[move.to].type == PieceType::None) continue;

            UndoInfo undo = p.applyMove(move);
            p.generatePieceLists();
            MoveGenResult res = p.generateAllValidMovesForSide(p.getSideToMove());
            int eval = quiescence(p, true, res, c, alpha, beta).score;
            p.undoMove(move, undo);
            p.generatePieceLists();

            if(eval < min_eval){
                min_eval = eval;
                bestMove = move;
            }
            beta = std::min(beta, eval);
            if(beta <= alpha) break;
        }
        return {bestMove, min_eval};
    }
}

void Eval::resetTT(){
    std::fill(tt.begin(), tt.end(), TTEntry{});
    return;
}
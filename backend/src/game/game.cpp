
#include "game.h"
#include <iostream>
#include <string>
#include <chrono>


Game::Game(){
    res = GameResult::InProgress;
    p.setSideToMove(Side::w);
    currentMoves = p.generateAllValidMovesForSide(Side::w);
    positionHistory.emplace_back(p.getHash());
}

int stringToSquare(std::string_view squareString){
    if(squareString.length() != 2){
        return -1;
    }

    int file = (squareString[0] - 'a');
    int rank = squareString[1] - '0';

    if(file < 0 || file > 7 || rank < 1 || rank > 8){
        return -1;
    }

    return (8 * (8 - rank) + file);
}

Move Game::moveStringToMove(std::string_view MoveString){
    if(MoveString.size() != 5){
        throw std::logic_error("Game:: moveStringToMove: invalid move string");
    }

    std::string_view view(MoveString);
    size_t space_pos = view.find(' ');

    if(space_pos != 2){
        throw std::logic_error("Game:: moveStringToMove: invalid move string");
    }
    std::string_view sub1 = view.substr(0, space_pos);
    std::string_view sub2 = view.substr(space_pos + 1);

    return Move(stringToSquare(sub1), stringToSquare(sub2));
}

std::string Game::squareToAlgebraic(int square) {
    if (square < 0 || square > 63) return "??";
    char file = 'a' + (square % 8);
    char rank = '0' + (8 - square / 8);
    return std::string(1, file) + std::string(1, rank);
}

bool Game::makeMove(const std::string& from, const std::string& to) {
    bool moveIsReversible = false;
    int fromSq = stringToSquare(from);
    int toSq = stringToSquare(to);
    if (fromSq < 0 || toSq < 0) return false;
    Move move(static_cast<uint8_t>(fromSq), static_cast<uint8_t>(toSq));
    const Piece& PieceFrom = p.getBoard().at(fromSq);
    const Piece& PieceTo = p.getBoard().at(toSq);
    if(move.type == MoveType::Normal && PieceFrom.type != PieceType::P && PieceTo.color == Color::None ){
        moveIsReversible = true;
    }
    bool success = p.makeMove(move, currentMoves);
    if (success) {
        if(!moveIsReversible){
            positionHistory.clear();
        }
        moveHistory.push_back(move);
        uint64_t newPositionHash = p.getHash();
        positionHistory.emplace_back(newPositionHash);
        currentMoves = p.generateAllValidMovesForSide(p.getSideToMove());
        int repetitionCount = 0;
        for(const auto& hash : positionHistory){
            if(hash == newPositionHash){
                repetitionCount++;
            }
        }
        if(repetitionCount >= 3){
            currentMoves.status = PositionStatus::MoveDraw;
        }
        if(currentMoves.status == PositionStatus::CheckMateWhite){
                res = GameResult::WhiteWin;
        }
        else if(currentMoves.status == PositionStatus::CheckMateBlack){
                res = GameResult::BlackWin;
        }
        else if(currentMoves.status == PositionStatus::Stalemate || currentMoves.status == PositionStatus::MoveDraw){
            res = GameResult::Draw;
        }
    }
    return success;
}

void Game::reset() {
    eval.resetTT();
    p = Position();
    moveHistory.clear();
    positionHistory.clear();
    positionHistory.emplace_back(p.getHash());
    res = GameResult::InProgress;
    p.setSideToMove(Side::w);
    currentMoves = p.generateAllValidMovesForSide(Side::w);
}

bool Game::isEngineTurn() const {
    if (res != GameResult::InProgress) return false;
    if (engineMode == EngineMode::Off) return false;
    if (engineMode == EngineMode::White && p.getSideToMove() == Side::w) return true;
    if (engineMode == EngineMode::Black && p.getSideToMove() == Side::b) return true;
    return false;
}

std::optional<Move> Game::getEngineBestMove() {
    if (!isEngineTurn() || currentMoves.moves.empty()){
        return std::nullopt;
    }
    Color engineColor = (engineMode == EngineMode::White) ? Color::w : Color::b;
    MiniMaxResult result{{0,0}, negInf};
    eval.nodes = 0;
    eval.clearKillers();
    eval.clearHistory();
    auto start = std::chrono::steady_clock::now();
    for(int d = 1; d <= searchDepth; d++){
        MoveGenResult movesCopy = currentMoves;
        result = eval.MiniMax(p, d, 0, true, movesCopy, engineColor, negInf, posInf, positionHistory);
    }
    auto end = std::chrono::steady_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end - start).count();
    double nps = (ms > 0) ? (eval.nodes / (ms / 1000.0)) : 0;
    std::cout << "Depth " << searchDepth << " | Nodes: " << eval.nodes << " | Time: " << static_cast<int>(ms) << "ms" << " | NPS: " << static_cast<int>(nps) << " | Eval: " << result.score << std::endl;
    return result.bestMove;
}

void Game::setSelectedMove(const Move& m){
    selectedMove = m;
}


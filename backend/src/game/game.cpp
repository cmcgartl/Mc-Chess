
#include "game.h"
#include <iostream>
#include <string>


Game::Game(){
    res = GameResult::InProgress;
    p.setSideToMove(Side::w);
    currentMoves = p.generateAllValidMovesForSide(Side::w);
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
    int fromSq = stringToSquare(from);
    int toSq = stringToSquare(to);
    if (fromSq < 0 || toSq < 0) return false;
    Move move(static_cast<uint8_t>(fromSq), static_cast<uint8_t>(toSq));
    bool success = p.makeMove(move, currentMoves);
    if (success) {
        moveHistory.push_back(move);
        currentMoves = p.generateAllValidMovesForSide(p.getSideToMove());
        if(currentMoves.status == PositionStatus::CheckMate){
            p.getSideToMove() == Side::w?
                res = GameResult::BlackWin :
                res = GameResult::WhiteWin;
        }
        else if(currentMoves.status == PositionStatus::Stalemate || currentMoves.status == PositionStatus::MoveDraw){
            res = GameResult::Draw;
        }
    }
    return success;
}

void Game::reset() {
    p = Position();
    moveHistory.clear();
    res = GameResult::InProgress;
    p.setSideToMove(Side::w);
    currentMoves = p.generateAllValidMovesForSide(Side::w);
}


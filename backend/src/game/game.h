#pragma once
#include "board.h"
#include "vector"
#include "move.h"
#include "position.h"

enum class GameResult {InProgress, WhiteWin, BlackWin, Draw};

class Game {
    public:
        Game();

        GameResult getResult(){return res;}
        std::string getResultString() const {
            switch(res){
                case GameResult::InProgress: return "Game In Progress";
                case GameResult::WhiteWin: return "Checkmate! White Wins!";
                case GameResult::BlackWin: return "Checkmate! Black Wins!";
                case GameResult::Draw: return "draw";
            }
            return "unknown";
        }
        Move moveStringToMove(std::string_view MoveString);
        bool makeMove(const std::string& from, const std::string& to);
        void reset();
        Position& getPosition(){return p;}
        const MoveGenResult& getCurrentMoves() const {return currentMoves;}
        std::string squareToAlgebraic(int square);

    private:
        Position p;
        MoveGenResult currentMoves;
        std::vector<Move> moveHistory;
        GameResult res;
};
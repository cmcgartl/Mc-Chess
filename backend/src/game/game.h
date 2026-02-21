#pragma once
#include "board.h"
#include "vector"
#include "move.h"
#include "pin.h"
#include "position.h"

enum class GameResult {InProgress, WhiteWin, BlackWin, Draw};

class Game {
    public:
        Game();

        void run();

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
        std::string squareToAlgebraic(int square);

        template<typename Func>
        void testGameWithInput(std::vector<std::string> moves, Func testFunc);

    private:
        Position p;
        std::vector<Move> moveHistory;
        GameResult res;
};
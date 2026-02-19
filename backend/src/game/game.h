#pragma once
#include "board.h"
#include "vector"
#include "move.h"
#include "pin.h"
#include "position.h"

enum class GameResult {InProgress, WhiteWin, BlackWin, Stalemate} ;

class Game {
    public:
        Game();

        void run();

        GameResult getResult(){return res;}
        Move moveStringToMove(std::string_view MoveString);
        Position& getPosition(){return p;}

        template<typename Func>
        void testGameWithInput(std::vector<std::string> moves, Func testFunc);

    private:
        Position p;
        std::vector<Move> moveHistory;
        GameResult res;
};
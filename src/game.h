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


    private:
        Position p;
        std::vector<Move> moveHistory;
        GameResult res;
};
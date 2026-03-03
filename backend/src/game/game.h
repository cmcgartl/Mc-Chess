#pragma once
#include "board.h"
#include "vector"
#include "move.h"
#include "position.h"
#include "eval.h"
#include <optional>

enum class GameResult {InProgress, WhiteWin, BlackWin, Draw};
enum class EngineMode {Off, White, Black};

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

        void setEngineMode(EngineMode mode) { engineMode = mode; }
        EngineMode getEngineMode() const { return engineMode; }
        void setSearchDepth(int depth) { searchDepth = depth; }
        int getSearchDepth() const { return searchDepth; }
        bool isEngineTurn() const;
        std::optional<Move> getEngineBestMove();
        void setSelectedMove(const Move& m);
        const Move& getSelectedMove() const {return selectedMove;}

    private:
        Position p;
        MoveGenResult currentMoves;
        std::vector<Move> moveHistory;
        GameResult res;
        Eval eval;
        EngineMode engineMode = EngineMode::Off;
        int searchDepth = 4;
        Move selectedMove;
};
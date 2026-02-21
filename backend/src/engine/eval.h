#pragma once
#include "position.h"
#include "board.h"
//piece evaluations roughly based on their in game values:
//pawn = 1, Knight/Bishop = 3, Rook = 5, Queen = 9, King = game deciding.
//translate these values into this struct, with a slight preference for bishops over knights
struct PieceValues {
    static constexpr int P = 100;
    static constexpr int N = 320;
    static constexpr int B = 330;
    static constexpr int R = 500;
    static constexpr int Q = 900;
    static constexpr int K = 20000;
};

class Evaluation {
    public:
        int EvaluatePosition(std::array<Piece, 64>& squares, Color color);
        int getValueForPiece(Piece& piece);
        void tryMoveAndEval(std::array<Piece, 64>& squares, Color color);

    private:
        Position p;
};


//Minimax Logic:
    //minimax(start position, search depth, maximizing player)
        //for every move in the position
            //try the move, evaluate the resulting position
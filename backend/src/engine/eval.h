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

static constexpr int negInf = std::numeric_limits<int>::min();  // -2147483648
static constexpr int posInf = std::numeric_limits<int>::max();

static constexpr int pawnTable[64] = 
{
 0,  0,  0,  0,  0,  0,  0,  0, 
 50, 50, 50, 50, 50, 50, 50, 50, 
 10, 10, 20, 30, 30, 20, 10, 10,
 5,  5, 10, 25, 25, 10,  5,  5,
 0,  0,  0, 20, 20,  0,  0,  0,
 5, -5,-10,  0,  0,-10, -5,  5,
 5, 10, 10,-20,-20, 10, 10,  5,
 0,  0,  0,  0,  0,  0,  0,  0
};

static constexpr int knightTable[64] = 
{
-50,-40,-30,-30,-30,-30,-40,-50,
-40,-20,  0,  0,  0,  0,-20,-40,
-30,  0, 10, 15, 15, 10,  0,-30,
-30,  5, 15, 20, 20, 15,  5,-30,
-30,  0, 15, 20, 20, 15,  0,-30,
-30,  5, 10, 15, 15, 10,  5,-30,
-40,-20,  0,  5,  5,  0,-20,-40,
-50,-40,-30,-30,-30,-30,-40,-50
};


static constexpr int bishopTable[64] = 
{
-20,-10,-10,-10,-10,-10,-10,-20,
-10,  0,  0,  0,  0,  0,  0,-10,
-10,  0,  5, 10, 10,  5,  0,-10,
-10,  5,  5, 10, 10,  5,  5,-10,
-10,  0, 10, 10, 10, 10,  0,-10,
-10, 10, 10, 10, 10, 10, 10,-10,
-10,  5,  0,  0,  0,  0,  5,-10,
-20,-10,-10,-10,-10,-10,-10,-20,
};

static constexpr int rookTable[64] = 
{
  0,  0,  0,  0,  0,  0,  0,  0,
  5, 10, 10, 10, 10, 10, 10,  5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
  0,  0,  0,  5,  5,  0,  0,  0
};

static constexpr int queenTable[64] = 
{
-20,-10,-10, -5, -5,-10,-10,-20,
-10,  0,  0,  0,  0,  0,  0,-10,
-10,  0,  5,  5,  5,  5,  0,-10,
 -5,  0,  5,  5,  5,  5,  0, -5,
  0,  0,  5,  5,  5,  5,  0, -5,
-10,  5,  5,  5,  5,  5,  0,-10,
-10,  0,  5,  0,  0,  0,  0,-10,
-20,-10,-10, -5, -5,-10,-10,-20
};

static constexpr int kingMidGameTable[64] = 
{
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-20,-30,-30,-40,-40,-30,-30,-20,
-10,-20,-20,-20,-20,-20,-20,-10,
 20, 20,  0,  0,  0,  0, 20, 20,
 20, 30, 10,  0,  0, 10, 30, 20
};
struct MiniMaxResult {
    Move bestMove;
    int score;
};

enum class TTFlag { EXACT, LOWER_BOUND, UPPER_BOUND };

struct TTEntry {
    uint64_t hash = 0;
    int depth = 0;
    int score = 0;
    TTFlag flag = TTFlag::EXACT;
    Move bestMove{0, 0};
};

class Eval {
    public:
        int EvaluatePosition(const std::array<Piece, 64>& squares, Color color);
        int getValueForPiece(const Piece& piece, int square);
        MiniMaxResult MiniMax(Position& p, int depth, bool isMaximizer, MoveGenResult& moveState, Color c, int alpha, int beta);
        uint64_t nodes = 0;
    private:
        static constexpr size_t TT_SIZE = 1 << 20;
        std::vector<TTEntry> tt{TT_SIZE};
};


//Minimax Logic:
    //minimax(start position, search depth, maximizing player)
        //for every move in the position
            //try the move, evaluate the resulting position
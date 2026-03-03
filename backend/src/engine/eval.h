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
static constexpr size_t TT_SIZE = 1 << 20;
static constexpr int pawnTable[64] = 
{
 0,  0,  0,  0,  0,  0,  0,  0, 
 50, 50, 50, 50, 50, 50, 50, 50, 
 10, 10, 20, 30, 30, 20, 10, 10,
 5,  5, 10, 27, 27, 10,  5,  5,
 0,  0,  0, 25, 25,  0,  0,  0,
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

static constexpr int kingEndGameTable[64] = 
{
    -50,-40,-30,-20,-20,-30,-40,-50,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-30,  0,  0,  0,  0,-30,-30,
    -50,-30,-30,-30,-30,-30,-30,-50
};


struct MiniMaxResult {
    Move bestMove;
    int score;
};

enum class TTFlag { EXACT, LOWER, UPPER };

struct TTEntry {
    uint64_t hash = 0;
    int depth = 0;
    int score = 0;
    TTFlag flag = TTFlag::EXACT;
    Move bestMove{0, 0};
};

class Eval {
    public:
        int EvaluatePosition(const std::array<Piece, 64>& squares, Color color, PositionStatus status);
        int getValueForPiece(const Piece& piece, int square);
        int getMVVLVAScore(int from, int to, const std::array<Piece, 64>& squares);
        MiniMaxResult MiniMax(Position& p, int depth, int ply, bool isMaximizer, MoveGenResult& moveState, Color c, int alpha, int beta);
        MiniMaxResult quiescence(Position& p, bool isMaximizer, MoveGenResult& moveState, Color c, int alpha, int beta);
        void updateHistoryTable(const std::array<Piece, 64>& squares, const Move& move, int depth, int ply, int sideToMove);
        void clearKillers();
        void clearHistory();
        void resetTT();
        uint64_t nodes = 0;
    private:
        static constexpr int MAX_DEPTH = 64;
        std::vector<TTEntry> tt{TT_SIZE};
        Move killers[64][2];
        int historyTable[2][64][64];
};


//Minimax Logic:
    //minimax(start position, search depth, maximizing player)
        //for every move in the position
            //try the move, evaluate the resulting position
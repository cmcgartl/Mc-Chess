#include "board.h"
#include "vector"


enum class Side {w, b};

class Position{
    public:
        Position():board(START_FEN) , sideToMove(Side::w){
            generatePieceLists();
        };
        Position(const std::string& FEN);
        bool tryMove(int src, int dest);
        void generatePieceLists();
        std::vector<int> getValidMoves(int square);
        std::vector<int> getValidMovesPawn(int square);

    private:
        Board board;
        Side sideToMove;
        std::vector<Piece> whitePieces;
        std::vector<Piece> blackPieces;
        static constexpr const char* START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w";

};
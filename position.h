#include "board.h"
#include "vector"


enum class Side {w, b};

class Position{
    public:
        Position():board(START_FEN) , sideToMove(Side::w){
            generatePieceLists();
        };
        Position(const std::string& FEN): board(FEN){
            generatePieceLists();
        }
        bool tryMove(int src, int dest);
        void generatePieceLists();

        //return vector by value because new generated list of moves -> probably want to add a member vec of possible moves stored privately in position and update this
        std::vector<int> getValidMoves(int square) const;
        std::vector<int> getValidMovesPawn(int square) const; 
        std::vector<int> getValidMovesBishop(int square)const;
        std::vector<int> getValidMovesQueen(int square)const;
        std::vector<int> getValidMovesKnight(int square)const;
        std::vector<int> getValidMovesKing(int square)const;


        //return a const reference so that we get the private data, but cannot modify
        const std::vector<Piece>& getWhitePieces() const {return whitePieces;}
        const std::vector<Piece>& getBlackPieces() const {return blackPieces;}


    private:
        Board board;
        Side sideToMove;
        std::vector<Piece> whitePieces;
        std::vector<Piece> blackPieces;
        static constexpr const char* START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w";

        void generateDiagonalMoves(std::vector<int>& moves, int square, bool cap, Color color) const;
        void generateOrthoganalMoves(std::vector<int>& moves, int square, bool cap, Color color) const;
};
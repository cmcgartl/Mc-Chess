#pragma once 
#include "board.h"
#include "vector"
#include "move.h"
#include "pin.h"


enum class Side {w, b};

class Position{
    public:
        Position():board(START_FEN) , sideToMove(Side::w){
            generatePieceLists();
            moveStartIndices.fill(-1);
            moveCounts.fill(-1);
        };
        Position(const std::string& FEN): board(FEN){
            generatePieceLists();
            moveStartIndices.fill(-1);
            moveCounts.fill(-1);
        }

        bool tryMove(int src, int dest);
        void generatePieceLists();

        //return vector by value because new generated list of moves -> probably want to add a member vec of possible moves stored privately in position and update this
        const std::vector<Move>& getValidMoves() const {return possibleMoves;}

        void generateAllValidMovesForSide(Side side);
        void generateValidMoves(int square);
        void getValidMovesPawn(int& count, int squar, std::vector<int>& pinDirections); 
        void getValidMovesKnight(int& count, int square);
        std::vector<Move> getValidMovesForPieceAt(int square) const;
        bool moveStopsCheck(Move& move, std::vector<int>& attackingSquares);



        //return a const reference so that we get the private data, but cannot modify
        const std::vector<Piece>& getWhitePieces() const {return whitePieces;}
        const std::vector<Piece>& getBlackPieces() const {return blackPieces;}
        const std::vector<int>& getPiecesAttackingBlack() const {return squaresAttackingBlackKing;}
        const std::vector<int>& getPiecesAttackingWhite() const {return squaresAttackingWhiteKing;}
        const std::vector<Pin>& getPins() const {return pins;}
        Side getSideToMove() const {return sideToMove;}
        void setSideToMove(Side side) {sideToMove = side;}
        const Board& getBoard() const {return board;}
        void makeMove(const Move& move);

        template<size_t N, typename Func>
        void walkDirectionsAndDo(int startSquare, const int (&directions)[N], bool limitedMovementPiece, Func func);
        bool isSquareAttacked(int square, Color color);

    private:
        Board board;
        Side sideToMove;
        std::vector<Piece> whitePieces;
        std::vector<Piece> blackPieces;
        std::vector<Move> possibleMoves; //vector of Moves (to and from squares)
        std::array<int, 64> moveStartIndices; //moveStartSquares[i] = index in possibleMoves where the possible moves from square i are stored

        std::array<int, 64> moveCounts; //stores moveCounts[i] = how many moves there are from square i
        static constexpr const char* START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w";

        void generateDiagonalMoves(int& count, int square, bool cap, Color color, std::vector<int>& pinDirections);
        void generateOrthoganalMoves(int& count, int square, bool cap, Color color, std::vector<int>& pinDirections);
        void generateCheckResolutions(int square, Color color);



        //whiteMoveStarts
        //blackMoveStarts
        //bool whiteInCheck;
        //bool blackInCheck;
        std::vector<int> squaresAttackingWhiteKing;
        std::vector<int> squaresAttackingBlackKing;
        std::vector<Pin> pins;
};

//generating moves:
//for piece in whitePieces
    //make list of all possible moves for that piece
    //insert all possible moves into possibleMoves. record start index (possibleMoves.size())
    //update moveStartSquares[square] = start index in possibleMoves

//when we try to make a move (where move is requestedMove):
//go to possibleMoves[moveStartSquares[requestedMove.from]]
//while current move in possibleMoves currentMove.from == requestedMove.from
    //if currentMove ==requestedMove: execute move
    //else, move to next move

//if in check
//only produce moves that solve check
//for every pseudo-legal move
    //check if move stops check
        //if move captures attacker
        //if move blocks attacker
        //if move moves the king
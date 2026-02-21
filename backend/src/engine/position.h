#pragma once
#include "board.h"
#include "vector"
#include "move.h"


enum class Side {w, b};

struct UndoInfo {
    Piece capturedPiece;
    int whiteKingSquare;
    int blackKingSquare;
    Side sideToMove;
    bool kingHasMovedWhite, kingHasMovedBlack;
    bool rookLeftHasMovedWhite, rookRightHasMovedWhite;
    bool rookLeftHasMovedBlack, rookRightHasMovedBlack;
};

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

        MoveGenResult generateAllValidMovesForSide(Side side);
        bool moveStopsCheck(Move& move, std::vector<int>& attackingSquares);

        const std::vector<Piece>& getWhitePieces() const {return whitePieces;}
        const std::vector<Piece>& getBlackPieces() const {return blackPieces;}
        Side getSideToMove() const {return sideToMove;}
        void setSideToMove(Side side) {sideToMove = side;}
        const Board& getBoard() const {return board;}
        bool makeMove(const Move& move, const MoveGenResult& currentMoves);
        UndoInfo applyMove(const Move& move);
        void undoMove(const Move& move, const UndoInfo& undo);
        std::string toFEN() const;

        template<size_t N, typename Func>
        void walkDirectionsAndDo(int startSquare, const int (&directions)[N], bool limitedMovementPiece, Func func);
        bool isSquareAttacked(int square, Color color);

    private:
        Board board;
        Side sideToMove;
        std::vector<Piece> whitePieces;
        std::vector<Piece> blackPieces;
        static constexpr const char* START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w";

        void generateValidMoves(int square, MoveGenResult& result);
        void getValidMovesPawn(int& count, int square, MoveGenResult& result);
        void getValidMovesKnight(int& count, int square, MoveGenResult& result);
        void generateDiagonalMoves(int& count, int square, bool cap, Color color, MoveGenResult& result);
        void generateOrthoganalMoves(int& count, int square, bool cap, Color color, MoveGenResult& result);
        void generateCheckResolutions(int square, Color color, MoveGenResult& result);

        bool kingHasMovedWhite = false;
        bool kingHasMovedBlack = false;
        bool rookLeftHasMovedWhite = false;
        bool rookRightHasMovedWhite = false;
        bool rookLeftHasMovedBlack = false;
        bool rookRightHasMovedBlack = false;
};

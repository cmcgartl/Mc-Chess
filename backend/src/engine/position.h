#pragma once
#include "board.h"
#include "vector"
#include "move.h"
#include "zobrist.h"

class Position;
enum class Side {w, b};

struct UndoInfo {
    Piece capturedPiece;
    int whiteKingSquare;
    int blackKingSquare;
    Side sideToMove;
    bool kingHasMovedWhite, kingHasMovedBlack;
    bool rookLeftHasMovedWhite, rookRightHasMovedWhite;
    bool rookLeftHasMovedBlack, rookRightHasMovedBlack;
    int enPassantSquare;
    bool isEndGame;
    uint64_t hash;
    UndoInfo() = default;
    UndoInfo(const Move& m, const Position& p);
};

class Position{
    public:
    
        Position():board(START_FEN) , sideToMove(Side::w){
            generatePieceLists();
            hash = computeHash();
        };

        Position(const std::string& FEN): board(FEN){
            generatePieceLists();
            hash = computeHash();
        }

        bool tryMove(int src, int dest);
        void generatePieceLists();
        uint64_t computeHash() const;
        uint64_t getHash() const { return hash; }
        MoveGenResult generateAllValidMovesForSide(Side side);
        Side getSideToMove() const {return sideToMove;}
        UndoInfo applyMove(const Move& move);
        std::string toFEN() const;
        const std::vector<Piece>& getWhitePieces() const {return whitePieces;}
        const std::vector<Piece>& getBlackPieces() const {return blackPieces;}
        void setSideToMove(Side side) {sideToMove = side;}
        const Board& getBoard() const {return board;}
        bool makeMove(const Move& move, const MoveGenResult& currentMoves);
        void undoMove(const Move& move, const UndoInfo& undo);
        int getEnPassantSquare() const {return enPassantSquare;}
        bool getIsEndGame() const {return isEndGame;}

    private:
        Board board;
        Side sideToMove;
        std::vector<Piece> whitePieces;
        std::vector<Piece> blackPieces;
        uint64_t hash;
        static constexpr const char* START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w";
        bool kingHasMovedWhite = false;
        bool kingHasMovedBlack = false;
        bool rookLeftHasMovedWhite = false;
        bool rookRightHasMovedWhite = false;
        bool rookLeftHasMovedBlack = false;
        bool rookRightHasMovedBlack = false;
        bool isEndGame = false;
        int enPassantSquare = -1;

        void generateValidMoves(int square, MoveGenResult& result);
        friend struct UndoInfo;
};

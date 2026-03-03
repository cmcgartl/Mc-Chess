#pragma once
#include <iostream>
#include "piece.h"
#include <array>
#include "move.h"


class Board {
public:
    Board();
    Board(const std::string& FEN);
    void printBoard() const;
    Piece& at(int square);
    const Piece& at(int square) const;
    void movePiece(const Move& move, uint64_t& hash);
    void clearBoard();
    void verifyFEN(const std::string& FEN) const;
    void printSquareValues() const;
    int whiteKingSquare;
    int blackKingSquare;
    std::array<Piece, 64>& getSquares(){return squares;}
    void updateHash(const Move& move, uint64_t& hash);
    const std::array<Piece, 64>& getSquares() const { return squares; }
    std::string toFEN() const;

private:
    std::array<Piece, 64> squares;
    static bool isValidLetter(char c);
    static PieceType charToPieceType(char c);
    static bool isNum(char c);
};
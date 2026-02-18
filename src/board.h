#pragma once
#include <iostream>
#include "piece.h"
#include <array>


class Board{
public:
    Board();
    Board(const std::string& FEN);
    void printBoard() const;
    Piece& at(int square);
    void movePiece(int src, int dest);
    void clearBoard();
    void verifyFEN(const std::string& FEN) const;
    void printSquareValues() const;
    int whiteKingSquare;
    int blackKingSquare;
    std::array<Piece, 64>& getSquares() {return squares;}

private:
    std::array<Piece, 64> squares;
    static bool isValidLetter(char c);
    static PieceType charToPieceType(char c);
    static bool isNum(char c);
};
#pragma once
#include <iostream>
#include "piece.h"
#include <array>


class Board{
public:
    Board();
    Board(const std::string& FEN);
    void printBoard() const;
    Piece at(int square) const;
    void movePiece(int src, int dest);
    void clearBoard();
    void verifyFEN(const std::string& FEN) const;
    void printSquareValues() const;

private:
    std::array<Piece, 64> squares;
    static bool isValidLetter(char c);
    static PieceType charToPieceType(char c);
    static bool isNum(char c);
};
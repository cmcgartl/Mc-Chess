#include <iostream>
#pragma once

enum class PieceType{
    P,
    N,
    B,
    R,
    K,
    Q,
    None
};

enum class Color{
    w,
    b,
    None
};

struct Piece {
    PieceType type;
    Color color;
    bool pinned;
    Pin* pin;
    Piece() : type(PieceType::None), color(Color::None), pinned(false), pin(nullptr) {}
    Piece(PieceType t, Color c) : type(t), color(c), pinned(false), pin(nullptr) {}
    Piece(PieceType t, Color c, Pin* p): type(t), color(c), pin(p), pinned(p == nullptr){}


    bool operator==(const Piece& other) const {
        return type == other.type &&
        color == other.color;
    }

};

//what does a piece need?
//piece type
//position
//move function
//get available moves()
//move is valid()
//value -- can be implicit
//available moves
//Color!!!

//class square
//occupied/not occupied
//occupied by what piece
//cell #


//capture function: takes in capturer, capturee
#include <iostream>
#pragma once

struct Pin;

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
    bool pinnedD;
    bool pinnedO;
    std::vector<Pin*> pins;
    std::vector<int> pinDirections;
    Piece() : type(PieceType::None), color(Color::None), pinnedD(false), pinnedO(false) {}
    Piece(PieceType t, Color c) : type(t), color(c), pinnedD(false), pinnedO(false) {}
    Piece(PieceType t, Color c, Pin* p, int direction, bool pD, bool pO): type(t), color(c), pinnedD(pD), pinnedO(pO){
        if(p == nullptr){
            pinnedD = false;
            pinnedO = false;
            return;
        }
        pinDirections.push_back(direction);
        pins.push_back(p);
    }


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
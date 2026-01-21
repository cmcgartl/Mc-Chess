#include <iostream>


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
    Piece() : type(PieceType::None), color(Color::None) {}
    Piece(PieceType t, Color c) : type(t), color(c) {}

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
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
    Piece() : type(PieceType::None), color(Color::None) {}
    Piece(PieceType t, Color c) : type(t), color(c) {}

    bool operator==(const Piece& other) const {
        return type == other.type &&
        color == other.color;
    }
};
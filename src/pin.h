#pragma once
#include <cstdint>
#include "piece.h"

enum class PinType{
    Diagonal,
    Orthoganal
};

struct Piece;

struct Pin {
    uint8_t pinSquare;
    uint8_t from;
    uint8_t to;
    Piece& pinnedPiece;
    PinType pType;

    Pin(uint8_t pinSquare, uint8_t from, uint8_t to, Piece& pinnedPiece, PinType pType):
        pinSquare(pinSquare), from(from), to(to), pinnedPiece(pinnedPiece), pType(pType){};

    bool operator==(const Pin& other) const {
        return pinSquare == other.pinSquare &&
            from == other.from &&
            to == other.to &&
            pinnedPiece == other.pinnedPiece;
    }
};

#pragma once
#include <cstdint>
#include "piece.h"

struct Pin {
    uint8_t pinSquare;
    uint8_t from;
    uint8_t to;
    Piece pinnedPiece;
    Pin(uint8_t pinSquare, uint8_t from, uint8_t to, Piece pinnedPiece):
        pinSquare(pinSquare), from(from), to(to), pinnedPiece(pinnedPiece){};

    bool operator==(const Pin& other) const {
        return pinSquare == other.pinSquare &&
            from == other.from &&
            to == other.to &&
            pinnedPiece == other.pinnedPiece;
    }
};

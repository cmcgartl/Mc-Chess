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
};
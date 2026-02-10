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
            pinnedPiece == other.pinnedPiece &&
            pType == other.pType;

    }

    void printPin(){
        std::cout << "PinSquare: " << std::to_string(pinSquare) << '\n';
        std::cout << "from: " << std::to_string(from) << '\n';
        std::cout << "to: " << std::to_string(to) << '\n';
    }
};

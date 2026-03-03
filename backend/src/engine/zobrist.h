#pragma once
#include <cstdint>
#include <random>

struct Zobrist {
    static uint64_t pieceKeys[6][2][64];
    static uint64_t sideToMoveKey;
    static uint64_t whiteCastleRightsKey;
    static uint64_t blackCastleRightsKey;
    static uint64_t enPassantFileKeys[8];

    static void init();
};

#pragma once
#include <cstdint>
#include <random>

struct Zobrist {
    static uint64_t pieceKeys[6][2][64];
    static uint64_t sideToMoveKey;
    static void init();
};

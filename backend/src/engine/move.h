#pragma once
#include <cstdint>

struct Move {
    uint8_t from;
    uint8_t to;
    Move(uint8_t from, uint8_t to):from(from), to(to){};

    bool operator==(const Move& other) const {
        return from == other.from &&
            to == other.to;
    }
};
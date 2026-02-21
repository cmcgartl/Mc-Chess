#pragma once
#include <cstdint>
#include <vector>
#include <array>

enum class MoveType{Normal, CastleLong, CastleShort, EP, P};
enum class PositionStatus{InProgress, CheckMate, Stalemate, MoveDraw};

struct Move {
    uint8_t from;
    uint8_t to;
    MoveType type;
    Move(uint8_t from, uint8_t to):from(from), to(to), type(MoveType::Normal){};
    Move(uint8_t from, uint8_t to, MoveType type): from(from), to(to), type(type){};

    bool operator==(const Move& other) const {
        return from == other.from &&
            to == other.to;
    }
};

struct MoveGenResult {
    std::vector<Move> moves;
    std::array<int, 64> startIndices;
    std::array<int, 64> counts;
    std::vector<int> attacksOnWhiteKing;
    std::vector<int> attacksOnBlackKing;
    PositionStatus status = PositionStatus::InProgress;

    MoveGenResult() {
        startIndices.fill(-1);
        counts.fill(0);
    }
};
#include "position.h"
#include "move.h"

//define move/diretion distances
//moves that change files are ordered together for wrap around handling (moves that could go off the side edge of the board)
static constexpr int DiagonalDirections[4] = {7, -9, 9, -7};
static constexpr int OrthoganalDirections[4] = {-1, 1, -8, 8};
static constexpr int KnightMoves[8] = {-10, 6, -17, 15, 17, -15, 10, -6};

static constexpr int pawnDirectionsWhite[] = {8, 7, 9};
static constexpr int pawnDirectionsBlack[] = {-8, -7, -9};
static constexpr int pawnAttackerDirectionsBlack[] = {7, 9};
static constexpr int pawnAttackerDirectionsWhite[] = {-7, -9};

struct DirectionSet {
    const int* dirs;
    size_t count;
    bool limited;
};

DirectionSet getDirections(PieceType type, Color color);
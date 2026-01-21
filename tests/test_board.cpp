#include "../third_party/catch.hpp"
#include "../board.h"

// Adjust this FEN string depending on what your Board constructor expects.
// If your Board only parses piece placement, keep only the board section.
// If it parses full FEN, include " w - - 0 1" etc.
static const std::string START_POS =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

TEST_CASE("Board default constructor clears board") {
    Board b;
    for (int sq = 0; sq < 64; ++sq) {
        REQUIRE(b.at(sq).type == PieceType::None);
    }
}

TEST_CASE("Board FEN constructor places pieces") {
    Board b(START_POS);

    // If square 0 is a8, then:
    // a8 rook = black rook
    REQUIRE(b.at(0).type == PieceType::R);
    REQUIRE(b.at(0).color == Color::b);

    // e1 king is at square 60 if 0=a8 indexing
    REQUIRE(b.at(60).type == PieceType::K);
    REQUIRE(b.at(60).color == Color::w);

    // a2 pawn is at square 48 if 0=a8 indexing
    REQUIRE(b.at(48).type == PieceType::P);
    REQUIRE(b.at(48).color == Color::w);
}

/*TEST_CASE("movePiece moves a piece and clears source") {
    Board b(START_POS);

    // Move white pawn a2 -> a3 (48 -> 40) with 0=a8 indexing
    b.movePiece(48, 40);

    REQUIRE(b.at(48).type == PieceType::None);
    REQUIRE(b.at(40).type == PieceType::P);
    REQUIRE(b.at(40).color == Color::w);
}*/

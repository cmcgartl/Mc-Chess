#include "../third_party/catch.hpp"
#include "../position.h"

TEST_CASE("Test number of Pawn moves") {
    Position p;
    REQUIRE(p.getWhitePieces().size() == 16);
    REQUIRE(p.getBlackPieces().size() == 16);

    Position onePawnLeft("8/P7/8/8/8/8/8/8");
    REQUIRE(onePawnLeft.getWhitePieces().size() == 1);
    REQUIRE(onePawnLeft.getBlackPieces().size() == 0);
    REQUIRE(onePawnLeft.getValidMovesPawn(8).size() == 1);

    Position onePawnRight("8/8/8/7P/8/8/8/8");
    REQUIRE(onePawnRight.getWhitePieces().size() == 1);
    REQUIRE(onePawnRight.getBlackPieces().size() == 0);
    REQUIRE(onePawnRight.getValidMovesPawn(31).size() == 1);

    Position onePawnMid("8/8/8/3P4/8/8/8/8");
    REQUIRE(onePawnMid.getWhitePieces().size() == 1);
    REQUIRE(onePawnMid.getBlackPieces().size() == 0);
    REQUIRE(onePawnMid.getValidMovesPawn(27).size() == 1);

    Position oneTwoCaptures("8/8/2n1n3/3P4/8/8/8/8");
    REQUIRE(oneTwoCaptures.getWhitePieces().size() == 1);
    REQUIRE(oneTwoCaptures.getBlackPieces().size() == 2);
    REQUIRE(oneTwoCaptures.getValidMovesPawn(27).size() == 3);

    Position oneTwoCapturesBlocked("8/8/2nrn3/3P4/8/8/8/8");
    REQUIRE(oneTwoCapturesBlocked.getWhitePieces().size() == 1);
    REQUIRE(oneTwoCapturesBlocked.getBlackPieces().size() == 3);
    REQUIRE(oneTwoCapturesBlocked.getValidMovesPawn(27).size() == 2);

    Position oneTwoCapturesRank2("8/8/8/8/8/2n1n3/3P4/8");
    REQUIRE(oneTwoCapturesRank2.getWhitePieces().size() == 1);
    REQUIRE(oneTwoCapturesRank2.getBlackPieces().size() == 2);
    REQUIRE(oneTwoCapturesRank2.getValidMovesPawn(51).size() == 4);

    Position oneTwoCapturesRank2Blocked1("8/8/8/8/3p4/2n1n3/3P4/8");
    REQUIRE(oneTwoCapturesRank2Blocked1.getWhitePieces().size() == 1);
    REQUIRE(oneTwoCapturesRank2Blocked1.getBlackPieces().size() == 3);
    REQUIRE(oneTwoCapturesRank2Blocked1.getValidMovesPawn(51).size() == 3);

    Position oneTwoCapturesRank2Blocked2("8/8/8/8/8/2npn3/3P4/8");
    REQUIRE(oneTwoCapturesRank2Blocked2.getWhitePieces().size() == 1);
    REQUIRE(oneTwoCapturesRank2Blocked2.getBlackPieces().size() == 3);
    REQUIRE(oneTwoCapturesRank2Blocked2.getValidMovesPawn(51).size() == 2);

}

TEST_CASE("Test number of bishop moves"){
    Position oneMid("8/8/8/3B4/8/8/8/8");
    REQUIRE(oneMid.getWhitePieces().size() == 1);
    REQUIRE(oneMid.getBlackPieces().size() == 0);
    REQUIRE(oneMid.getValidMovesBishop(27).size() == 13);

    Position oneBottomLeft("8/8/8/8/8/8/8/b7");
    REQUIRE(oneBottomLeft.getWhitePieces().size() == 0);
    REQUIRE(oneBottomLeft.getBlackPieces().size() == 1);
    REQUIRE(oneBottomLeft.getValidMovesBishop(56).size() == 7);

    Position oneTopLeft("b7/8/8/8/8/8/8/8");
    REQUIRE(oneTopLeft.getWhitePieces().size() == 0);
    REQUIRE(oneTopLeft.getBlackPieces().size() == 1);
    REQUIRE(oneTopLeft.getValidMovesBishop(0).size() == 7);

    Position middleWithCaptures("8/6p1/3n4/4B3/8/8/1P6/8");
    REQUIRE(middleWithCaptures.getWhitePieces().size() == 2);
    REQUIRE(middleWithCaptures.getBlackPieces().size() == 2);
    REQUIRE(middleWithCaptures.getValidMovesBishop(28).size() == 8);

    Position middleWithCaptures1("1n5r/8/8/4B3/8/8/7P/8");
    REQUIRE(middleWithCaptures1.getWhitePieces().size() == 2);
    REQUIRE(middleWithCaptures1.getBlackPieces().size() == 2);
    REQUIRE(middleWithCaptures1.getValidMovesBishop(28).size() == 12);

}

TEST_CASE("Test number of queen moves"){
    Position oneMid("8/8/8/3Q4/8/8/8/8");
    REQUIRE(oneMid.getWhitePieces().size() == 1);
    REQUIRE(oneMid.getBlackPieces().size() == 0);
    REQUIRE(oneMid.getValidMovesQueen(27).size() == 27);

    Position surroundedByEnemies("8/8/8/2prp3/2bQb3/2prp3/8/8");
    REQUIRE(surroundedByEnemies.getWhitePieces().size() == 1);
    REQUIRE(surroundedByEnemies.getBlackPieces().size() == 8);
    REQUIRE(surroundedByEnemies.getValidMovesQueen(35).size() == 8);

    Position surroundedByFriends("8/8/8/2prp3/2bqb3/2prp3/8/8");
    REQUIRE(surroundedByFriends.getWhitePieces().size() == 0);
    REQUIRE(surroundedByFriends.getBlackPieces().size() == 9);
    REQUIRE(surroundedByFriends.getValidMovesQueen(35).size() == 0);

    Position multipleCapturesBlocks("8/b2R4/8/4p3/N2qR3/8/8/3r4");
    REQUIRE(multipleCapturesBlocks.getWhitePieces().size() == 3);
    REQUIRE(multipleCapturesBlocks.getBlackPieces().size() == 4);
    REQUIRE(multipleCapturesBlocks.getValidMovesQueen(35).size() == 17);
}

TEST_CASE("Test number of knight moves"){
    Position oneMid("8/8/8/3N4/8/8/8/8");
    REQUIRE(oneMid.getWhitePieces().size() == 1);
    REQUIRE(oneMid.getBlackPieces().size() == 0);
    REQUIRE(oneMid.getValidMovesKnight(27).size() == 8);

    Position oneLeftEdge("8/8/8/N7/8/8/8/8");
    REQUIRE(oneLeftEdge.getWhitePieces().size() == 1);
    REQUIRE(oneLeftEdge.getBlackPieces().size() == 0);
    REQUIRE(oneLeftEdge.getValidMovesKnight(24).size() == 4);

    Position oneRightEdge("8/8/8/7N/8/8/8/8");
    REQUIRE(oneRightEdge.getWhitePieces().size() == 1);
    REQUIRE(oneRightEdge.getBlackPieces().size() == 0);
    REQUIRE(oneRightEdge.getValidMovesKnight(31).size() == 4);

    Position oneCornerTR("7N/8/8/8/8/8/8/8");
    REQUIRE(oneCornerTR.getWhitePieces().size() == 1);
    REQUIRE(oneCornerTR.getBlackPieces().size() == 0);
    REQUIRE(oneCornerTR.getValidMovesKnight(7).size() == 2);

    Position oneCornerTR2("8/6N1/8/8/8/8/8/8");
    REQUIRE(oneCornerTR2.getWhitePieces().size() == 1);
    REQUIRE(oneCornerTR2.getBlackPieces().size() == 0);
    REQUIRE(oneCornerTR2.getValidMovesKnight(14).size() == 4);

    Position oneCornerBL("8/8/8/8/8/8/8/N7");
    REQUIRE(oneCornerBL.getWhitePieces().size() == 1);
    REQUIRE(oneCornerBL.getBlackPieces().size() == 0);
    REQUIRE(oneCornerBL.getValidMovesKnight(56).size() == 2);

    Position oneCornerBL2("8/8/8/8/8/8/1N6/8");
    REQUIRE(oneCornerBL2.getWhitePieces().size() == 1);
    REQUIRE(oneCornerBL2.getBlackPieces().size() == 0);
    REQUIRE(oneCornerBL2.getValidMovesKnight(49).size() == 4);

    Position oneMidCaptures("8/3q1b2/2P3b1/4N3/2r3P1/3r1P2/8/8");
    REQUIRE(oneMidCaptures.getWhitePieces().size() == 4);
    REQUIRE(oneMidCaptures.getBlackPieces().size() == 5);
    REQUIRE(oneMidCaptures.getValidMovesKnight(28).size() == 5);
}

TEST_CASE("Test number of king moves"){
    Position oneMid("8/8/8/3K4/8/8/8/8");
    REQUIRE(oneMid.getWhitePieces().size() == 1);
    REQUIRE(oneMid.getBlackPieces().size() == 0);
    REQUIRE(oneMid.getValidMovesKing(27).size() == 8);

    Position oneCornerBL("8/8/8/8/8/8/8/K7");
    REQUIRE(oneCornerBL.getWhitePieces().size() == 1);
    REQUIRE(oneCornerBL.getBlackPieces().size() == 0);
    REQUIRE(oneCornerBL.getValidMovesKing(56).size() == 3);

    Position oneCornerTR("7K/8/8/8/8/8/8/8");
    REQUIRE(oneCornerTR.getWhitePieces().size() == 1);
    REQUIRE(oneCornerTR.getBlackPieces().size() == 0);
    REQUIRE(oneCornerTR.getValidMovesKing(7).size() == 3);

    Position oneMidCaptures("8/8/3qbP2/3rKP2/3rPb2/8/8/8");
    REQUIRE(oneMidCaptures.getWhitePieces().size() == 4);
    REQUIRE(oneMidCaptures.getBlackPieces().size() == 5);
    REQUIRE(oneMidCaptures.getValidMovesKing(28).size() == 5);
}



//---------------------------------------------------------------------//
//-------------- BEGIN GPT GENERATED TEST CASES------------------------//
//---------------ALL FOLLOWING TEST CASES ARE GPT GENERATED------------//
//---------------ALL PREVIOUS TEST CASES ARE MADE BY ME :)-------------//


// Adjust this FEN string depending on what your Board constructor expects.
// If your Board only parses piece placement, keep only the board section.
// If it parses full FEN, include " w - - 0 1" etc.
static const std::string START_POS =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

TEST_CASE("default constructor creates cleared board") {
    Board b;
    for (int i = 0; i < 64; i++) {
        REQUIRE(b.at(i).type == PieceType::None);
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

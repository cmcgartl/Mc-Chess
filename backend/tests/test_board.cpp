#include "catch.hpp"
#include "position.h"
#include "game.h"

// Helper to extract moves for a specific square from a MoveGenResult
static std::vector<Move> getMovesAt(const MoveGenResult& result, int square) {
    std::vector<Move> moves;
    int start = result.startIndices[square];
    if (start < 0) return moves;
    for (int i = 0; i < result.counts[square]; i++) {
        moves.push_back(result.moves[start + i]);
    }
    return moves;
}

TEST_CASE("Test number of Pawn moves") {
    Position p;
    REQUIRE(p.getWhitePieces().size() == 16);
    REQUIRE(p.getBlackPieces().size() == 16);

    Position onePawnLeft("8/P7/8/8/8/8/8/8");
    REQUIRE(onePawnLeft.getWhitePieces().size() == 1);
    REQUIRE(onePawnLeft.getBlackPieces().size() == 0);
    auto result = onePawnLeft.generateAllValidMovesForSide(Side::w);
    REQUIRE(result.moves.size() == 1);

    Position onePawnRight("8/8/8/7P/8/8/8/8");
    REQUIRE(onePawnRight.getWhitePieces().size() == 1);
    REQUIRE(onePawnRight.getBlackPieces().size() == 0);
    result = onePawnRight.generateAllValidMovesForSide(Side::w);
    REQUIRE(result.moves.size() == 1);

    Position onePawnMid("8/8/8/3P4/8/8/8/8");
    REQUIRE(onePawnMid.getWhitePieces().size() == 1);
    REQUIRE(onePawnMid.getBlackPieces().size() == 0);
    result = onePawnMid.generateAllValidMovesForSide(Side::w);
    REQUIRE(result.moves.size() == 1);

    Position oneTwoCaptures("8/8/2n1n3/3P4/8/8/8/8");
    REQUIRE(oneTwoCaptures.getWhitePieces().size() == 1);
    REQUIRE(oneTwoCaptures.getBlackPieces().size() == 2);
    result = oneTwoCaptures.generateAllValidMovesForSide(Side::w);
    REQUIRE(result.moves.size() == 3);

    Position oneTwoCapturesBlocked("8/8/2nrn3/3P4/8/8/8/8");
    REQUIRE(oneTwoCapturesBlocked.getWhitePieces().size() == 1);
    REQUIRE(oneTwoCapturesBlocked.getBlackPieces().size() == 3);
    result = oneTwoCapturesBlocked.generateAllValidMovesForSide(Side::w);
    REQUIRE(result.moves.size() == 2);

    Position oneTwoCapturesRank2("8/8/8/8/8/2n1n3/3P4/8");
    REQUIRE(oneTwoCapturesRank2.getWhitePieces().size() == 1);
    REQUIRE(oneTwoCapturesRank2.getBlackPieces().size() == 2);
    result = oneTwoCapturesRank2.generateAllValidMovesForSide(Side::w);
    REQUIRE(result.moves.size() == 4);

    Position oneTwoCapturesRank2Blocked1("8/8/8/8/3p4/2n1n3/3P4/8");
    REQUIRE(oneTwoCapturesRank2Blocked1.getWhitePieces().size() == 1);
    REQUIRE(oneTwoCapturesRank2Blocked1.getBlackPieces().size() == 3);
    result = oneTwoCapturesRank2Blocked1.generateAllValidMovesForSide(Side::w);
    REQUIRE(result.moves.size() == 3);

    Position oneTwoCapturesRank2Blocked2("8/8/8/8/8/2npn3/3P4/8");
    REQUIRE(oneTwoCapturesRank2Blocked2.getWhitePieces().size() == 1);
    REQUIRE(oneTwoCapturesRank2Blocked2.getBlackPieces().size() == 3);
    result = oneTwoCapturesRank2Blocked2.generateAllValidMovesForSide(Side::w);
    REQUIRE(result.moves.size() == 2);

}

TEST_CASE("Test number of bishop moves"){
    Position oneMid("8/8/8/3B4/8/8/8/8");
    REQUIRE(oneMid.getWhitePieces().size() == 1);
    REQUIRE(oneMid.getBlackPieces().size() == 0);
    auto result = oneMid.generateAllValidMovesForSide(Side::w);
    REQUIRE(result.moves.size() == 13);

    Position oneBottomLeft("8/8/8/8/8/8/8/b7");
    REQUIRE(oneBottomLeft.getWhitePieces().size() == 0);
    REQUIRE(oneBottomLeft.getBlackPieces().size() == 1);
    result = oneBottomLeft.generateAllValidMovesForSide(Side::b);
    REQUIRE(result.moves.size() == 7);

    Position oneTopLeft("b7/8/8/8/8/8/8/8");
    REQUIRE(oneTopLeft.getWhitePieces().size() == 0);
    REQUIRE(oneTopLeft.getBlackPieces().size() == 1);
    result = oneTopLeft.generateAllValidMovesForSide(Side::b);
    REQUIRE(result.moves.size() == 7);

    Position middleWithCaptures("8/6p1/3n4/4B3/8/8/1P6/8");
    REQUIRE(middleWithCaptures.getWhitePieces().size() == 2);
    REQUIRE(middleWithCaptures.getBlackPieces().size() == 2);
    result = middleWithCaptures.generateAllValidMovesForSide(Side::w);
    REQUIRE(getMovesAt(result, 28).size() == 8);

    Position middleWithCaptures1("1n5r/8/8/4B3/8/8/7P/8");
    REQUIRE(middleWithCaptures1.getWhitePieces().size() == 2);
    REQUIRE(middleWithCaptures1.getBlackPieces().size() == 2);
    result = middleWithCaptures1.generateAllValidMovesForSide(Side::w);
    REQUIRE(getMovesAt(result, 28).size() == 12);

}



TEST_CASE("Test number of queen moves"){
    Position oneMid("8/8/8/3Q4/8/8/8/8");
    REQUIRE(oneMid.getWhitePieces().size() == 1);
    REQUIRE(oneMid.getBlackPieces().size() == 0);
    auto result = oneMid.generateAllValidMovesForSide(Side::w);
    REQUIRE(result.moves.size() == 27);

    Position surroundedByEnemies("8/8/8/2prp3/2bQb3/2prp3/8/8");
    REQUIRE(surroundedByEnemies.getWhitePieces().size() == 1);
    REQUIRE(surroundedByEnemies.getBlackPieces().size() == 8);
    result = surroundedByEnemies.generateAllValidMovesForSide(Side::w);
    REQUIRE(getMovesAt(result, 35).size() == 8);

    Position surroundedByFriends("8/8/8/2prp3/2bqb3/2prp3/8/8");
    REQUIRE(surroundedByFriends.getWhitePieces().size() == 0);
    REQUIRE(surroundedByFriends.getBlackPieces().size() == 9);
    result = surroundedByFriends.generateAllValidMovesForSide(Side::b);
    REQUIRE(getMovesAt(result, 35).size() == 0);

    Position multipleCapturesBlocks("8/b2R4/8/4p3/N2qR3/8/8/3r4");
    REQUIRE(multipleCapturesBlocks.getWhitePieces().size() == 3);
    REQUIRE(multipleCapturesBlocks.getBlackPieces().size() == 4);
    result = multipleCapturesBlocks.generateAllValidMovesForSide(Side::b);
    REQUIRE(getMovesAt(result, 35).size() == 17);
}



TEST_CASE("Test number of knight moves"){

    std::pair<std::string, int> testInputs[] = {
        std::make_pair("8/8/8/3N4/8/8/8/8", 8),
        std::make_pair("8/8/8/N7/8/8/8/8", 4),
        std::make_pair("8/8/8/7N/8/8/8/8", 4),
        std::make_pair("7N/8/8/8/8/8/8/8", 2),
        std::make_pair("8/6N1/8/8/8/8/8/8", 4),
        std::make_pair("8/8/8/8/8/8/8/N7", 2),
        std::make_pair("8/8/8/8/8/8/1N6/8", 4),
        std::make_pair("8/3q1b2/2P3b1/4N3/2r3P1/3r1P2/8/8", 5)
    };
    int squares[] = {27, 24, 31, 7, 14, 56, 49, 28};

    int i = 0;
    for(auto& test : testInputs){
        Position testPosition(test.first);
        auto result = testPosition.generateAllValidMovesForSide(Side::w);
        REQUIRE(getMovesAt(result, squares[i]).size() == test.second);
        i++;
    }
}


TEST_CASE("Test number of king moves"){
    std::pair<std::string, int> testInputs[] = {
        std::make_pair("8/8/8/3K4/8/8/8/8", 8),
        std::make_pair("8/8/8/8/8/8/8/K7", 3),
        std::make_pair("7K/8/8/8/8/8/8/8", 3),
        std::make_pair("8/8/8/3PK3/8/8/8/8", 7),
    };

    int squares[] = {27, 56, 7, 28};

    int i = 0;
    for(auto& test : testInputs){
        Position testPosition(test.first);
        auto result = testPosition.generateAllValidMovesForSide(Side::w);
        REQUIRE(getMovesAt(result, squares[i]).size() == test.second);
        i++;
    }
}


TEST_CASE("test number of rook moves"){

    std::pair<std::string, int> testInputs[] = {
        std::make_pair("8/8/8/3R4/8/8/8/8", 14),
        std::make_pair("8/8/8/8/8/8/8/R7", 14),
        std::make_pair("8/8/4P3/1b2Rn2/8/8/8/4b3", 8),
    };

    int squares[] = {27, 56, 28};

    int i = 0;
    for(auto& test : testInputs){
        Position testPosition(test.first);
        auto result = testPosition.generateAllValidMovesForSide(Side::w);
        REQUIRE(getMovesAt(result, squares[i]).size() == test.second);
        i++;
    }
}

TEST_CASE("test number of moves for all pieces in a positon"){
    Position p;
    REQUIRE(p.getWhitePieces().size() == 16);
    REQUIRE(p.getBlackPieces().size() == 16);
    auto result = p.generateAllValidMovesForSide(Side::w);
    REQUIRE(result.moves.size() == 20);
}

TEST_CASE("test check detection"){
    Position p;
    REQUIRE(p.getWhitePieces().size() == 16);
    REQUIRE(p.getBlackPieces().size() == 16);
    auto result = p.generateAllValidMovesForSide(Side::w);
    REQUIRE(result.moves.size() == 20);
    REQUIRE(result.attacksOnBlackKing.size() == 0);
    REQUIRE(result.attacksOnWhiteKing.size() == 0);

    Position p1("8/8/8/8/8/3n1N2/8/4K3");
    result = p1.generateAllValidMovesForSide(Side::w);
    REQUIRE(result.attacksOnBlackKing.size() == 0);
    REQUIRE(result.attacksOnWhiteKing.size() == 1);

    Position p2("8/8/8/8/5p1p/6K1/8/8");
    result = p2.generateAllValidMovesForSide(Side::w);
    REQUIRE(result.attacksOnBlackKing.size() == 0);
    REQUIRE(result.attacksOnWhiteKing.size() == 2);

    Position p3("3B4/1N6/8/k2R1n2/1P2p1p1/QN4K1/8/8 b - - 0 1");
    result = p3.generateAllValidMovesForSide(Side::w);
}

TEST_CASE("Test Pinned Piece Detection"){

    // Test that pinned pieces have restricted moves
    // Instead of checking pin objects directly, verify move counts are correct

    // Bishop pinned diagonally - can only move along pin ray
    Position bishopPinnedDiagonal("8/8/5q2/4B3/3K4/8/8/8");
    auto result = bishopPinnedDiagonal.generateAllValidMovesForSide(Side::w);
    std::vector<Move> bishopMoves = getMovesAt(result, 28);
    REQUIRE(bishopMoves.size() == 1); // Can only capture queen at f6

    // Bishop pinned orthogonally - cannot move at all
    Position bishopPinnedOrthogonal("8/8/8/1r1BK3/8/8/8/8 w - - 0 1");
    result = bishopPinnedOrthogonal.generateAllValidMovesForSide(Side::w);
    std::vector<Move> bishopPinnedOrtho = getMovesAt(result, 27);
    REQUIRE(bishopPinnedOrtho.size() == 0);
}

TEST_CASE("Test Pinned piece movement generation"){

    // Bishop pinned diagonally - can only move along pin ray toward attacker
    Position bishopPinnedDiagonal("8/8/5q2/4B3/3K4/8/8/8");
    auto result = bishopPinnedDiagonal.generateAllValidMovesForSide(Side::w);
    std::vector<Move> bishopMoves = getMovesAt(result, 28);
    REQUIRE(bishopMoves.size() == 1);
    REQUIRE(bishopMoves[0].to == 21); // Can only move to f6

    // Bishop pinned orthogonally - cannot move at all
    Position bishopPinnedOrthogonal("8/8/8/1r1BK3/8/8/8/8 w - - 0 1");
    result = bishopPinnedOrthogonal.generateAllValidMovesForSide(Side::w);
    std::vector<Move> bishopPinnedOrtho = getMovesAt(result, 27);
    REQUIRE(bishopPinnedOrtho.size() == 0);

    // Queen pinned diagonally - can move multiple squares along pin ray
    Position queenPinnedDiagonal("8/6K1/8/8/3Q4/8/8/q7 w - - 0 1");
    result = queenPinnedDiagonal.generateAllValidMovesForSide(Side::w);
    std::vector<Move> queenMoves = getMovesAt(result, 35);
    REQUIRE(queenMoves.size() == 5);
    // Check that moves are along the pin ray
    bool hasB2 = false, hasA1 = false;
    for(const auto& move : queenMoves) {
        if(move.to == 49) hasB2 = true;  // b2
        if(move.to == 56) hasA1 = true;  // a1 (capture)
    }
    REQUIRE(hasB2);
    REQUIRE(hasA1);
}


TEST_CASE("isSquareAttacked - diagonal attack by bishop"){
    Position p("8/1b6/8/8/4K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == true);
}

TEST_CASE("isSquareAttacked - diagonal blocked by piece"){
    Position p("8/1b6/2P5/8/4K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == false);
}

TEST_CASE("isSquareAttacked - orthogonal attack by rook"){
    Position p("4r3/8/8/8/4K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == true);
}

TEST_CASE("isSquareAttacked - orthogonal blocked by piece"){
    Position p("4r3/8/8/4P3/4K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == false);
}

TEST_CASE("isSquareAttacked - knight attack"){
    Position p("8/8/5n2/8/4K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == true);
}

TEST_CASE("isSquareAttacked - knight not attacking"){
    Position p("n7/8/8/8/4K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == false);
}

TEST_CASE("isSquareAttacked - black pawn attacks white piece"){
    Position p("8/8/8/3p4/4K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == true);
}

TEST_CASE("isSquareAttacked - white pawn attacks black piece"){
    Position p("8/8/8/4k3/3P4/8/8/8");
    REQUIRE(p.isSquareAttacked(28, Color::b) == true);
}

TEST_CASE("isSquareAttacked - pawn does not attack forward"){
    Position p("8/8/8/4p3/4K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == false);
}

TEST_CASE("isSquareAttacked - queen diagonal"){
    Position p("8/8/8/8/4K3/8/8/7q");
    REQUIRE(p.isSquareAttacked(36, Color::w) == true);
}

TEST_CASE("isSquareAttacked - queen orthogonal"){
    Position p("8/8/8/8/q3K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == true);
}

TEST_CASE("isSquareAttacked - not attacked empty board"){
    Position p("8/8/8/8/4K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == false);
}

TEST_CASE("isSquareAttacked - rook on a-file attacks corner"){
    Position p("K7/8/8/8/8/8/8/r7");
    REQUIRE(p.isSquareAttacked(0, Color::w) == true);
}

TEST_CASE("isSquareAttacked - multiple attackers"){
    Position p("4r3/1b6/8/8/4K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == true);
}


TEST_CASE("Check resolution - knight check, king escapes only"){
    Position p("8/8/8/8/8/3n4/8/4K3");
    auto result = p.generateAllValidMovesForSide(Side::w);

    REQUIRE(result.attacksOnWhiteKing.size() == 1);
    std::vector<Move> kingMoves = getMovesAt(result, 60);
    REQUIRE(kingMoves.size() == 4);

    bool hasD1 = false, hasD2 = false, hasE2 = false, hasF1 = false;
    for(const auto& m : kingMoves){
        if(m.to == 59) hasD1 = true;
        if(m.to == 51) hasD2 = true;
        if(m.to == 52) hasE2 = true;
        if(m.to == 61) hasF1 = true;
    }
    REQUIRE(hasD1);
    REQUIRE(hasD2);
    REQUIRE(hasE2);
    REQUIRE(hasF1);
}


TEST_CASE("Check resolution - rook check, bishop blocks"){
    Position p("4r3/8/8/8/8/2B5/8/4K3");
    auto result = p.generateAllValidMovesForSide(Side::w);

    REQUIRE(result.attacksOnWhiteKing.size() == 1);

    std::vector<Move> bishopMoves = getMovesAt(result, 42);
    REQUIRE(bishopMoves.size() == 1);
    REQUIRE(bishopMoves[0].from == 42);
    REQUIRE(bishopMoves[0].to == 28);

    std::vector<Move> kingMoves = getMovesAt(result, 60);
    REQUIRE(kingMoves.size() == 4);
}


TEST_CASE("Check resolution - rook check, knight captures or blocks"){
    Position p("4r3/8/3N4/8/8/8/8/4K3");
    auto result = p.generateAllValidMovesForSide(Side::w);

    REQUIRE(result.attacksOnWhiteKing.size() == 1);

    std::vector<Move> knightMoves = getMovesAt(result, 19);
    REQUIRE(knightMoves.size() == 2);
    bool hasCapture = false, hasBlock = false;
    for(const auto& m : knightMoves){
        if(m.to == 4) hasCapture = true;
        if(m.to == 36) hasBlock = true;
    }
    REQUIRE(hasCapture);
    REQUIRE(hasBlock);

    std::vector<Move> kingMoves = getMovesAt(result, 60);
    REQUIRE(kingMoves.size() == 4);
}



TEST_CASE("Check resolution - double check, only king moves"){
    Position p("4r3/8/8/8/8/8/5b2/4K3");
    auto result = p.generateAllValidMovesForSide(Side::w);

    REQUIRE(result.attacksOnWhiteKing.size() == 2);

    std::vector<Move> kingMoves = getMovesAt(result, 60);
    REQUIRE(kingMoves.size() == 4);
}

TEST_CASE("Check resolution - king captures checking piece"){
    Position p("8/8/8/8/8/8/3b4/4K3");
    auto result = p.generateAllValidMovesForSide(Side::w);

    REQUIRE(result.attacksOnWhiteKing.size() == 1);

    std::vector<Move> kingMoves = getMovesAt(result, 60);
    REQUIRE(kingMoves.size() == 5);
}

TEST_CASE("Check resolution - X-ray, king can't retreat along attack ray"){
    Position p("8/8/8/8/r3K3/8/8/8");
    auto result = p.generateAllValidMovesForSide(Side::w);

    REQUIRE(result.attacksOnWhiteKing.size() == 1);

    std::vector<Move> kingMoves = getMovesAt(result, 36);
    REQUIRE(kingMoves.size() == 6);
}


TEST_CASE("Check resolution - rook check, rook blocks"){
    Position p("4r3/8/8/R7/8/8/8/4K3");
    auto result = p.generateAllValidMovesForSide(Side::w);

    REQUIRE(result.attacksOnWhiteKing.size() == 1);

    std::vector<Move> rookMoves = getMovesAt(result, 24);
    REQUIRE(rookMoves.size() == 1);
    REQUIRE(rookMoves[0].to == 28);

    std::vector<Move> kingMoves = getMovesAt(result, 60);
    REQUIRE(kingMoves.size() == 4);
}

TEST_CASE("Check resolution - pawn captures attacker"){
    Position p("8/8/8/4r3/3P4/8/8/4K3");
    auto result = p.generateAllValidMovesForSide(Side::w);

    REQUIRE(result.attacksOnWhiteKing.size() == 1);

    std::vector<Move> pawnMoves = getMovesAt(result, 35);
    REQUIRE(pawnMoves.size() == 1);
    REQUIRE(pawnMoves[0].to == 28);

    std::vector<Move> kingMoves = getMovesAt(result, 60);
    REQUIRE(kingMoves.size() == 4);
}


TEST_CASE("Check resolution - back rank checkmate, zero legal moves"){
    Position p("8/8/8/8/8/8/3PPP2/r2rK3");
    auto result = p.generateAllValidMovesForSide(Side::w);

    REQUIRE(result.attacksOnWhiteKing.size() >= 1);

    std::vector<Move> kingMoves = getMovesAt(result, 60);
    REQUIRE(kingMoves.size() == 0);

    std::vector<Move> pawnD2 = getMovesAt(result, 51);
    std::vector<Move> pawnE2 = getMovesAt(result, 52);
    std::vector<Move> pawnF2 = getMovesAt(result, 53);
    REQUIRE(pawnD2.size() == 0);
    REQUIRE(pawnE2.size() == 0);
    REQUIRE(pawnF2.size() == 0);
}

TEST_CASE("Check resolution - pinned piece cannot resolve checks"){
    Position p("rnb1k1nr/ppp2ppp/8/P3P3/1b2q2P/2N2P2/1PP2P2/R1BQKB1R w KQkq - 0 1");
    auto result = p.generateAllValidMovesForSide(Side::w);

    REQUIRE(result.attacksOnWhiteKing.size() == 1);

    std::vector<Move> kingMoves = getMovesAt(result, 60);
    REQUIRE(kingMoves.size() == 1);

    std::vector<Move> KnightC3 = getMovesAt(result, 42);
    std::vector<Move> PawnF3 = getMovesAt(result, 45);
    std::vector<Move> BishopC1 = getMovesAt(result, 58);
    std::vector<Move> BishopF1 = getMovesAt(result, 61);
    std::vector<Move> QueenD1 = getMovesAt(result, 60);
    REQUIRE(KnightC3.size() == 0);
    REQUIRE(PawnF3.size() == 1);
    REQUIRE(BishopC1.size() == 1);
    REQUIRE(BishopF1.size() == 1);
    REQUIRE(QueenD1.size() == 1);
}

TEST_CASE("Test Move Sequences"){
    Game game;

    // After Qh4, the f2 pawn is pinned diagonally (h4-g3-f2-e1)
    std::vector<std::string> testMoves = {"e2 e4", "e7 e5", "d2 d4", "d7 d5", "b1 c3", "d8 h4"};
    for(size_t i = 0; i < testMoves.size(); i++){
        bool result = game.makeMove(
            std::string(1, testMoves[i][0]) + testMoves[i][1],
            std::string(1, testMoves[i][3]) + testMoves[i][4]
        );
        REQUIRE(result);
    }

    // f2-f3 should be illegal since the pawn is pinned
    bool illegal = game.makeMove("f2", "f3");
    REQUIRE(!illegal);
}

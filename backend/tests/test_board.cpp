#include "catch.hpp"
#include "position.h"
#include "game.h"

TEST_CASE("Test number of Pawn moves") {
    Position p;
    REQUIRE(p.getWhitePieces().size() == 16);
    REQUIRE(p.getBlackPieces().size() == 16);

    Position onePawnLeft("8/P7/8/8/8/8/8/8");
    REQUIRE(onePawnLeft.getWhitePieces().size() == 1);
    REQUIRE(onePawnLeft.getBlackPieces().size() == 0);
    onePawnLeft.generateValidMoves(8);
    REQUIRE(onePawnLeft.getValidMoves().size() == 1);

    Position onePawnRight("8/8/8/7P/8/8/8/8");
    REQUIRE(onePawnRight.getWhitePieces().size() == 1);
    REQUIRE(onePawnRight.getBlackPieces().size() == 0);
    onePawnRight.generateValidMoves(31);
    REQUIRE(onePawnRight.getValidMoves().size() == 1);

    Position onePawnMid("8/8/8/3P4/8/8/8/8");
    REQUIRE(onePawnMid.getWhitePieces().size() == 1);
    REQUIRE(onePawnMid.getBlackPieces().size() == 0);
    onePawnMid.generateValidMoves(27);
    REQUIRE(onePawnMid.getValidMoves().size() == 1);

    Position oneTwoCaptures("8/8/2n1n3/3P4/8/8/8/8");
    REQUIRE(oneTwoCaptures.getWhitePieces().size() == 1);
    REQUIRE(oneTwoCaptures.getBlackPieces().size() == 2);
    oneTwoCaptures.generateValidMoves(27);
    REQUIRE(oneTwoCaptures.getValidMoves().size() == 3);

    Position oneTwoCapturesBlocked("8/8/2nrn3/3P4/8/8/8/8");
    REQUIRE(oneTwoCapturesBlocked.getWhitePieces().size() == 1);
    REQUIRE(oneTwoCapturesBlocked.getBlackPieces().size() == 3);
    oneTwoCapturesBlocked.generateValidMoves(27);
    REQUIRE(oneTwoCapturesBlocked.getValidMoves().size() == 2);

    Position oneTwoCapturesRank2("8/8/8/8/8/2n1n3/3P4/8");
    REQUIRE(oneTwoCapturesRank2.getWhitePieces().size() == 1);
    REQUIRE(oneTwoCapturesRank2.getBlackPieces().size() == 2);
    oneTwoCapturesRank2.generateValidMoves(51);
    REQUIRE(oneTwoCapturesRank2.getValidMoves().size() == 4);

    Position oneTwoCapturesRank2Blocked1("8/8/8/8/3p4/2n1n3/3P4/8");
    REQUIRE(oneTwoCapturesRank2Blocked1.getWhitePieces().size() == 1);
    REQUIRE(oneTwoCapturesRank2Blocked1.getBlackPieces().size() == 3);
    oneTwoCapturesRank2Blocked1.generateValidMoves(51);
    REQUIRE(oneTwoCapturesRank2Blocked1.getValidMoves().size() == 3);

    Position oneTwoCapturesRank2Blocked2("8/8/8/8/8/2npn3/3P4/8");
    REQUIRE(oneTwoCapturesRank2Blocked2.getWhitePieces().size() == 1);
    REQUIRE(oneTwoCapturesRank2Blocked2.getBlackPieces().size() == 3);
    oneTwoCapturesRank2Blocked2.generateValidMoves(51);
    REQUIRE(oneTwoCapturesRank2Blocked2.getValidMoves().size() == 2);

}

TEST_CASE("Test number of bishop moves"){
    Position oneMid("8/8/8/3B4/8/8/8/8");
    REQUIRE(oneMid.getWhitePieces().size() == 1);
    REQUIRE(oneMid.getBlackPieces().size() == 0);
    oneMid.generateValidMoves(27);
    REQUIRE(oneMid.getValidMoves().size() == 13);

    Position oneBottomLeft("8/8/8/8/8/8/8/b7");
    REQUIRE(oneBottomLeft.getWhitePieces().size() == 0);
    REQUIRE(oneBottomLeft.getBlackPieces().size() == 1);
    oneBottomLeft.generateValidMoves(56);
    REQUIRE(oneBottomLeft.getValidMoves().size() == 7);

    Position oneTopLeft("b7/8/8/8/8/8/8/8");
    REQUIRE(oneTopLeft.getWhitePieces().size() == 0);
    REQUIRE(oneTopLeft.getBlackPieces().size() == 1);
    oneTopLeft.generateValidMoves(0);
    REQUIRE(oneTopLeft.getValidMoves().size() == 7);

    Position middleWithCaptures("8/6p1/3n4/4B3/8/8/1P6/8");
    REQUIRE(middleWithCaptures.getWhitePieces().size() == 2);
    REQUIRE(middleWithCaptures.getBlackPieces().size() == 2);
    middleWithCaptures.generateValidMoves(28);
    REQUIRE(middleWithCaptures.getValidMoves().size() == 8);

    Position middleWithCaptures1("1n5r/8/8/4B3/8/8/7P/8");
    REQUIRE(middleWithCaptures1.getWhitePieces().size() == 2);
    REQUIRE(middleWithCaptures1.getBlackPieces().size() == 2);
    middleWithCaptures1.generateValidMoves(28);
    REQUIRE(middleWithCaptures1.getValidMoves().size() == 12);

}



TEST_CASE("Test number of queen moves"){
    Position oneMid("8/8/8/3Q4/8/8/8/8");
    REQUIRE(oneMid.getWhitePieces().size() == 1);
    REQUIRE(oneMid.getBlackPieces().size() == 0);
    oneMid.generateValidMoves(27);
    REQUIRE(oneMid.getValidMoves().size() == 27);

    Position surroundedByEnemies("8/8/8/2prp3/2bQb3/2prp3/8/8");
    REQUIRE(surroundedByEnemies.getWhitePieces().size() == 1);
    REQUIRE(surroundedByEnemies.getBlackPieces().size() == 8);
    surroundedByEnemies.generateValidMoves(35);
    REQUIRE(surroundedByEnemies.getValidMovesForPieceAt(35).size() == 8);

    Position surroundedByFriends("8/8/8/2prp3/2bqb3/2prp3/8/8");
    REQUIRE(surroundedByFriends.getWhitePieces().size() == 0);
    REQUIRE(surroundedByFriends.getBlackPieces().size() == 9);
    surroundedByFriends.generateValidMoves(35);
    REQUIRE(surroundedByFriends.getValidMovesForPieceAt(35).size() == 0);

    Position multipleCapturesBlocks("8/b2R4/8/4p3/N2qR3/8/8/3r4");
    REQUIRE(multipleCapturesBlocks.getWhitePieces().size() == 3);
    REQUIRE(multipleCapturesBlocks.getBlackPieces().size() == 4);
    multipleCapturesBlocks.generateValidMoves(35);
    REQUIRE(multipleCapturesBlocks.getValidMovesForPieceAt(35).size() == 17);
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
        testPosition.generateValidMoves(squares[i]);
        REQUIRE(testPosition.getValidMoves().size() == test.second);
        i++;
    }
}


TEST_CASE("Test number of king moves"){
    std::pair<std::string, int> testInputs[] = {
        std::make_pair("8/8/8/3K4/8/8/8/8", 8), 
        std::make_pair("8/8/8/8/8/8/8/K7", 3),
        std::make_pair("7K/8/8/8/8/8/8/8", 3),
        std::make_pair("8/8/3qbP2/3rKP2/3rPb2/8/8/8", 5),
    };

    int squares[] = {27, 56, 7, 28};

    int i = 0;
    for(auto& test : testInputs){
        Position testPosition(test.first);
        testPosition.generateValidMoves(squares[i]);
        REQUIRE(testPosition.getValidMoves().size() == test.second);
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
        testPosition.generateValidMoves(squares[i]);
        REQUIRE(testPosition.getValidMovesForPieceAt(squares[i]).size() == test.second);
        i++;
    }
}

TEST_CASE("test number of moves for all pieces in a positon"){
    Position p;
    REQUIRE(p.getWhitePieces().size() == 16);
    REQUIRE(p.getBlackPieces().size() == 16);
    Side side = Side::w;
    p.generateAllValidMovesForSide(side);
    REQUIRE(p.getValidMoves().size() == 20);
}

TEST_CASE("test check detection"){
    Position p;
    REQUIRE(p.getWhitePieces().size() == 16);
    REQUIRE(p.getBlackPieces().size() == 16);
    Side side = Side::w;
    p.generateAllValidMovesForSide(side);
    REQUIRE(p.getValidMoves().size() == 20);
    REQUIRE(p.getPiecesAttackingBlack().size() == 0);
    REQUIRE(p.getPiecesAttackingWhite().size() == 0);

    Position p1("8/8/8/8/8/3n1N2/8/4K3");
    side = Side::b;
    p1.generateAllValidMovesForSide(side);
    REQUIRE(p1.getValidMoves().size() == 8);
    REQUIRE(p1.getPiecesAttackingBlack().size() == 0);
    REQUIRE(p1.getPiecesAttackingWhite().size() == 1);

    Position p2("8/8/8/8/5p1p/6K1/8/8");
    side = Side::b;
    p2.generateAllValidMovesForSide(side);
    side = Side::w;
    p2.generateAllValidMovesForSide(side);
    //REQUIRE(p2.getValidMoves().size() == 8);
    REQUIRE(p2.getPiecesAttackingBlack().size() == 0);
    REQUIRE(p2.getPiecesAttackingWhite().size() == 2);

    Position p3("3B4/1N6/8/k2R1n2/1P2p1p1/QN4K1/8/8 b - - 0 1");
    side = Side::b;
    p3.generateAllValidMovesForSide(side);
    side = Side::w;
    p3.generateAllValidMovesForSide(side);
    //REQUIRE(p2.getValidMoves().size() == 8);
    //REQUIRE(p3.getPiecesAttackingBlack().size() == 5);
    //REQUIRE(p3.getPiecesAttackingWhite().size() == 6);
}

TEST_CASE("Test Pinned Piece Detection"){

    Piece pawn(PieceType::P, Color::w);
    Piece& p = pawn;
    Piece knight(PieceType::N, Color::w);
    Piece& n = knight;
    std::vector<std::vector<Pin>> pins = {
        {Pin(28, 12, 60, p, PinType::Orthoganal)},
        {Pin(28, 12, 60, p, PinType::Orthoganal), Pin(42, 33, 60, n, PinType::Diagonal)},
        {},
        {
            Pin(51, 48, 52, p, PinType::Orthoganal), Pin(43, 25, 52, p, PinType::Diagonal), Pin(44, 4, 52, p, PinType::Orthoganal),
            Pin(45, 31, 52, p, PinType::Diagonal), Pin(53, 55, 52, p, PinType::Orthoganal)
        },
        {Pin(53, 39, 60, p, PinType::Diagonal)}
        //pinned: 51, 43, 44, 45,  53
        //from: 48, 25, 4, 31, 55
        //to: 52
    };

    std::vector<std::string> boards = {
        "rnb1k1n1/ppp1qppp/8/4P3/3P4/2P2N2/P1P2PPP/R1BQKB1R w KQkq - 0 1",
        "rnb1k1n1/ppp1qppp/8/4P3/1b1P4/2N2N2/PPP2PPP/R1BQKB1R w KQkq",
        "rnb1k1n1/ppp1qppp/8/4P3/1b1P4/2N2N2/PPPBBPPP/R2QK2R b KQkq - 0 1",
        "4r3/8/8/1b5b/3n4/3PPP2/q2PKP1r/8 b - - 0 1",
        "rnb1kbnr/ppp2ppp/8/3pp3/3PP2q/8/PPP2PPP/RNBQKBNR w KQkq - 0 1"
    };


    REQUIRE(boards.size() == pins.size());

    std::cout << " boards size = " << boards.size() << std::endl;

    for(size_t i = 0; i < boards.size(); i++){
        Position testPosition(boards[i]);
        testPosition.generateAllValidMovesForSide(Side::b);
        testPosition.generateAllValidMovesForSide(Side::w);
        std::vector<Pin> detectedPins = testPosition.getPins();
        REQUIRE(detectedPins.size() == pins[i].size());
        int count = 0;
        for(size_t j = 0; j < detectedPins.size(); j++){
            for(int k = 0; k < pins[i].size(); k++){

                if(pins[i].size() > 0 && detectedPins[j] == pins[i][k]){
                    count++;
                }
            }
        }
        REQUIRE(count == pins[i].size());
    }
}

TEST_CASE("Test Pinned piece movement generation"){

    // Test 1: Bishop pinned diagonally - can only move along pin ray toward attacker
    Position bishopPinnedDiagonal("8/8/5q2/4B3/3K4/8/8/8");
    // White bishop at e5 (square 20), white king at d4 (27), black queen at f6 (13)
    // Bishop is pinned on the d4-e5-f6 diagonal
    // Should only be able to capture the queen at f6
    bishopPinnedDiagonal.generateAllValidMovesForSide(Side::b);
    bishopPinnedDiagonal.generateAllValidMovesForSide(Side::w);
    REQUIRE(bishopPinnedDiagonal.getPins().size() == 1);
    std::vector<Move> bishopMoves = bishopPinnedDiagonal.getValidMovesForPieceAt(28);
    REQUIRE(bishopMoves.size() == 1);
    REQUIRE(bishopMoves[0].to == 21); // Can only move to f6

    // Test 2: Bishop pinned orthogonally - cannot move at all
    Position bishopPinnedOrthogonal("8/8/8/1r1BK3/8/8/8/8 w - - 0 1");
    // White bishop at d5 (square 27), white king at e5 (28), black rook at d1 (59)
    // Bishop is pinned orthogonally along the d-file
    // Bishops can only move diagonally, so a bishop pinned orthogonally has no legal moves
    bishopPinnedOrthogonal.generateAllValidMovesForSide(Side::b);
    bishopPinnedOrthogonal.generateAllValidMovesForSide(Side::w);
    std::vector<Move> bishopPinnedOrtho = bishopPinnedOrthogonal.getValidMovesForPieceAt(27);
    REQUIRE(bishopPinnedOrtho.size() == 0);

    // Test 3: Queen pinned diagonally - can move multiple squares along pin ray
    Position queenPinnedDiagonal("8/6K1/8/8/3Q4/8/8/q7 w - - 0 1");
    // White queen at c3 (square 42), white king at d2 (51), black queen at a1 (56)
    // Queen is pinned on the a1-c3-d2 diagonal (direction +7/-7)
    // Should be able to move to b2 (49) and a1 (56, capturing the attacker)
    queenPinnedDiagonal.generateAllValidMovesForSide(Side::b);
    queenPinnedDiagonal.generateAllValidMovesForSide(Side::w);
    std::vector<Move> queenMovesB = queenPinnedDiagonal.getValidMovesForPieceAt(56);
    std::vector<Move> queenMoves = queenPinnedDiagonal.getValidMovesForPieceAt(35);
    REQUIRE(queenMovesB.size() == 17);
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
    // Black bishop b7(9) attacks e4(36) along diagonal
    Position p("8/1b6/8/8/4K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == true);
}

TEST_CASE("isSquareAttacked - diagonal blocked by piece"){
    // Black bishop b7(9), white pawn c6(18) blocks diagonal to e4(36)
    Position p("8/1b6/2P5/8/4K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == false);
}

TEST_CASE("isSquareAttacked - orthogonal attack by rook"){
    // Black rook e8(4) attacks e4(36) along e-file
    Position p("4r3/8/8/8/4K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == true);
}

TEST_CASE("isSquareAttacked - orthogonal blocked by piece"){
    // Black rook e8(4), white pawn e5(28) blocks e-file to e4(36)
    Position p("4r3/8/8/4P3/4K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == false);
}

TEST_CASE("isSquareAttacked - knight attack"){
    // Black knight f6(21) attacks e4(36) via knight move
    Position p("8/8/5n2/8/4K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == true);
}

TEST_CASE("isSquareAttacked - knight not attacking"){
    // Black knight a8(0) does NOT attack e4(36)
    Position p("n7/8/8/8/4K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == false);
}

TEST_CASE("isSquareAttacked - black pawn attacks white piece"){
    // Black pawn d5(27) attacks e4(36) diagonally downward
    Position p("8/8/8/3p4/4K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == true);
}

TEST_CASE("isSquareAttacked - white pawn attacks black piece"){
    // White pawn d4(35) attacks e5(28) diagonally upward
    Position p("8/8/8/4k3/3P4/8/8/8");
    REQUIRE(p.isSquareAttacked(28, Color::b) == true);
}

TEST_CASE("isSquareAttacked - pawn does not attack forward"){
    // Black pawn e5(28) is directly above e4(36) but pawns don't attack forward
    Position p("8/8/8/4p3/4K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == false);
}

TEST_CASE("isSquareAttacked - queen diagonal"){
    // Black queen h1(63) attacks e4(36) diagonally
    Position p("8/8/8/8/4K3/8/8/7q");
    REQUIRE(p.isSquareAttacked(36, Color::w) == true);
}

TEST_CASE("isSquareAttacked - queen orthogonal"){
    // Black queen a4(32) attacks e4(36) along rank 4
    Position p("8/8/8/8/q3K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == true);
}

TEST_CASE("isSquareAttacked - not attacked empty board"){
    // Only a white king, nothing attacking it
    Position p("8/8/8/8/4K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == false);
}

TEST_CASE("isSquareAttacked - rook on a-file attacks corner"){
    // Black rook a1(56) attacks a8(0) along a-file
    Position p("K7/8/8/8/8/8/8/r7");
    REQUIRE(p.isSquareAttacked(0, Color::w) == true);
}

TEST_CASE("isSquareAttacked - multiple attackers"){
    // Black rook e8(4) and black bishop b7(9) both attack e4(36)
    Position p("4r3/1b6/8/8/4K3/8/8/8");
    REQUIRE(p.isSquareAttacked(36, Color::w) == true);
}


TEST_CASE("Check resolution - knight check, king escapes only"){
    // Black knight d3(43) checks white king e1(60)
    // Knight attacks: c1(58), e1(60), b2(49), f2(53), b4(33), f4(37), c5(26), e5(28)
    // King escapes: d1(59)ok, d2(51)ok, e2(52)ok, f1(61)ok, f2(53)attacked
    // No other white pieces to block or capture
    Position p("8/8/8/8/8/3n4/8/4K3");
    p.generateAllValidMovesForSide(Side::b);
    p.generateAllValidMovesForSide(Side::w);

    REQUIRE(p.getPiecesAttackingWhite().size() == 1);
    std::vector<Move> kingMoves = p.getValidMovesForPieceAt(60);
    REQUIRE(kingMoves.size() == 4);

    // Verify specific escape squares
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
    // Black rook e8(4) checks white king e1(60) along e-file
    // White bishop c3(42) can block at e5(28) via the -7 diagonal
    // King escapes: d1(59)ok, d2(51)ok, f1(61)ok, f2(53)ok, e2(52)attacked
    Position p("4r3/8/8/8/8/2B5/8/4K3");
    p.generateAllValidMovesForSide(Side::b);
    p.generateAllValidMovesForSide(Side::w);

    REQUIRE(p.getPiecesAttackingWhite().size() == 1);

    std::vector<Move> bishopMoves = p.getValidMovesForPieceAt(42);
    REQUIRE(bishopMoves.size() == 1);
    REQUIRE(bishopMoves[0].from == 42);
    REQUIRE(bishopMoves[0].to == 28); // e5 blocks the e-file

    std::vector<Move> kingMoves = p.getValidMovesForPieceAt(60);
    REQUIRE(kingMoves.size() == 4);
}


TEST_CASE("Check resolution - rook check, knight captures or blocks"){
    // Black rook e8(4) checks white king e1(60) along e-file
    // White knight d6(19) can:
    //   - Capture rook at e8(4): 19-15=4 (valid knight move)
    //   - Block at e4(36): 19+17=36 (valid knight move, on e-file between e8 and e1)
    // King escapes: d1(59)ok, d2(51)ok, f1(61)ok, f2(53)ok, e2(52)attacked
    Position p("4r3/8/3N4/8/8/8/8/4K3");
    p.generateAllValidMovesForSide(Side::b);
    p.generateAllValidMovesForSide(Side::w);

    REQUIRE(p.getPiecesAttackingWhite().size() == 1);

    std::vector<Move> knightMoves = p.getValidMovesForPieceAt(19);
    REQUIRE(knightMoves.size() == 2);
    bool hasCapture = false, hasBlock = false;
    for(const auto& m : knightMoves){
        if(m.to == 4) hasCapture = true;   // capture rook
        if(m.to == 36) hasBlock = true;     // block at e4
    }
    REQUIRE(hasCapture);
    REQUIRE(hasBlock);

    std::vector<Move> kingMoves = p.getValidMovesForPieceAt(60);
    REQUIRE(kingMoves.size() == 4);
}



TEST_CASE("Check resolution - double check, only king moves"){
    // Black rook e8(4) checks along e-file
    // Black bishop f2(53) checks via diagonal (53+7=60)
    // Double check: only king can move
    // King e1(60) neighbors: d1(59), d2(51), e2(52), f1(61), f2(53)
    //   d1(59): safe (not on bishop diags or e-file)
    //   d2(51): safe
    //   e2(52): attacked by rook (e-file)
    //   f1(61): safe
    //   f2(53): bishop there, king can capture (no other attacker on f2)
    Position p("4r3/8/8/8/8/8/5b2/4K3");
    p.generateAllValidMovesForSide(Side::b);
    p.generateAllValidMovesForSide(Side::w);

    REQUIRE(p.getPiecesAttackingWhite().size() == 2);

    std::vector<Move> kingMoves = p.getValidMovesForPieceAt(60);
    // Correct: d1, d2, f1, f2(capture) = 4
    // NOTE: kingEscapeLambda only handles empty squares currently,
    // so this will fail until king captures are implemented.
    REQUIRE(kingMoves.size() == 4);
}

TEST_CASE("Check resolution - king captures checking piece"){
    // Black bishop d2(51) checks white king e1(60) via diagonal (51+9=60)
    // No other black pieces, no X-ray issues
    // King e1(60) neighbors: d1(59), d2(51), e2(52), f1(61), f2(53)
    //   All safe including d2 capture (bishop only attacks c1,e1,c3,e3)
    Position p("8/8/8/8/8/8/3b4/4K3");
    p.generateAllValidMovesForSide(Side::b);
    p.generateAllValidMovesForSide(Side::w);

    REQUIRE(p.getPiecesAttackingWhite().size() == 1);

    std::vector<Move> kingMoves = p.getValidMovesForPieceAt(60);
    // Correct: d1, d2(capture), e2, f1, f2 = 5
    // NOTE: will fail until kingEscapeLambda handles captures
    REQUIRE(kingMoves.size() == 5);
}

TEST_CASE("Check resolution - X-ray, king can't retreat along attack ray"){
    // Black rook a4(32) checks white king e4(36) along rank 4
    // King e4(36) neighbors: d3(43), d4(35), d5(27), e3(44), e5(28), f3(45), f4(37), f5(29)
    //   d4(35): rank 4, attacked by rook
    //   f4(37): rank 4, rook sees through king with X-ray -> attacked
    //   All other squares: safe
    // Correct: d3, d5, e3, e5, f3, f5 = 6
    Position p("8/8/8/8/r3K3/8/8/8");
    p.generateAllValidMovesForSide(Side::b);
    p.generateAllValidMovesForSide(Side::w);

    REQUIRE(p.getPiecesAttackingWhite().size() == 1);

    std::vector<Move> kingMoves = p.getValidMovesForPieceAt(36);
    // NOTE: will fail (returns 7) until isSquareAttacked supports skipSquare
    REQUIRE(kingMoves.size() == 6);
}


TEST_CASE("Check resolution - rook check, rook blocks"){
    // Black rook e8(4) checks white king e1(60) along e-file
    // White rook a5(24) can block at e5(28) by moving along rank 5
    // King escapes: d1(59), d2(51), f1(61), f2(53) = 4
    Position p("4r3/8/8/R7/8/8/8/4K3");
    p.generateAllValidMovesForSide(Side::b);
    p.generateAllValidMovesForSide(Side::w);

    REQUIRE(p.getPiecesAttackingWhite().size() == 1);

    std::vector<Move> rookMoves = p.getValidMovesForPieceAt(24);
    REQUIRE(rookMoves.size() == 1);
    REQUIRE(rookMoves[0].to == 28); // e5 blocks the e-file

    std::vector<Move> kingMoves = p.getValidMovesForPieceAt(60);
    REQUIRE(kingMoves.size() == 4);
}

TEST_CASE("Check resolution - pawn captures attacker"){
    // Black rook e5(28) checks white king e1(60) along e-file
    // White pawn d4(35) can capture rook at e5: captureRight = 35-7 = 28
    // King escapes: d1(59), d2(51), f1(61), f2(53) = 4 (e2 on e-file)
    Position p("8/8/8/4r3/3P4/8/8/4K3");
    p.generateAllValidMovesForSide(Side::b);
    p.generateAllValidMovesForSide(Side::w);

    REQUIRE(p.getPiecesAttackingWhite().size() == 1);

    std::vector<Move> pawnMoves = p.getValidMovesForPieceAt(35);
    REQUIRE(pawnMoves.size() == 1);
    REQUIRE(pawnMoves[0].to == 28); // capture rook at e5

    std::vector<Move> kingMoves = p.getValidMovesForPieceAt(60);
    REQUIRE(kingMoves.size() == 4);
}


TEST_CASE("Check resolution - back rank checkmate, zero legal moves"){
    // Black rook d1(59) checks white king e1(60) along rank 1
    // Black rook a1(56) defends d1 along rank 1
    // White pawns at d2(51), e2(52), f2(53) block escape upward
    // f1(61) attacked by rooks along rank 1
    // d1(59) defended by a1 rook, can't safely capture
    // = checkmate, 0 legal moves
    Position p("8/8/8/8/8/8/3PPP2/r2rK3");
    p.generateAllValidMovesForSide(Side::b);
    p.generateAllValidMovesForSide(Side::w);

    REQUIRE(p.getPiecesAttackingWhite().size() >= 1);

    std::vector<Move> kingMoves = p.getValidMovesForPieceAt(60);
    REQUIRE(kingMoves.size() == 0);

    // Pawns can't resolve either
    std::vector<Move> pawnD2 = p.getValidMovesForPieceAt(51);
    std::vector<Move> pawnE2 = p.getValidMovesForPieceAt(52);
    std::vector<Move> pawnF2 = p.getValidMovesForPieceAt(53);
    REQUIRE(pawnD2.size() == 0);
    REQUIRE(pawnE2.size() == 0);
    REQUIRE(pawnF2.size() == 0);
}

TEST_CASE("Check resolution - pinned piece cannot resolve checks"){
    // Black rook d1(59) checks white king e1(60) along rank 1
    // Black rook a1(56) defends d1 along rank 1
    // White pawns at d2(51), e2(52), f2(53) block escape upward
    // f1(61) attacked by rooks along rank 1
    // d1(59) defended by a1 rook, can't safely capture
    // = checkmate, 0 legal moves
    Position p("rnb1k1nr/ppp2ppp/8/P3P3/1b2q2P/2N2P2/1PP2P2/R1BQKB1R w KQkq - 0 1");
    p.generateAllValidMovesForSide(Side::b);
    p.generateAllValidMovesForSide(Side::w);

    REQUIRE(p.getPiecesAttackingWhite().size() == 1);

    std::vector<Move> kingMoves = p.getValidMovesForPieceAt(60);
    REQUIRE(kingMoves.size() == 1);

    // Pawns can't resolve either
    std::vector<Move> KnightC3 = p.getValidMovesForPieceAt(42);
    std::vector<Move> PawnF3 = p.getValidMovesForPieceAt(45);
    std::vector<Move> BishopC1 = p.getValidMovesForPieceAt(58);
    std::vector<Move> BishopF1 = p.getValidMovesForPieceAt(61);
    std::vector<Move> QueenD1 = p.getValidMovesForPieceAt(60);
    REQUIRE(KnightC3.size() == 0);
    REQUIRE(PawnF3.size() == 1);
    REQUIRE(BishopC1.size() == 1);
    REQUIRE(BishopF1.size() == 1);
    REQUIRE(QueenD1.size() == 1);
}

TEST_CASE("Test Move Sequences"){
    Game game;
    Position p = game.getPosition();
    Piece pawn(PieceType::P, Color::w);
    Piece& pn = pawn;

    // After Qh4, the f2 pawn is pinned diagonally (h4-g3-f2-e1)
    std::vector<std::string> testMoves = {"e2 e4", "e7 e5", "d2 d4", "d7 d5", "b1 c3", "d8 h4"};
    for(auto i = 0; i < testMoves.size(); i++){
        bool result = p.makeMove(game.moveStringToMove(testMoves[i]));
        REQUIRE(result);
    }
    // After Qh4, f2 pawn should be pinned by queen on h4 to king on e1
    REQUIRE(!p.getPins().empty());
    // f2-f3 should be illegal since the pawn is pinned and f3 is off the pin line
    bool illegal = p.makeMove(game.moveStringToMove("f2 f3"));
    REQUIRE(!illegal);
    //game.testGameWithInput(testMoves, );
}

/*bool Position::testDiagonalIntersects(){
    bool t1 = squareIntersectsDiagonal(0, 3, 7);
    bool t2 = squareIntersectsDiagonal(10, 26, 50);
    bool t3 = squareIntersectsDiagonal(42, 36, 22);
    bool t4 = squareIntersectsDiagonal(60, 60, 24);

    bool t5 = squareIntersectsDiagonal(8, 17, 26);
    bool t6 = squareIntersectsDiagonal(59, 45, 31);
    bool t7 = squareIntersectsDiagonal(60, 51, 24);

    return !t1 && !t2 && !t3 && !t4 && t5 && t6 && t7;
}

bool Position::testOrthoganalIntersects(){
    bool t1 = squareIntersectsOrthoganal(0, 9, 15);
    bool t2 = squareIntersectsOrthoganal(0, 5, 39);
    bool t3 = squareIntersectsOrthoganal(13, 36, 61);

    bool t4 = squareIntersectsOrthoganal(42, 53, 47);
    bool t5 = squareIntersectsOrthoganal(0, 3, 7);
    bool t6 = squareIntersectsOrthoganal(2, 10, 26);
    bool t7 = squareIntersectsOrthoganal(55, 52, 49);
    bool t8 = squareIntersectsOrthoganal(49, 41, 33);

    std::cout << "gave test t1 a result of " << t1 << std::endl;
    std::cout << "gave test t2 a result of " << t3 << std::endl;
    std::cout << "gave test t3 a result of " << t4 << std::endl;
    std::cout << "gave test t4 a result of " << t5 << std::endl;
    std::cout << "gave test t5 a result of " << t6 << std::endl;
    std::cout << "gave test t6 a result of " << t7 << std::endl;
    std::cout << "gave test t7 a result of " << t8 << std::endl;
    
    return !t1 && !t2 && !t3 && !t4 && t5 && t6 && t7 && t8;
}*/



/*
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
} */

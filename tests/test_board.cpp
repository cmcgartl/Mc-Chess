#include "../third_party/catch.hpp"
#include "../src/position.h"

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

    Position p3("3B4/1N6/8/k2R1n2/1P3p1p/QN4K1/4n2q/6r1");
    side = Side::b;
    p3.generateAllValidMovesForSide(side);
    side = Side::w;
    p3.generateAllValidMovesForSide(side);
    //REQUIRE(p2.getValidMoves().size() == 8);
    REQUIRE(p3.getPiecesAttackingBlack().size() == 6);
    REQUIRE(p3.getPiecesAttackingWhite().size() == 6);
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
        }
        //pinned: 51, 43, 44, 45,  53
        //from: 48, 25, 4, 31, 55
        //to: 52
    };

    std::vector<std::string> boards = {
        "rnb1k1n1/ppp1qppp/8/4P3/3P4/2P2N2/P1P2PPP/R1BQKB1R w KQkq - 0 1",
        "rnb1k1n1/ppp1qppp/8/4P3/1b1P4/2N2N2/PPP2PPP/R1BQKB1R w KQkq",
        "rnb1k1n1/ppp1qppp/8/4P3/1b1P4/2N2N2/PPPBBPPP/R2QK2R b KQkq - 0 1",
        "4r3/8/8/1b5b/3n4/3PPP2/q2PKP1r/8 b - - 0 1"
    };


    REQUIRE(boards.size() == pins.size());

    std::cout << " boards size = " << boards.size() << std::endl;

    for(size_t i = 0; i < boards.size(); i++){
        Position testPosition(boards[i]);
        testPosition.generateAllValidMovesForSide(Side::w);
        testPosition.generateAllValidMovesForSide(Side::b);
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

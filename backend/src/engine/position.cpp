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

DirectionSet getDirections(PieceType type, Color color) {
    switch(type) {
        case PieceType::B: return {DiagonalDirections, 4, false};
        case PieceType::R: return {OrthoganalDirections, 4, false};
        case PieceType::Q: return {DiagonalDirections, 4, false}; // need both, see below
        case PieceType::N: return {KnightMoves, 8, true};
        case PieceType::P: return color == Color::w 
            ? DirectionSet{pawnAttackerDirectionsWhite, 2, true}
            : DirectionSet{pawnAttackerDirectionsBlack, 2, true};
        default: return {nullptr, 0, false};
    }
}

bool squareInvalid(int square){
    if(square < 0 || square > 63){
        return true;
    }
    return false;
}

bool squareIntersectsDiagonal(int start, int square, int end){
    if(squareInvalid(start) || squareInvalid(end) || squareInvalid(square)){
        throw std::out_of_range("Position::squareIntersectsOrthoganal: provided square is out of range");
    }
    if(start == square || end == square || start == end){
        return false;
    }

    //start/end must form a diagonal: checks if the difference in rank = the difference in file
    if(std::abs((start / 8) - (end / 8)) != std::abs((start % 8) - (end % 8))){
        return false;
    }

    //determine the diagonal direction
    int direction = 0;
    bool valid = false;
    if((start - end) % 7 == 0){
        if(start - end > 0){
            direction = DiagonalDirections[3];
        }
        else{
            direction = DiagonalDirections[0];
        }
        valid = true;
    }
    if((start - end) % 9 == 0){
        if(start - end > 0){
            direction = DiagonalDirections[1];
        }
        else{
            direction = DiagonalDirections[2];
        }
        valid = true;
    }

    //if provided start/end is not a valid diagonal, stop
    if(!valid){
        return false;
    }

    //traverse diagonal until we see target square
    int curr = start;
    while(curr != end){
        curr += direction;
        if(curr == square){
            return true;
        }
    }

    return false;
}


bool squareIntersectsOrthoganal(int start, int square, int end){
    if(squareInvalid(start) || squareInvalid(end) || squareInvalid(square)){
        throw std::out_of_range("Position::squareIntersectsOrthoganal: provided square is out of range");
    }

    if(start == square || end == square || start == end){
       return false;
    };

    //if target square is not withing start - end, return false
    if(!((start < square && end > square) || (end < square && start > square))) return false;

    //if we are checking intersection of a horizontal line (same rank)
    if(start / 8 == end / 8){
        return true;
    }

    //if we are checking intersection of a vertical line (same file)
    if((start - end) % 8 == 0){

        //return true if square is along this file, false o.w.
        return !((start - square) % 8);
    }

    return false;
}

void Position::generatePieceLists(){
    whitePieces.clear();
    blackPieces.clear();
    for(int i = 0; i < 64; i++){
        Piece p = board.at(i);
        if(p.type == PieceType::None) continue;
        if(p.color == Color::w){
            whitePieces.push_back(p);
        }
        else{
            blackPieces.push_back(p);
        }
    }
}

void Position::getValidMovesPawn(int& count, int square, std::vector<int>& pinDirections){
    if(square < 0 || square >= 64){
        throw std::out_of_range("Position::getValidMovesPawn: provided square is out of range");
    }
    Piece p = board.at(square);
    if(p.type != PieceType::P){
        throw std::logic_error("Position::getValidMovesPawn: generating moves for incorrect piece at square" + std::to_string(square));
    }

    auto dirAllowed = [&pinDirections](int dir){
        if(pinDirections.empty()) return true;
        for(int pd : pinDirections){
            if(pd == dir || pd == -dir) return true;
        }
        return false;
    };

    if(p.color == Color::w){
        int oneForward = square - 8;
        int twoForward = square - 16;
        int captureLeft = (square - 8) - 1;
        int captureRight = (square - 8) + 1;
        if(dirAllowed(-8) && oneForward >= 0){
            if(board.at(oneForward).type == PieceType::None){
                possibleMoves.push_back(Move(square, oneForward));
                count++;

                if(square >= 48 && square <= 55){
                    if(board.at(twoForward).type == PieceType::None){
                        possibleMoves.push_back(Move(square, twoForward));
                        count++;
                    }
                }
            }
        }
        if(dirAllowed(-9) && captureLeft >= 0 && ((square % 8) != 0)){
            if(board.at(captureLeft).color == Color::b){
                possibleMoves.push_back(Move(square, captureLeft));
                if(board.at(captureLeft).type == PieceType::K){
                    p.color == Color::w ?
                        squaresAttackingBlackKing.push_back(square) :
                        squaresAttackingWhiteKing.push_back(square);
                }
                count++;
            }
        }
        if(dirAllowed(-7) && captureRight >= 0 && ((square + 1) % 8 != 0)){
            if(board.at(captureRight).color == Color::b){
                possibleMoves.push_back(Move(square, captureRight));
                if(board.at(captureRight).type == PieceType::K){
                    p.color == Color::w ?
                        squaresAttackingBlackKing.push_back(square) :
                        squaresAttackingWhiteKing.push_back(square);
                }
                count++;
            }
        }
    }
    if(p.color == Color::b){
        int oneForward = square + 8;
        int twoForward = square + 16;
        int captureLeft = square + 8 + 1;
        int captureRight = square + 8 - 1;
        if(dirAllowed(8) && oneForward <= 63){
            if(board.at(oneForward).type == PieceType::None){
                possibleMoves.push_back(Move(square, oneForward));
                count++;

                if(square >= 8 && square <= 15){
                    if(board.at(twoForward).type == PieceType::None){
                        possibleMoves.push_back(Move(square, square + 16));
                        count++;
                    }
                }
            }
        }
        if(dirAllowed(7) && captureRight <= 63 && (square % 8 != 0)){
            if(board.at(captureRight).color == Color::w){
                possibleMoves.push_back(Move(square, captureRight));
                if(board.at(captureRight).type == PieceType::K){
                    p.color == Color::w ?
                        squaresAttackingBlackKing.push_back(square) :
                        squaresAttackingWhiteKing.push_back(square);
                }
                count++;
            }
        }
        if(dirAllowed(9) && captureLeft <= 63 && ((square + 1) % 8 != 0)){
            if(board.at(captureLeft).color == Color::w){
                possibleMoves.push_back(Move(square, captureLeft));
                if(board.at(captureLeft).type == PieceType::K){
                    p.color == Color::w ?
                        squaresAttackingBlackKing.push_back(square) :
                        squaresAttackingWhiteKing.push_back(square);
                }
                count++;
            }
        }
    }
}

void Position::generateDiagonalMoves(int& count, int square, bool cap, Color color, std::vector<int>& pinDirections){
    Piece p = board.at(square);
    for(int i = 0; i < 4; i++){
        int nextSquare = square;
        bool checkingForPin = false;
        int possiblePinSquare = -1;
        while(true){
            if(nextSquare % 8 == 0 && i <= 1){
                break;
            }
            if((nextSquare + 1) % 8 == 0 && i >= 2){
                break;
            }
            nextSquare+= DiagonalDirections[i];
            
            //determine if the current direction is a restricted direction due to a pin
            if(p.pinnedD){
                bool restrictedDirection = true;
                for(size_t j = 0; j < pinDirections.size(); j++){
                    if(DiagonalDirections[i] == pinDirections[j] ||DiagonalDirections[i] == -pinDirections[j]){
                        restrictedDirection = false;
                    }
                }
                //if this direction is unavailable due to pin, don't search it
                if(restrictedDirection){
                    break;
                }
            }


            if(nextSquare < 0 || nextSquare >= 64){
                break;
            }

            Piece p = board.at(nextSquare);

            if(p.type == PieceType::None){
                if(!checkingForPin){
                    Move m(static_cast<uint8_t>(square), static_cast<uint8_t>(nextSquare));
                    possibleMoves.push_back(m);
                    count++;
                }
                if(cap){
                    break;
                }
            }
            else if(p.color != color){
                if(checkingForPin){
                    if(p.type == PieceType::K){
                        Piece& piece = board.at(possiblePinSquare);
                        Pin pin(possiblePinSquare, square, nextSquare, board.at(possiblePinSquare), PinType::Diagonal);
                        pins.emplace_back(pin);
                        piece.pinnedD = true;
                        piece.pins.push_back(&pins.back());
                        piece.pinDirections.push_back(DiagonalDirections[i]);
                    }
                    break;
                }
                if(!checkingForPin){
                    Move m(static_cast<uint8_t>(square), static_cast<uint8_t>(nextSquare));
                    possibleMoves.push_back(m);
                    count++;
                    if(p.type == PieceType::K){
                        color == Color::w ? 
                            squaresAttackingBlackKing.push_back(square) :
                            squaresAttackingWhiteKing.push_back(square);
                            break;
                    }
                    checkingForPin = true;
                    possiblePinSquare = nextSquare;
                }
                if(cap || !checkingForPin){
                    break;
                }
            }
            else{
                break;
            }
        }
    }
}

void Position::generateOrthoganalMoves(int& count, int square, bool cap, Color color, std::vector<int>& pinDirections){;
    Piece p = board.at(square);
    for(int i = 0; i < 4; i++){
        bool checkingForPin = false;
        int nextSquare = square;
        int possiblePinSquare = -1;
        while(true){
            if(nextSquare % 8 == 0 && i == 0){
                break;
            }
            if((nextSquare + 1) % 8 == 0 && i == 1){
                break;
            }
            nextSquare+= OrthoganalDirections[i];
            if(nextSquare < 0 || nextSquare >= 64){
                break;
            }


            //determine if the current direction is a restricted direction due to a pin
            if(p.pinnedO){
                bool restrictedDirection = true;
                for(size_t j = 0; j < pinDirections.size(); j++){
                    if(OrthoganalDirections[i] == pinDirections[j] || OrthoganalDirections[i] == -pinDirections[j]){
                        restrictedDirection = false;
                    }
                }
                //if this direction is unavailable due to pin, don't search it
                if(restrictedDirection){
                    break;
                }
            }

            Piece p = board.at(nextSquare);
            if(p.type == PieceType::None){
                if(!checkingForPin){
                    Move m(static_cast<uint8_t>(square), static_cast<uint8_t>(nextSquare));
                    possibleMoves.push_back(m);
                    count++;
                }
                if(cap){
                    break;
                }
            }
            else if(p.color != color){
                if(checkingForPin){
                    if(p.type == PieceType::K){
                        Piece& piece = board.at(possiblePinSquare);
                        Pin pin(possiblePinSquare, square, nextSquare, board.at(possiblePinSquare), PinType::Orthoganal);
                        pins.emplace_back(pin);
                        piece.pinnedO = true;
                        piece.pins.push_back(&pins.back());
                        piece.pinDirections.push_back(OrthoganalDirections[i]);
                    }
                    break;
                }
                if(!checkingForPin){
                    Move m(static_cast<uint8_t>(square), static_cast<uint8_t>(nextSquare));
                    possibleMoves.push_back(m);
                    count++;
                    if(p.type == PieceType::K){
                        color == Color::w ? 
                            squaresAttackingBlackKing.push_back(square) :
                            squaresAttackingWhiteKing.push_back(square);
                            break;
                    }
                    checkingForPin = true;
                    possiblePinSquare = nextSquare;
                }
                if(cap || !checkingForPin){
                    break;
                }
            }
            else{
                break;
            }
        }
    }
}

void Position::getValidMovesKnight(int& count, int square){
    if(square < 0 || square >= 64){
        throw std::out_of_range("Position::getValidMovesKnight: provided square is out of range");
    }
    Piece n = board.at(square);
    if(n.type != PieceType::N){
        throw std::logic_error("Position::getValidMovesKnight: generating moves for incorret piece");
    }

    for(int i = 0; i < std::size(KnightMoves); i++){
        if((square % 8) == 0 && i < 4){ //a file
            continue;
        }
        else if((square - 1) % 8 == 0 && i < 2){ //b file
            continue;
        }  
        else if((square + 1) % 8 == 0 && i >= 4){ //h file
            continue;
        }
        else if((square + 2) % 8 == 0 && i >= 6){
            continue;
        }

        int targetSquare = square + KnightMoves[i];
        if(targetSquare >= 0 && targetSquare <= 63){
            Piece p = board.at(targetSquare);
            if(p.color != n.color){
                possibleMoves.push_back(Move(square, targetSquare));
                count++;
                if(p.type == PieceType::K){
                    n.color == Color::w ? 
                        squaresAttackingBlackKing.push_back(square) :
                        squaresAttackingWhiteKing.push_back(square);
                }
            }
        }
    }
}

bool Position::tryMove(int src, int dest){
    if(src < 0 || src > 63 || dest < 0 || dest > 63){
        throw std::out_of_range("Position::tryMove: provided square is out of range");
    }

    //TODO
}

void Position::generateAllValidMovesForSide(Side side){
    Color color;
    side == Side::w ? color = Color::w : color = Color::b;

    bool thisSideInCheck = ((color == Color::w && squaresAttackingWhiteKing.size() > 0) || (color == Color::b && squaresAttackingBlackKing.size() > 0));

    for(int i = 0; i < 64; i++){
        if(board.at(i).color == color){
            if(thisSideInCheck){
                generateCheckResolutions(i, color);
            }
            else{
                generateValidMoves(i);
            }
        }
    }
}


void Position::generateValidMoves(int square){
    if(square < 0 || square > 63){
        throw std::out_of_range("Position::generateValidMoves: provided square is out of range");
    }

    Piece p = board.at(square);
    if(p.type == PieceType::None) return;
    Color pColor = p.color;

    int moveIndex = possibleMoves.size();
    int count = 0;

    std::string piece = "";
    if(p.type == PieceType::R){
        piece = "Rook";
    }

    bool canCastle = true;
    auto castleLambda = [this, &pColor, &canCastle](int sq){
        if((sq % 8) == 0 || sq % 8 == 0){
            return true;
        }
        if(board.at(sq).type != PieceType::None){
            canCastle = false;
            return true;
        }
        if(isSquareAttacked(sq, pColor)){
            canCastle = false;
            return true;
        }
    };

    if(pColor == Color::w){
        if(!kingHasMovedWhite){
            if(!rookLeftHasMovedWhite){
                bool canCastle = true;
                if(squaresAttackingWhiteKing.size() == 0){
                    walkDirectionsAndDo(square, {-1}, false, castleLambda);
                    if(canCastle){
                        //add the move
                    }
                }
            }
        }
    }

    std::vector<Pin*> pins = p.pins;
    switch(p.type){
        case PieceType::P:
            getValidMovesPawn(count, square, p.pinDirections);
            break;
        case PieceType::B:
            if(!p.pinnedO){
                generateDiagonalMoves(count, square, false, pColor, p.pinDirections);
            }
            break;
        case PieceType::N:
            if(!p.pinnedO && !p.pinnedD){
                getValidMovesKnight(count, square);
            }
            break;
        case PieceType::R:
            if(!p.pinnedD){
                generateOrthoganalMoves(count, square, false, pColor, p.pinDirections);
            }
            break;
        case PieceType::Q:
            if(!p.pinnedD){
                generateOrthoganalMoves(count, square, false, pColor, p.pinDirections);
            }
            if(!p.pinnedO){
                generateDiagonalMoves(count, square, false, pColor, p.pinDirections);
            }
            break;
        case PieceType::K:
            generateDiagonalMoves(count, square, true, pColor, p.pinDirections);
            generateOrthoganalMoves(count, square, true, pColor, p.pinDirections);
            break;
        default:
            break;
    }

    if(count > 0){
        moveStartIndices[square] = moveIndex;
        moveCounts[square] = count;
    }
}

std::vector<Move> Position::getValidMovesForPieceAt(int square) const {
    std::vector<Move> moves;
    int startIndex = moveStartIndices[square];
    for(int i = 0; i < moveCounts[square]; i++){
        moves.push_back(possibleMoves[startIndex + i]);
    }
    return moves;
}

//Possible performance issue in that we will call this multiple times for a position with a check, causing multiple redeclarations of the lambda
void Position::generateCheckResolutions(int square, Color color){
    Piece p = board.at(square);
    std::vector<int>* attackers;
    int kingSquare = -1;
    color == Color::w ? attackers = &squaresAttackingWhiteKing : attackers = &squaresAttackingBlackKing;
    color == Color::w ? kingSquare = board.whiteKingSquare : kingSquare = board.blackKingSquare;

    int generatedMoveCount = 0;
    int moveIndex = possibleMoves.size();

    // Pinned pieces can never resolve a check
    if(p.type != PieceType::K && (p.pinnedD || p.pinnedO)){
        return;
    }

    auto pawnMoveLambda = [&color, this, &attackers, &kingSquare, &square, &generatedMoveCount](int sq){
        bool checkResolved = false;
        for(const auto& attackerSquare : *attackers){
            if(board.at(sq).color == Color::None){
                if(squareIntersectsDiagonal(attackerSquare, sq, kingSquare) || squareIntersectsOrthoganal(attackerSquare, sq, kingSquare)){
                    checkResolved = true;
                }
            }
            if(checkResolved == false){
                return false;
            }
            checkResolved = false;
        }
        possibleMoves.push_back(Move(square, sq));
        generatedMoveCount++;
        return true;
    };

    auto pawnCaptureLambda = [&color, this, &attackers, &kingSquare, &generatedMoveCount, &square](int sq){
        bool checkResolved = false;
        for(const auto& attackerSquare : *attackers){
            if(board.at(sq).color != color && board.at(sq).color != Color::None){
                if(squareIntersectsDiagonal(attackerSquare, sq, kingSquare) || squareIntersectsOrthoganal(attackerSquare, sq, kingSquare)){
                    checkResolved = true;
                }

                if(sq == attackerSquare){
                    checkResolved = true;
                }
            }
            if(checkResolved == false){
                return false;
            }
            checkResolved = false;
        }
        possibleMoves.push_back(Move(square, sq));
        generatedMoveCount++;
        return true;
    };

    auto checkLambda = [&color, this, &attackers, &kingSquare, &generatedMoveCount, &square](int sq){
        bool checkResolved = false;
        for(const auto& attackerSquare : *attackers){

            if(board.at(sq).color != color){
                if(squareIntersectsDiagonal(attackerSquare, sq, kingSquare) || squareIntersectsOrthoganal(attackerSquare, sq, kingSquare)){
                    checkResolved = true;

                        //TODO: mark self as pinned WHEN THE MOVE IS ACTUALLY MADE
                }
                if(sq == attackerSquare){
                    checkResolved = true;
                }
            }

            if(checkResolved == false){
                return false;
            }

            checkResolved = false;
        }
        possibleMoves.push_back(Move(square, sq));
        generatedMoveCount++;
        return true;
    };

    auto kingEscapeLambda = [this, &color, &generatedMoveCount, &square](int sq){
        if(board.at(sq).color != color){
            if(!isSquareAttacked(sq, color)){
                possibleMoves.push_back(Move(square, sq));
                generatedMoveCount++;
            }
        }
        return true;
    };


    switch (p.type){
        case PieceType::B:
            walkDirectionsAndDo(square, DiagonalDirections, false, checkLambda);
            break;
        case PieceType::Q:
            walkDirectionsAndDo(square, DiagonalDirections, false, checkLambda);
            walkDirectionsAndDo(square, OrthoganalDirections, false, checkLambda);
            break;
        case PieceType::R:
            walkDirectionsAndDo(square, OrthoganalDirections, false, checkLambda);
            break;
        case PieceType::N:
            walkDirectionsAndDo(square, KnightMoves, true, checkLambda);
            break;
        case PieceType::P:
            if(color == Color::w){
                walkDirectionsAndDo(square, {-7, -9}, true, pawnCaptureLambda);
                if(square >= 48 && square <= 55 && board.at(square - 8).type == PieceType::None){
                    walkDirectionsAndDo(square, {-8, -16}, true, pawnMoveLambda);
                }
                else{
                    walkDirectionsAndDo(square, {-8}, true, pawnMoveLambda);
                }
            }
            else{
                walkDirectionsAndDo(square, {7, 9}, true, pawnCaptureLambda);
                if(square >= 8 && square <= 15 && board.at(square + 8).type == PieceType::None){
                    walkDirectionsAndDo(square, {8, 16}, true, pawnMoveLambda);
                }
                else{
                    walkDirectionsAndDo(square, {8}, true, pawnMoveLambda);
                }
            }
            break;
        case PieceType::K:
            walkDirectionsAndDo(square, DiagonalDirections, true, kingEscapeLambda);
            walkDirectionsAndDo(square, OrthoganalDirections, true, kingEscapeLambda);
            break;
        default: 
            break;
        }

        if(generatedMoveCount > 0){
            moveStartIndices[square] = moveIndex;
            moveCounts[square] = generatedMoveCount;
        }
        return;      
    }

template<size_t N, typename Func>
void Position::walkDirectionsAndDo(int startSquare, const int (&directions)[N], bool limitedMovementPiece, Func func){
    int curr = startSquare;
    for(int i = 0; i < N; i++){
        while(true){
            if(curr % 8 == 0 && (directions[i] == -1 || directions[i] == 7 || directions[i] == -9)) break;
            if((curr + 1) % 8 == 0 && (directions[i] == 1 || directions[i] == -7 || directions[i] == 9)) break;
    
            curr += directions[i];
            if(curr < 0 || curr >= 64) break;

            if(func(curr) || limitedMovementPiece) break;
        }
        curr = startSquare;
    }
}


bool Position::isSquareAttacked(int square, Color color){
    bool attacked = false;

    walkDirectionsAndDo(square, DiagonalDirections, false, [&attacked, this, color](int sq){
        if(board.at(sq).type != PieceType::None){
            if((board.at(sq).type == PieceType::B || board.at(sq).type == PieceType::Q) && board.at(sq).color != color){
                    attacked = true; 
            }

            //to handle squares attacked by x-ray: if we see our own king, keep looking
            if(board.at(sq).type == PieceType::K && board.at(sq).color == color){
                return false;
            }
            return true;
        }

        return false;
    });

    walkDirectionsAndDo(square, OrthoganalDirections, false, [&attacked, this, color](int sq){
        if(board.at(sq).type != PieceType::None){
            if((board.at(sq).type == PieceType::R || board.at(sq).type == PieceType::Q) && board.at(sq).color != color){
                    attacked = true;
            }

            //to handle squares attacked by x-ray: if we see our own king, keep looking
            if(board.at(sq).type == PieceType::K && board.at(sq).color == color){
                return false;
            }
            return true;
        }
        return false;
    });

    walkDirectionsAndDo(square, KnightMoves, true, [&attacked, this, color](int sq){
        if(board.at(sq).type != PieceType::None){
            if((board.at(sq).type == PieceType::N) && board.at(sq).color != color){
                    attacked = true; 
            }
            return true;
        }
        return false;
    });

    walkDirectionsAndDo(square, color == Color::w ? pawnAttackerDirectionsWhite : pawnAttackerDirectionsBlack, true, [&attacked, this, color](int sq){
        if(board.at(sq).type != PieceType::None){
            if((board.at(sq).type == PieceType::P) && board.at(sq).color != color){
                    attacked = true;; 
            }
            return true;
        }
        return false;
    });

    return attacked;
}

bool Position::makeMove(const Move& move){
    Side opponentSide = (sideToMove == Side::w) ? Side::b : Side::w;
    generateAllValidMovesForSide(opponentSide);
    possibleMoves.clear();
    moveStartIndices.fill(-1);
    moveCounts.fill(-1);

    generateAllValidMovesForSide(sideToMove);

    int start = moveStartIndices[move.from];
    int end = start + moveCounts[move.from];
    bool found = false;
    for(int i = start; i < end; i++){
        if(possibleMoves[i] == move){
            found = true;
        }
    }

    if(!found){
        return false;
    }

    board.movePiece(move.from, move.to);
    possibleMoves.clear();
    moveStartIndices.fill(-1);
    moveCounts.fill(0);
    pins.clear();

    for(auto& piece : board.getSquares()){
        piece.pins.clear();
        piece.pinDirections.clear();
        piece.pinnedD = false;
        piece.pinnedO = false;
    }

    sideToMove = (sideToMove == Side::w) ? Side::b : Side::w;

    generatePieceLists();

    possibleMoves.clear();
    moveStartIndices.fill(-1);
    moveCounts.fill(-1);

    squaresAttackingBlackKing.clear();
    squaresAttackingWhiteKing.clear();

    // Generate opponent moves for attack/pin data
    opponentSide = (sideToMove == Side::w) ? Side::b : Side::w;
    generateAllValidMovesForSide(opponentSide);

    // Clear move data but keep attack/pin info
    possibleMoves.clear();
    moveStartIndices.fill(-1);
    moveCounts.fill(-1);

    // Generate current side's legal moves
    generateAllValidMovesForSide(sideToMove);

    return true;
}

std::string Position::toFEN() const {
    std::string fen = board.toFEN();
    fen += (sideToMove == Side::w) ? " w" : " b";
    fen += " - - 0 1";
    return fen;
}

//determining checks
//need to know:
    //are we currently in check?
        //if we are currently in check:
            //can we move out of check?
            //can we block the check?
            //can we capture the attacker?
    //will a move put us in check? (therefore illegal)


    //start: generate all possible moves for white
        //let white make a move
        //black generates all of white's possible moves, and generates its own possible moves based on white's possible moves
        //black makes a move
        //white generates all of black's possible moves, and generates its own possible moves based on black's possible moves
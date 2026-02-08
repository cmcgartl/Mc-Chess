#include "position.h"
#include "move.h"

//define move/diretion distances
//moves that change files are ordered together for wrap around handling (moves that could go off the side edge of the board)
static constexpr int DiagonalDirections[4] = {7, -9, 9, -7};
static constexpr int OrthoganalDirections[4] = {-1, 1, -8, 8};
static constexpr int KnightMoves[8] = {-10, 6, -17, 15, 17, -15, 10, -6};

bool squareInvalid(int square){
    if(square < 0 || square > 63){
        return true;
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

void Position::getValidMovesPawn(int& count, int square){
    if(square < 0 || square >= 64){
        throw std::out_of_range("Position::getValidMovesPawn: provided square is out of range");
    }
    Piece p = board.at(square);
    if(p.type != PieceType::P){
        throw std::logic_error("Position::getValidMovesPawn: generating moves for incorrect piece at square" + std::to_string(square));
    }
    if(p.color == Color::w){
        int oneForward = square - 8; //square one square forward
        int twoForward = square - 16; //square two squares forward
        int captureLeft = (square - 8) - 1;
        int captureRight = (square - 8) + 1;
        if(oneForward >= 0){
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
        if(captureLeft >= 0 && ((square % 8) != 0)){
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
        if(captureRight >= 0 && ((square + 1) % 8 != 0)){
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
        int oneForward = square + 8; //square one square forward
        int twoForward = square + 16; //square two squares forward
        int captureLeft = square + 8 + 1;
        int captureRight = square + 8 - 1;
        if(oneForward <= 63){
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
        if(captureRight <= 63 && (square % 8 != 0)){
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
        if(captureLeft <= 63 && ((square + 1) % 8 != 0)){
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
                for(size_t j = 0; i < pinDirections.size(); i++){
                    if(DiagonalDirections[i] == pinDirections[j] || DiagonalDirections[i] == -pinDirections[j]){
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
                        piece.pins.push_back(&pin);
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

void Position::generateOrthoganalMoves(int& count, int square, bool cap, Color color){;
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
                        piece.pins.push_back(&pin);
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

    for(int i = 0; i < 64; i++){
        if(board.at(i).color == color){
            generateValidMoves(i);
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
    if(p.type == PieceType::P){
        piece = "Pawn";
    }

    std::vector<Pin*> pins = p.pins;
    switch(p.type){
        case PieceType::P:
            getValidMovesPawn(count, square);
            break;
        case PieceType::B:
            if(!p.pinnedO){
                generateDiagonalMoves(count, square, false, pColor);
            }
            break;
        case PieceType::N:
            if(!p.pinnedO && !p.pinnedD){
                getValidMovesKnight(count, square);
            }
            break;
        case PieceType::R:
            if(!p.pinnedD){
                generateOrthoganalMoves(count, square, false, pColor);
            }
            break;
        case PieceType::Q:
            if(!p.pinnedD){
                generateOrthoganalMoves(count, square, false, pColor);
            }
            if(!p.pinnedO){
                generateDiagonalMoves(count, square, false, pColor);
            }
            break;
        case PieceType::K:
            generateDiagonalMoves(count, square, true, pColor);
            generateOrthoganalMoves(count, square, true, pColor);
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

bool Position::moveStopsCheck(Move& move, std::vector<int>& attackingSquares){
    if(attackingSquares.size() == 0){
        return true;
    }
    if(attackingSquares.size() == 1 && move.to == attackingSquares[0]){
        return true;
    }

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
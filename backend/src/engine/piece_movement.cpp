#include "piece_movement.h"
#include "direction_constants.h"
#include "move_gen_helpers.h"
#include "move.h"

void getValidMovesPawn(int& count, int square, MoveGenResult& result, Position& P){
    int enPassantSquare = P.getEnPassantSquare();
    const auto& board = P.getBoard();
    if(square < 0 || square >= 64){
        throw std::out_of_range("Position::getValidMovesPawn: provided square is out of range");
    }
    Piece p = board.at(square);
    if(p.type != PieceType::P){
        throw std::logic_error("Position::getValidMovesPawn: generating moves for incorrect piece at square" + std::to_string(square));
    }

    const PinInfo& pin = result.pins[square];
    auto dirAllowed = [&pin](int dir){
        if(pin.pinDirection == 0) return true;
        return (pin.pinDirection == dir || pin.pinDirection == -dir);
    };

    if(p.color == Color::w){
        int oneForward = square - 8;
        int twoForward = square - 16;
        int captureLeft = (square - 8) - 1;
        int captureRight = (square - 8) + 1;
        if(dirAllowed(-8) && oneForward >= 0){
            if(board.at(oneForward).type == PieceType::None){
                if(0 <= oneForward && oneForward <= 7){
                    result.moves.push_back(Move(square, oneForward, MoveType::P));
                    count++;
                }
                else{
                    result.moves.push_back(Move(square, oneForward));
                    count++;
                }

                if(square >= 48 && square <= 55){
                    if(board.at(twoForward).type == PieceType::None){
                        result.moves.push_back(Move(square, twoForward, MoveType::PawnDouble));
                        count++;
                    }
                }
            }
        }
        if(dirAllowed(-9) && captureLeft >= 0 && ((square % 8) != 0)){
            if(captureLeft == enPassantSquare){
                result.moves.push_back(Move(square, captureLeft, MoveType::EP));
                count++;
            }
            else if(board.at(captureLeft).color == Color::b){
                if(board.at(captureLeft).type == PieceType::K){
                    p.color == Color::w ?
                        result.attacksOnBlackKing.push_back(square) :
                        result.attacksOnWhiteKing.push_back(square);
                }
                if(0 <= captureLeft && captureLeft <= 7){
                    result.moves.push_back(Move(square, captureLeft, MoveType::P));
                    count++;
                }
                else{
                    result.moves.push_back(Move(square, captureLeft));
                    count++;
                }
            }
        }
        if(dirAllowed(-7) && captureRight >= 0 && ((square + 1) % 8 != 0)){
            if(captureRight == enPassantSquare){
                result.moves.push_back(Move(square, captureRight, MoveType::EP));
                count++;
            }
            else if(board.at(captureRight).color == Color::b){
                if(board.at(captureRight).type == PieceType::K){
                    p.color == Color::w ?
                        result.attacksOnBlackKing.push_back(square) :
                        result.attacksOnWhiteKing.push_back(square);
                }
                if(0 <= captureRight && captureRight <= 7){
                    result.moves.push_back(Move(square, captureRight, MoveType::P));
                    count++;
                }
                else{
                    result.moves.push_back(Move(square, captureRight));
                    count++;
                }
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
                if(56 <= oneForward && oneForward <= 63){
                    result.moves.push_back(Move(square, oneForward, MoveType::P));
                    count++;
                }
                else{
                    result.moves.push_back(Move(square, oneForward));
                    count++;
                }

                if(square >= 8 && square <= 15){
                    if(board.at(twoForward).type == PieceType::None){
                        result.moves.push_back(Move(square, square + 16, MoveType::PawnDouble));
                        count++;
                    }
                }
            }
        }
        if(dirAllowed(7) && captureRight <= 63 && (square % 8 != 0)){
            if(captureRight == enPassantSquare){
                result.moves.push_back(Move(square, captureRight, MoveType::EP));
                count++;
            }
            else if(board.at(captureRight).color == Color::w){
                if(board.at(captureRight).type == PieceType::K){
                    p.color == Color::w ?
                        result.attacksOnBlackKing.push_back(square) :
                        result.attacksOnWhiteKing.push_back(square);
                }
                if(56 <= captureRight && captureRight <= 63){
                    result.moves.push_back(Move(square, captureRight, MoveType::P));
                    count++;
                }
                else{
                    result.moves.push_back(Move(square, captureRight));
                    count++;
                }
            }
        }
        if(dirAllowed(9) && captureLeft <= 63 && ((square + 1) % 8 != 0)){
            if(captureLeft == enPassantSquare){
                result.moves.push_back(Move(square, captureLeft, MoveType::EP));
                count++;
            }
            else if(board.at(captureLeft).color == Color::w){
                if(board.at(captureLeft).type == PieceType::K){
                    p.color == Color::w ?
                        result.attacksOnBlackKing.push_back(square) :
                        result.attacksOnWhiteKing.push_back(square);
                }
                if(56 <= captureLeft && captureLeft <= 63){
                    result.moves.push_back(Move(square, captureLeft, MoveType::P));
                    count++;
                }
                else{
                    result.moves.push_back(Move(square, captureLeft));
                    count++;
                }
            }
        }
    }
}

void generateDiagonalMoves(int& count, int square, bool cap, Color color, MoveGenResult& result, Position& P){
    const auto& board = P.getBoard();
    const PinInfo& pin = result.pins[square];
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
            if(pin.pinnedD){
                if(DiagonalDirections[i] != pin.pinDirection && DiagonalDirections[i] != -pin.pinDirection){
                    break;
                }
            }
            if(nextSquare < 0 || nextSquare >= 64){
                break;
            }

            Piece p = board.at(nextSquare);

            if(p.type == PieceType::None){
                if(!checkingForPin){
                    if(board.at(square).type == PieceType::K){
                        if(!isSquareAttacked(nextSquare, color, P)){
                            Move m(static_cast<uint8_t>(square), static_cast<uint8_t>(nextSquare));
                            result.moves.push_back(m);
                            count++;
                        }
                    }
                    else{
                    Move m(static_cast<uint8_t>(square), static_cast<uint8_t>(nextSquare));
                    result.moves.push_back(m);
                    count++;
                    }
                }
                if(cap){
                    break;
                }
            }
            else if(p.color != color){
                if(checkingForPin){
                    if(p.type == PieceType::K){
                        result.pins[possiblePinSquare].pinnedD = true;
                        result.pins[possiblePinSquare].pinDirection = DiagonalDirections[i];
                    }
                    break;
                }
                if(!checkingForPin){
                    if(board.at(square).type == PieceType::K){
                        if(!isSquareAttacked(nextSquare, color, P)){
                            Move m(static_cast<uint8_t>(square), static_cast<uint8_t>(nextSquare));
                            result.moves.push_back(m);
                            count++;
                        }
                    } else {
                        Move m(static_cast<uint8_t>(square), static_cast<uint8_t>(nextSquare));
                        result.moves.push_back(m);
                        count++;
                        if(p.type == PieceType::K){
                            color == Color::w ?
                                result.attacksOnBlackKing.push_back(square) :
                                result.attacksOnWhiteKing.push_back(square);
                                break;
                        }
                        checkingForPin = true;
                        possiblePinSquare = nextSquare;
                    }
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

void generateOrthoganalMoves(int& count, int square, bool cap, Color color, MoveGenResult& result, Position& P){
    const auto& board = P.getBoard();
    const PinInfo& pin = result.pins[square];
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
            if(pin.pinnedO){
                if(OrthoganalDirections[i] != pin.pinDirection && OrthoganalDirections[i] != -pin.pinDirection){
                    break;
                }
            }

            Piece p = board.at(nextSquare);
            if(p.type == PieceType::None){
                if(!checkingForPin){
                    if(board.at(square).type == PieceType::K){
                        if(!isSquareAttacked(nextSquare, color, P)){
                            Move m(static_cast<uint8_t>(square), static_cast<uint8_t>(nextSquare));
                            result.moves.push_back(m);
                            count++;
                        }
                    }
                    else{
                        Move m(static_cast<uint8_t>(square), static_cast<uint8_t>(nextSquare));
                        result.moves.push_back(m);
                        count++;
                    }
                }
                if(cap){
                    break;
                }
            }
            else if(p.color != color){
                if(checkingForPin){
                    if(p.type == PieceType::K){
                        result.pins[possiblePinSquare].pinnedO = true;
                        result.pins[possiblePinSquare].pinDirection = OrthoganalDirections[i];
                    }
                    break;
                }
                if(!checkingForPin){
                    if(board.at(square).type == PieceType::K){
                        if(!isSquareAttacked(nextSquare, color, P)){
                            Move m(static_cast<uint8_t>(square), static_cast<uint8_t>(nextSquare));
                            result.moves.push_back(m);
                            count++;
                        }
                    } else {
                        Move m(static_cast<uint8_t>(square), static_cast<uint8_t>(nextSquare));
                        result.moves.push_back(m);
                        count++;
                        if(p.type == PieceType::K){
                            color == Color::w ?
                                result.attacksOnBlackKing.push_back(square) :
                                result.attacksOnWhiteKing.push_back(square);
                                break;
                        }
                        checkingForPin = true;
                        possiblePinSquare = nextSquare;
                    }
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

void getValidMovesKnight(int& count, int square, MoveGenResult& result, Position& P){
    const auto& board = P.getBoard();
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
                result.moves.push_back(Move(square, targetSquare));
                count++;
                if(p.type == PieceType::K){
                    n.color == Color::w ?
                        result.attacksOnBlackKing.push_back(square) :
                        result.attacksOnWhiteKing.push_back(square);
                }
            }
        }
    }
}
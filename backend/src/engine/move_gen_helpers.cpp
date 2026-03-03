#include "move_gen_helpers.h"
#include "direction_constants.h"

bool isSquareAttacked(int square, Color color, Position& p){
    bool attacked = false;
    const auto& board = p.getBoard();
    walkDirectionsAndDo(square, DiagonalDirections, false, [&attacked, &board, color](int sq){
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

    walkDirectionsAndDo(square, OrthoganalDirections, false, [&attacked, &board, color](int sq){
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

    walkDirectionsAndDo(square, KnightMoves, true, [&attacked, &board, color](int sq){
        if(board.at(sq).type != PieceType::None){
            if((board.at(sq).type == PieceType::N) && board.at(sq).color != color){
                    attacked = true;
            }
            return true;
        }
        return false;
    });

    walkDirectionsAndDo(square, color == Color::w ? pawnAttackerDirectionsWhite : pawnAttackerDirectionsBlack, true, [&attacked, &board, color](int sq){
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


//Possible performance issue in that we will call this multiple times for a position with a check, causing multiple redeclarations of the lambda
void generateCheckResolutions(int square, Color color, MoveGenResult& result, Position& P){
    const auto& board = P.getBoard();
    Piece p = board.at(square);
    std::vector<int>* attackers;
    int kingSquare = -1;
    color == Color::w ? attackers = &result.attacksOnWhiteKing : attackers = &result.attacksOnBlackKing;
    color == Color::w ? kingSquare = board.whiteKingSquare : kingSquare = board.blackKingSquare;

    int generatedMoveCount = 0;
    int moveIndex = result.moves.size();

    // Pinned pieces can never resolve a check
    const PinInfo& pin = result.pins[square];
    if(p.type != PieceType::K && (pin.pinnedD || pin.pinnedO)){
        return;
    }

    auto pawnMoveLambda = [&color, &board, &attackers, &kingSquare, &square, &generatedMoveCount, &result](int sq){
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
        result.moves.push_back(Move(square, sq));
        generatedMoveCount++;
        return true;
    };

    auto pawnCaptureLambda = [&color, &board, &attackers, &kingSquare, &generatedMoveCount, &square, &result](int sq){
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
        result.moves.push_back(Move(square, sq));
        generatedMoveCount++;
        return true;
    };

    auto checkLambda = [&color, &board, &attackers, &kingSquare, &generatedMoveCount, &square, &result](int sq){
        bool checkResolved = false;
        for(const auto& attackerSquare : *attackers){

            if(board.at(sq).color != color){
                if(squareIntersectsDiagonal(attackerSquare, sq, kingSquare) || squareIntersectsOrthoganal(attackerSquare, sq, kingSquare)){
                    checkResolved = true;
                }
                if(board.at(sq).type != PieceType::None){
                    if(sq == attackerSquare){
                        checkResolved = true;
                    }
                    else{
                        checkResolved = false;
                        return true;
                    }
                }
            }
            else{
                return true;
            }
            if(checkResolved == false){
                return false;
            }

            checkResolved = false;
        }
        result.moves.push_back(Move(square, sq));
        generatedMoveCount++;
        return true;
    };

    auto kingEscapeLambda = [&board, &color, &generatedMoveCount, &square, &result, &P](int sq){
        if(board.at(sq).color != color){
            if(!isSquareAttacked(sq, color, P)){
                result.moves.push_back(Move(square, sq));
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
            result.startIndices[square] = moveIndex;
            result.counts[square] = generatedMoveCount;
        }
        return;
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

bool squareInvalid(int square){
    if(square < 0 || square > 63){
        return true;
    }
    return false;
}
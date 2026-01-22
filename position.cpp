#include "position.h"

static constexpr int DiagonalDirections[4] = {7, -9, 9, -7};
static constexpr int OrthoganalDirections[4] = {-1, 1, -8, 8};

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


std::vector<int> Position::getValidMovesPawn(int square) const{
    std::vector<int> moves;
    if(square < 0 || square >= 64){
        throw std::out_of_range("Position::getValidMovesPawn: provided squar is out of range");
    }
    Piece p = board.at(square);
    if(p.type != PieceType::P){
        throw std::logic_error("Position::getValidMovesPawn: generating moves for incorret piece");
    }
    if(p.color == Color::w){
        int oneForward = square - 8; //square one square forward
        int twoForward = square - 16; //square two squares forward
        int captureLeft = (square - 8) - 1;
        int captureRight = (square - 8) + 1;
        if(oneForward >= 0){
            if(board.at(oneForward).type == PieceType::None){
                moves.push_back(oneForward);
            
                if(square >= 48 && square <= 55){
                    if(board.at(twoForward).type == PieceType::None){
                        moves.push_back(square - 16);
                    }
                }
            }
        }
        if(captureLeft >= 0 && ((square % 8) != 0)){
            if(board.at(captureLeft).color == Color::b){
                moves.push_back(captureLeft);
            }
        }
        if(captureRight >= 0 && ((square + 1) % 8 != 0)){
            if(board.at(captureRight).color == Color::b){
                moves.push_back(captureRight);
            }
        }
    }
    if(p.color == Color::b){
        int oneForward = square + 8; //square one square forward
        int twoForward = square + 16; //square two squares forward
        int captureLeft = square + 8 - 1;
        int captureRight = square + 8 + 1;
        if(oneForward <= 63){
            if(board.at(oneForward).type == PieceType::None){
                moves.push_back(oneForward);
            
                if(square >= 8 && square <= 15){
                    if(board.at(twoForward).type == PieceType::None){
                        moves.push_back(square + 16);
                    }
                }
            }
        }
        if(captureRight <= 63 && (square % 8 != 0)){
            if(board.at(captureRight).color == Color::w){
                moves.push_back(captureRight);
            }
        }
        if(captureLeft <= 63 && ((square + 1) % 8 != 0)){
            if(board.at(captureLeft).color == Color::w){
                moves.push_back(captureLeft);
            }
        }
    }
    return moves;
}

std::vector<int> Position::getValidMovesBishop(int square) const{
    std::vector<int> moves;
    moves.reserve(13);
    if(square < 0 || square >= 64){
        throw std::out_of_range("Position::getValidMovesPawn: provided squar is out of range");
    }
    Piece b = board.at(square);
    if(b.type != PieceType::B){
        throw std::logic_error("Position::getValidMovesPawn: generating moves for incorret piece");
    }

    generateDiagonalMoves(moves, square, false, b.color);

    return moves;
}

std::vector<int>Position::getValidMovesQueen(int square) const {
    std::vector<int> moves;
    moves.reserve(27); // maximum moves a queen can make
    if(square < 0 || square >= 64){
        throw std::out_of_range("Position::getValidMovesPawn: provided squar is out of range");
    }
    Piece q = board.at(square);
    if(q.type != PieceType::Q){
        throw std::logic_error("Position::getValidMovesPawn: generating moves for incorret piece");
    }

    generateDiagonalMoves(moves, square, false, q.color);
    generateOrthoganalMoves(moves, square, false, q.color);

    return moves;
}


void Position::generateDiagonalMoves(std::vector<int>& moves, int square, bool cap, Color color) const{;
    for(int i = 0; i < 4; i++){
        int nextSquare = square;
        int count = 0;
        while(true){
            if(nextSquare % 8 == 0 && i <= 1){
                break;
            }
            if((nextSquare + 1) % 8 == 0 && i >= 2){
                break;
            }
            nextSquare+= DiagonalDirections[i];
            if(nextSquare < 0 || nextSquare >= 64){
                break;
            }

            Piece p = board.at(nextSquare);

            if(p.type == PieceType::None){
                moves.push_back(nextSquare);
                count++;
                if(cap){
                    break;
                }
            }
            else if(p.color != color){
                moves.push_back(nextSquare);
                count++;
                break;
            }
            else{
                break;
            }
        }
    }
}

void Position::generateOrthoganalMoves(std::vector<int>& moves, int square, bool cap, Color color) const{;
    for(int i = 0; i < 4; i++){
        int nextSquare = square;
        int count = 0;
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
                moves.push_back(nextSquare);
                count++;
                if(cap){
                    break;
                }
            }
            else if(p.color != color){
                moves.push_back(nextSquare);
                count++;
                break;
            }
            else{
                break;
            }
        }
    }
}

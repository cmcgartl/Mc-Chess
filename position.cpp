#include "position.h"

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

std::vector<int> Position::getValidMovesPawn(int square){
    std::vector<int> moves;
    Piece p = board.at(square);
    if(p.type != PieceType::P){
        throw std::logic_error("Position::getValidMovesPawn: generating moves for incorret piece");
    }
    if(square < 0 || square >= 64){
        throw std::out_of_range("Position::getValidMovesPawn: provided squar is out of range");
    }
    if(p.color == Color::w){
        int one = square - 8; //square one square forward
        int two = square - 16; //square two squares forward
        if(one >= 0){
            if(board.at(one).type == PieceType::None){
                moves.push_back(one);
            }
        
            if(square >= 48 && square <= 55){
                if(board.at(two).type == PieceType::None){
                    moves.push_back(square - 16);
                }
            }
        }
    }
    if(p.color == Color::b){
        int one = square - 8; //square one square forward
        int two = square - 16; //square two squares forward
        if(one <= 63){
            if(board.at(one).type == PieceType::None){
                moves.push_back(one);
            }
        
            if(square >= 8 && square <= 15){
                if(board.at(two).type == PieceType::None){
                    moves.push_back(square + 16);
                }
            }
        }
    }
    return moves;
}




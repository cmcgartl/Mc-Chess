#include "eval.h"

int Evaluation::EvaluatePosition(std::array<Piece, 64>& squares, Color color){
        int eval = 0;
        for(size_t i = 0; i < squares.size(); i++){
            int value = getValueForPiece(squares[i]);
            squares[i].color == color ? 
                eval += value : 
                eval -= value;
        }
}

int Evaluation::getValueForPiece(Piece& piece){
    switch(piece.type){
        case PieceType::P:
            return PieceValues::P;
            break;
        case PieceType::N:
            return PieceValues::N;
            break;
        case PieceType::B:
            return PieceValues::B;
            break;
        case PieceType::R:
            return PieceValues::R;
            break;
        case PieceType::Q:
            return PieceValues::Q;
            break;
        case PieceType::K:
            return PieceValues::K;
            break;
    }
}
#include "position.h"
#include "move.h"
#include "direction_constants.h"

DirectionSet getDirections(PieceType type, Color color){
    switch(type) {
        case PieceType::B: 
            return {DiagonalDirections, 4, false};
        case PieceType::R: 
            return {OrthoganalDirections, 4, false};
        case PieceType::Q: 
            return {DiagonalDirections, 4, false};
        case PieceType::N: 
            return {KnightMoves, 8, true};
        case PieceType::P: 
            return color == Color::w
                ? DirectionSet{pawnAttackerDirectionsWhite, 2, true}
                : DirectionSet{pawnAttackerDirectionsBlack, 2, true};
        default: 
            return {nullptr, 0, false};
    }
};
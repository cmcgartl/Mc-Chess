#pragma once
#include "position.h"
#include "move.h"

//return true if square is attacked by an opponent piece
bool isSquareAttacked(int square, Color color, Position& p); //moveGen.cpp

bool squareIntersectsDiagonal(int start, int square, int end);
bool squareIntersectsOrthoganal(int start, int square, int end);
bool squareInvalid(int square);

//walk all squares along a set of directions and perform an action
template<size_t N, typename Func>
void walkDirectionsAndDo(int startSquare, const int (&directions)[N], bool limitedMovementPiece, Func func){
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
#pragma once
#include "position.h"
#include "move.h"

void getValidMovesPawn(int& count, int square, MoveGenResult& result, Position& P);
void getValidMovesKnight(int& count, int square, MoveGenResult& result, Position& P);
void generateDiagonalMoves(int& count, int square, bool cap, Color color, MoveGenResult& result, Position& P);
void generateOrthoganalMoves(int& count, int square, bool cap, Color color, MoveGenResult& result, Position& P);
void generateCheckResolutions(int square, Color color, MoveGenResult& result, Position& P);
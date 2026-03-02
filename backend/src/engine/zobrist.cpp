#include "zobrist.h"

uint64_t Zobrist::pieceKeys[6][2][64];
uint64_t Zobrist::sideToMoveKey;

void Zobrist::init(){
    std::mt19937_64 eng(12345);
    std::uniform_int_distribution<uint64_t> dist;

    for(auto& piece : pieceKeys){
        for(auto& side : piece){
            for(auto& square : side){
                square = dist(eng);
            }
        }
    }

    sideToMoveKey = dist(eng);
}

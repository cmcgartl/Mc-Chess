#include "zobrist.h"

uint64_t Zobrist::pieceKeys[6][2][64];
uint64_t Zobrist::sideToMoveKey;
uint64_t Zobrist::whiteCastleRightsKey;
uint64_t Zobrist::blackCastleRightsKey;
uint64_t Zobrist::enPassantFileKeys[8];

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

    for(auto& key : enPassantFileKeys){
        key = dist(eng);
    }

    sideToMoveKey = dist(eng);
    whiteCastleRightsKey = dist(eng);
    blackCastleRightsKey = dist(eng);
}

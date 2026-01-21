
#include "board.h"

int main(){
    Board gameboard("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
    gameboard.printBoard();
    gameboard.movePiece(8,16);
    gameboard.printBoard();
    gameboard.movePiece(45, 28);
    gameboard.printBoard();
}
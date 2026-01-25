
#include "board.h"
#include "position.h"

int main(){
    Board gameboard("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
    gameboard.printBoard();
    gameboard.movePiece(8,16);
    gameboard.printBoard();
    gameboard.movePiece(45, 28);
    gameboard.printBoard();
    gameboard.printSquareValues();
    Position p;
    bool passedD = p.testDiagonalIntersects();
    bool passedO = p.testOrthoganalIntersects();
    std::cout << "TEST RESULT: " << passedD<< std::endl;
    std::cout << "TEST RESULT: " << passedO<< std::endl;
}

#include "game.h"
#include <iostream>
#include <string>


Game::Game(){
    res = GameResult::InProgress;

    p.setSideToMove(Side::w);
    p.generateAllValidMovesForSide(Side::w);
}

template<typename Func>
void Game::testGameWithInput(std::vector<std::string> moves, Func testFunc){
    for(const auto& move : moves){
        p.makeMove(moveStringToMove(move));
        testFunc();
    }
}

int stringToSquare(std::string_view squareString){
    if(squareString.length() != 2){
        return -1;
    }

    int file = (squareString[0] - 'a');
    int rank = squareString[1] - '0';

    if(file < 0 || file > 7 || rank < 1 || rank > 8){
        return -1;
    }

    return (8 * (8 - rank) + file);
}

Move Game::moveStringToMove(std::string_view MoveString){
    if(MoveString.size() != 5){
        throw std::logic_error("Game:: moveStringToMove: invalid move string");
    }

    std::string_view view(MoveString);
    size_t space_pos = view.find(' ');

    if(space_pos != 2){
        throw std::logic_error("Game:: moveStringToMove: invalid move string");
    }
    std::string_view sub1 = view.substr(0, space_pos);
    std::string_view sub2 = view.substr(space_pos + 1);

    return Move(stringToSquare(sub1), stringToSquare(sub2));
}

void Game::run(){
    while(res == GameResult::InProgress){

        p.getBoard().printBoard();
        std::cout << '\n';

        std::string input;

        std::cout << "enter your move (c3 f3)" << '\n';
        std::getline(std::cin, input);


        p.makeMove(moveStringToMove(input));
        
    }
}

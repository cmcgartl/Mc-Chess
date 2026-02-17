#include "board.h"
#include <string>
#include <cctype>

//Board implementation:

//Supports default board constructor, and board constructor from FEN string

Board::Board(){
    clearBoard();
}

void Board::clearBoard(){
    squares.fill(Piece());
}


void Board::printBoard() const{
    for(int row = 0; row < 8; row++){
        std::cout << "---------------------------------" << std::endl;
        for(int col = 0; col < 8; col++){ 
            const Piece& p = squares[row * 8 + col];
            char c = '.';
            switch (p.type){
                    case PieceType::P:   c = 'P'; break;
                    case PieceType::N: c = 'N'; break;
                    case PieceType::B: c = 'B'; break;
                    case PieceType::R:   c = 'R'; break;
                    case PieceType::Q:  c = 'Q'; break;
                    case PieceType::K:   c = 'K'; break;
                    case PieceType::None:   c = '.'; break;
            }
            std::cout << "| " << c << " ";
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "-------------------------------" << std::endl;
}

//constructor from piece location information in FEN string
Board::Board(const std::string& FEN){
    int square = 0;                                         //initialize to starting square
    int FENIndex = 0;                                       //initialize to index zero inf FEN string
    for(int i = 0; i < 8; i++){                             //repeat for 8 ranks in FEN
        char delim = i < 7? '/' : ' ';                      //determine next expected delimiter
        size_t delim_pos = FEN.find(delim, FENIndex + 1);   //find location of delimiter
        if(delim == ' ' && delim_pos == std::string::npos){
            delim_pos = FEN.size();
        }
        for(size_t j = FENIndex; j < delim_pos; j++){          //repeat from current index to delimiter index
            if(isValidLetter(FEN[j])){                      //if character is a valid letter, create a piece
                Color color = isupper(FEN[j]) ? Color::w : Color::b;
                PieceType type = charToPieceType(FEN[j]);
                squares[square] = Piece(type, color);

                //if the piece is a king, save the king's location
                if(type == PieceType::K){
                    color == Color::w ? whiteKingSquare = square : blackKingSquare = square;
                }

                square++;
                FENIndex++;
            }
            else if(isNum(FEN[j])){                         //if character is a number, create that many blank spaces
                for(int k = 0; k < FEN[j] - '0'; k++){
                    squares[square] = Piece();
                    square++;;
                }
                FENIndex++;
            }
        }
        FENIndex++;
    }
}

bool Board::isValidLetter(char c){
    switch(toupper(static_cast<unsigned char>(c))){
        case 'P': case 'N': case 'B': 
        case 'R': case 'K': case 'Q': 
            return true;
        default: 
            return false;
    }
}

PieceType Board::charToPieceType(char c){
    switch (std::toupper(static_cast<unsigned char>(c))){
        case 'P': return PieceType::P;
        case 'N': return PieceType::N;
        case 'B': return PieceType::B;
        case 'R': return PieceType::R;
        case 'K': return PieceType::K;
        case 'Q': return PieceType::Q;
        default: return PieceType::None;
    }
}

bool Board::isNum(char c){
    return c >= '0' && c <= '9';
}

//naive move piece function: will move a piece from any valid starting location
//to any valid destination location, whether or not that move is valid
void Board::movePiece(int src, int dest){
    if(src < 0 || src >= 64 || dest < 0 ||  dest >= 64){
        throw std::out_of_range("Board::movePiece: index out of range");
    }
    if(squares[src].type == PieceType::None){
        throw std::logic_error("Board::movePiece: no piece at source square");
    }

    squares[dest] = squares[src];
    squares[src] = Piece{};
}

Piece& Board::at(int square){
    if(square >= 0 && square <= 63){
        return squares[square];
    }
    else{
        throw std::out_of_range("Board::at: index out of range");
    }
}

void Board::printSquareValues() const{
        for(int row = 0; row < 8; row++){
        std::cout << "---------------------------------" << std::endl;
        for(int col = 0; col < 8; col++){ 
            std::cout << "| " << row * 8 + col << " ";
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "-------------------------------" << std::endl;
}
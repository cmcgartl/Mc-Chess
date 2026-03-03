#include "position.h"
#include "move.h"
#include "direction_constants.h"
#include "move_gen_helpers.h"
#include "piece_movement.h"
#include "zobrist.h"

void Position::generatePieceLists(){
    whitePieces.clear();
    blackPieces.clear();
    for(int i = 0; i < 64; i++){
        Piece p = board.at(i);
        if(p.type == PieceType::None) continue;
        if(p.color == Color::w){
            whitePieces.push_back(p);
        }
        else{
            blackPieces.push_back(p);
        }
    }
}

MoveGenResult Position::generateAllValidMovesForSide(Side side){
    MoveGenResult result;
    Side opponentSide = side == Side::w ? Side::b : Side::w;
    Color opponentColor = opponentSide == Side::w ? Color::w : Color::b;
    Color color = (side == Side::w) ? Color::w : Color::b;

    for(int i = 0; i < 64; i++){
        if(board.at(i).color == opponentColor){
            generateValidMoves(i, result);
        }
    }

    // Clear Move data and leave the attack and pin information
    result.moves.clear();
    result.startIndices.fill(-1);
    result.counts.fill(0);

    // Phase 2: Generate current side's legal moves
    bool inCheck = false;
    if((color ==Color::w && !result.attacksOnWhiteKing.empty()) || (color == Color::b && !result.attacksOnBlackKing.empty())){
        inCheck = true;
    }

    for(int i = 0; i < 64; i++){
        if(board.at(i).color == color){
            if(inCheck){
                generateCheckResolutions(i, color, result, *this);
            }
            else{
                generateValidMoves(i, result);
            }
        }
    }

    // Determine position status
    if(result.moves.empty()){
        if(inCheck){
            result.status = side == Side::w ? PositionStatus::CheckMateBlack : PositionStatus::CheckMateWhite;
        }
        else{
            result.status = PositionStatus::Stalemate;
        } 
    }

    return result;
}


void Position::generateValidMoves(int square, MoveGenResult& result){
    if(square < 0 || square > 63){
        throw std::out_of_range("Position::generateValidMoves: provided square is out of range");
    }

    Piece p = board.at(square);
    if(p.type == PieceType::None) return;
    Color pColor = p.color;

    int moveIndex = result.moves.size();
    int count = 0;

    //bool canCastle = true;
    auto castleLambda = [this, &pColor, &square, &count, &result](int sq){
        if((sq % 8) == 0){
            Piece rook = board.at(sq);
            if(rook.type == PieceType::R && rook.color == (pColor == Color::w ? Color::w : Color::b)){
                result.moves.push_back(Move(square, sq + 2, MoveType::CastleLong));
                count++;
            }
            return true;
        }
        if((sq % 8) == 7){
            Piece rook = board.at(sq);
            if(rook.type == PieceType::R && rook.color == (pColor == Color::w ? Color::w : Color::b)){
                result.moves.push_back(Move(square, sq - 1, MoveType::CastleShort));
                count++;
            }
            return true;
        }

        if(board.at(sq).type != PieceType::None){
            return true;
        }
        if((std::abs(sq - square) <= 2) && isSquareAttacked(sq, pColor, *this)){
            return true;
        }
        return false;
    };


    if(pColor == Color::w && p.type == PieceType::K){
        if(!kingHasMovedWhite && result.attacksOnWhiteKing.size() == 0){
            if(!rookLeftHasMovedWhite){
                 walkDirectionsAndDo(square, {-1}, false, castleLambda);
            }
            if(!rookRightHasMovedWhite){
                 walkDirectionsAndDo(square, {1}, false, castleLambda);
            }
        }
    }
    else if (pColor == Color::b && p.type == PieceType::K){
        if(!kingHasMovedBlack && result.attacksOnBlackKing.size() == 0){
            if(!rookLeftHasMovedBlack){
                 walkDirectionsAndDo(square, {-1}, false, castleLambda);
            }
            if(!rookRightHasMovedBlack){
                 walkDirectionsAndDo(square, {1}, false, castleLambda);
            }
        }
    }

    const PinInfo& pin = result.pins[square];
    switch(p.type){
        case PieceType::P:
            getValidMovesPawn(count, square, result, *this);
            break;
        case PieceType::B:
            if(!pin.pinnedO){
                generateDiagonalMoves(count, square, false, pColor, result, *this);
            }
            break;
        case PieceType::N:
            if(!pin.pinnedO && !pin.pinnedD){
                getValidMovesKnight(count, square, result, *this);
            }
            break;
        case PieceType::R:
            if(!pin.pinnedD){
                generateOrthoganalMoves(count, square, false, pColor, result, *this);
            }
            break;
        case PieceType::Q:
            if(!pin.pinnedD){
                generateOrthoganalMoves(count, square, false, pColor, result, *this);
            }
            if(!pin.pinnedO){
                generateDiagonalMoves(count, square, false, pColor, result, *this);
            }
            break;
        case PieceType::K:
            generateDiagonalMoves(count, square, true, pColor, result, *this);
            generateOrthoganalMoves(count, square, true, pColor, result, *this);
            break;
        default:
            break;
    }

    if(count > 0){
        result.startIndices[square] = moveIndex;
        result.counts[square] = count;
    }
}

bool Position::makeMove(const Move& move, const MoveGenResult& currentMoves){
    int start = currentMoves.startIndices[move.from];
    int count = currentMoves.counts[move.from];
    if(start < 0 || count <= 0 || move.to == move.from) return false;

    const Move* matchedMove = nullptr;
    for(int i = start; i < start + count; i++){
        if(currentMoves.moves[i] == move){
            matchedMove = &currentMoves.moves[i];
        }
    }

    if(matchedMove == nullptr){
        return false;
    }

    applyMove(*matchedMove);
    generatePieceLists();

    return true;
}

uint64_t Position::computeHash() const {
    uint64_t h = 0;
    for(int i = 0; i < 64; i++){
        if(board.at(i).type != PieceType::None){
            int pieceIdx = static_cast<int>(board.at(i).type);
            int colorIdx = static_cast<int>(board.at(i).color);
            h ^= Zobrist::pieceKeys[pieceIdx][colorIdx][i];
        }
    }
    h ^= Zobrist::blackCastleRightsKey;
    h ^= Zobrist::whiteCastleRightsKey;
    h ^= Zobrist::enPassantFileKeys[enPassantSquare];

    if(sideToMove == Side::b){
        h ^= Zobrist::sideToMoveKey;
    }
    return h;
}

std::string Position::toFEN() const {
    std::string fen = board.toFEN();
    fen += (sideToMove == Side::w) ? " w" : " b";
    fen += " - - 0 1";
    return fen;
}

UndoInfo::UndoInfo(const Move& move, const Position& P) :
    capturedPiece(move.type == MoveType::EP 
        ? move.from > move.to  
            ? P.board.at(move.to + 8) 
            : P.board.at(move.to - 8)
        : P.board.at(move.to)),
    whiteKingSquare(P.board.whiteKingSquare),
    blackKingSquare(P.board.blackKingSquare),
    sideToMove(P.sideToMove),
    kingHasMovedWhite(P.kingHasMovedWhite),
    kingHasMovedBlack(P.kingHasMovedBlack),
    rookLeftHasMovedWhite(P.rookLeftHasMovedWhite),
    rookRightHasMovedWhite(P.rookRightHasMovedWhite),
    rookLeftHasMovedBlack(P.rookLeftHasMovedBlack),
    rookRightHasMovedBlack(P.rookRightHasMovedBlack),
    enPassantSquare(P.enPassantSquare),
    hash(P.hash)
{}

//function to apply a move during minimax, need to store the current move state so we can undo
UndoInfo Position::applyMove(const Move& move){
    UndoInfo undo(move, *this);

    //remove our piece from start square
    //xor our current hash with zobrist[the moving piece's index][the color index,][destination square index]

    Piece p = board.at(move.from);
    if(p.color == Color::w){
        if(!undo.kingHasMovedWhite){
            if(p.type == PieceType::K){
                hash ^= Zobrist::whiteCastleRightsKey;
                kingHasMovedWhite = true;
            }
            if(!undo.rookLeftHasMovedWhite){
                if(p.type == PieceType::R && (move.from % 8) == 0){
                    hash ^= Zobrist::whiteCastleRightsKey;
                    rookLeftHasMovedWhite = true;
                }
            }
            if(!undo.rookRightHasMovedWhite){
                if(p.type == PieceType::R && (move.from % 8) == 7){
                    hash ^= Zobrist::whiteCastleRightsKey;
                    rookRightHasMovedWhite = true;
                }
            }
        }
    }

    if(p.color == Color::b){
        if(!undo.kingHasMovedBlack){
            if(p.type == PieceType::K){
                hash ^= Zobrist::blackCastleRightsKey;
                kingHasMovedBlack = true;
            }
            if(!undo.rookLeftHasMovedBlack){
                if(p.type == PieceType::R && (move.from % 8) == 0){
                    hash ^= Zobrist::blackCastleRightsKey;
                    rookLeftHasMovedBlack = true;
                }
            }
            if(!undo.rookRightHasMovedBlack){
                if(p.type == PieceType::R && (move.from % 8) == 7){
                    hash ^= Zobrist::blackCastleRightsKey;
                    rookRightHasMovedBlack = true;
                }
            }
        }
    }
    
    sideToMove = (sideToMove == Side::w) ? Side::b : Side::w;

    //if enpassant square is set, xor it from hash
    if(enPassantSquare >= 0){
        hash ^= Zobrist::enPassantFileKeys[enPassantSquare];
    }

    //if we made a pawn double move, mark the square behind it as EP
    if(move.type == MoveType::PawnDouble){
        enPassantSquare = (move.from + move.to) / 2;
        hash ^= Zobrist::enPassantFileKeys[enPassantSquare];
    }

    //otherwise mark position as having no en passant
    else{
        enPassantSquare = -1;
    }
    board.movePiece(move, hash);
    hash ^= Zobrist::sideToMoveKey;

    return undo;
}

void Position::undoMove(const Move& move, const UndoInfo& undo){
    auto& squares = board.getSquares();
    //reverse the move
    if(move.type == MoveType::CastleLong){
        Move undoKingMove(move.to, move.from);
        Move undoRookMove(move.to + 1, move.to - 2);
        board.movePiece(undoKingMove, hash);
        board.movePiece(undoRookMove, hash);
    }
    
    else if(move.type == MoveType::CastleShort){
        Move undoKingMove(move.to, move.from);
        Move undoRookMove(move.to - 1, move.to + 1);
        board.movePiece(undoKingMove, hash);
        board.movePiece(undoRookMove, hash);
    }
 
    else if(move.type == MoveType::EP){
        squares[move.from] = squares[move.to];
        squares[move.to] = Piece{};
        int capturedSquare = move.from > move.to ? move.to + 8 : move.to - 8;
        squares[capturedSquare] = undo.capturedPiece;
    }
    else{
        squares[move.from] = squares[move.to];
        squares[move.to] = undo.capturedPiece;
        if(move.type == MoveType::P){
            squares[move.from].type = PieceType::P;
        }
    }

    // Restore all saved state
    board.whiteKingSquare = undo.whiteKingSquare;
    board.blackKingSquare = undo.blackKingSquare;
    sideToMove = undo.sideToMove;
    kingHasMovedWhite = undo.kingHasMovedWhite;
    kingHasMovedBlack = undo.kingHasMovedBlack;
    rookLeftHasMovedWhite = undo.rookLeftHasMovedWhite;
    rookRightHasMovedWhite = undo.rookRightHasMovedWhite;
    rookLeftHasMovedBlack = undo.rookLeftHasMovedBlack;
    rookRightHasMovedBlack = undo.rookRightHasMovedBlack;
    enPassantSquare = undo.enPassantSquare;
    hash = undo.hash;
}

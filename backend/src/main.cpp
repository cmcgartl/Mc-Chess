
#include "board.h"
#include "game.h"
#include "position.h"
#include "crow.h"
#include "nlohmann/json.hpp"

int main(){
    crow::SimpleApp app;

    Game game;

    CROW_ROUTE(app, "/hello")
    ([](){
        return "Hello from Crow!";
    });

    CROW_ROUTE(app, "/startGame")
    ([&game](){

        //get the FEN representation of the position
        Position& p = game.getPosition();
        std::string FEN = p.getBoard().toFEN();

        //initialize JSON components
        nlohmann::json response;
        nlohmann::json legalMoves = nlohmann::json::object();
        response["FEN"] = FEN;
        response["turn"] = "w";
        
        //fill legal moves with string representations
        for(const auto& move : game.getPosition().getPossibleMoves()){
            std::string from = game.squareToAlgebraic(move.from);
            std::string to = game.squareToAlgebraic(move.to);
            legalMoves[from].push_back(to);
        }

        response["legalMoves"] = legalMoves;

        //serialize JSON object into JSON string
        crow::response res(200, response.dump());

        //set header to indicate it is transferring JSON formatted data
        res.set_header("Content-Type", "application/json");
        return res;
    });

    app.port(18080).run();


}

#include "board.h"
#include "game.h"
#include "position.h"
#include "zobrist.h"
#include "crow.h"
#include "crow/middlewares/cors.h"
#include "nlohmann/json.hpp"

crow::response retrieveGameState(Game& game);

int main(){
    Zobrist::init();
    crow::App<crow::CORSHandler> app;
    auto& cors = app.get_middleware<crow::CORSHandler>();
    cors.global()
        .origin("http://localhost:5173")
        .methods("GET"_method, "POST"_method)
        .headers("Content-Type");

    Game game;

    CROW_ROUTE(app, "/hello")
    ([](){
        return "Hello from Crow!";
    });

    CROW_ROUTE(app, "/startGame").methods(crow::HTTPMethod::GET)
    ([&game](){
        return retrieveGameState(game);
    });

    CROW_ROUTE(app, "/makeMove").methods(crow::HTTPMethod::POST)
    ([&game](const crow::request& req){
        auto body = crow::json::load(req.body);

        std::string from = body["from"].s();
        std::string to = body["to"].s();
        if(game.makeMove(from, to)){
            // If engine should respond, compute and apply its move
            if (game.isEngineTurn()) {
                auto bestMove = game.getEngineBestMove();
                if (bestMove.has_value()) {
                    std::string engineFrom = game.squareToAlgebraic(bestMove->from);
                    std::string engineTo = game.squareToAlgebraic(bestMove->to);
                    game.makeMove(engineFrom, engineTo);
                }
            }
            return retrieveGameState(game);
        }

        return crow::response(400, "Invalid Move");

    });

    CROW_ROUTE(app, "/setEngine").methods(crow::HTTPMethod::POST)
    ([&game](const crow::request& req){
        auto body = crow::json::load(req.body);

        std::string side = body["side"].s();
        if (side == "white") game.setEngineMode(EngineMode::White);
        else if (side == "black") game.setEngineMode(EngineMode::Black);
        else game.setEngineMode(EngineMode::Off);

        if (body.has("depth")) {
            int depth = static_cast<int>(body["depth"].i());
            if (depth >= 1 && depth <= 8) {
                game.setSearchDepth(depth);
            }
        }

        // If it's now the engine's turn, compute and apply move immediately
        if (game.isEngineTurn()) {
            auto bestMove = game.getEngineBestMove();
            if (bestMove.has_value()) {
                std::string engineFrom = game.squareToAlgebraic(bestMove->from);
                std::string engineTo = game.squareToAlgebraic(bestMove->to);
                game.makeMove(engineFrom, engineTo);
            }
        }

        return retrieveGameState(game);
    });

    CROW_ROUTE(app, "/reset").methods(crow::HTTPMethod::POST)
    ([&game](){
        game = Game();

        return retrieveGameState(game);
    });


    app.port(18080).run();
}

crow::response retrieveGameState(Game& game){
        Position& p = game.getPosition();
        std::string FEN = p.getBoard().toFEN();

        //initialize JSON components
        nlohmann::json response;
        nlohmann::json legalMoves = nlohmann::json::object();
        response["FEN"] = FEN;
        response["turn"] = (p.getSideToMove() == Side::w) ? "w" : "b";
        response["status"] = game.getResultString();
        response["engineTurn"] = game.isEngineTurn();

        //fill legal moves with string representations
        for(const auto& move : game.getCurrentMoves().moves){
            std::string from = game.squareToAlgebraic(move.from);
            std::string to = game.squareToAlgebraic(move.to);
            legalMoves[from].push_back(to);
        }

        response["legalMoves"] = legalMoves;

        crow::response res(200,response.dump());
        res.set_header("Content-Type", "application/json");

        return res;
    };
#include <nlohmann/json.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include "chessboard.h"
#include <vector>
#include <map>
#include <sol/sol.hpp>
#include <filesystem>

typedef websocketpp::server<websocketpp::config::asio> server;
using json = nlohmann::json;

struct luaRoomState {
    std::string name;
    sol::state lua;
    Chessboard chessboard{8, 8};
};

std::map<
    websocketpp::connection_hdl,
    luaRoomState,
    std::owner_less<websocketpp::connection_hdl>> games;


void setup_lua_api(sol::state& lua, Chessboard& chessboard) {
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table, sol::lib::string, sol::lib::math); 

    lua.set_function("isOccupied", [&](int row, int col) {
        return chessboard.isOccupied(row, col);
    });

    lua.set_function("addMove", [&](Piece& p, int row, int col) {
        p.possibleMoves.push_back({row, col});
    });

    lua.set_function("addTake", [&](Piece& p, int row, int col) {
        p.possibleTakes.push_back({row, col});
    });

    lua.new_usertype<Chessboard>("Chessboard",
        "isOccupied", &Chessboard::isOccupied,
        "getPieceAt", &Chessboard::getPieceAt,
        "setPieceAt", &Chessboard::setPieceAt,
        "movePiece", &Chessboard::movePiece,
        "rows", sol::property(&Chessboard::getRows),
        "cols", sol::property(&Chessboard::getCols)
    );

    lua.new_usertype<Piece>("Piece",
        "type", &Piece::type,
        "image", &Piece::image,
        "canJumpOverPieces", &Piece::canJumpOverPieces,
        "possibleMoves", &Piece::possibleMoves,
        "possibleTakes", &Piece::possibleTakes
    );

    lua.new_usertype<Move>("Move",
        "dx", &Move::dx,
        "dy", &Move::dy,
        "repeat", &Move::repeat,
        "basedOnLastMove", &Move::basedOnLastMove
    );

}

void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg){
    std::string payload = msg->get_payload();
    json j = json::parse(payload);
    std::string type = j["type"];
    

    if (type == "ChessboardState") {
        Chessboard& chessboard = games[hdl].chessboard; 

        std::filesystem::path scriptPath = std::filesystem::current_path() / "backend" / "lua" / "isOccupiedTest.lua";
        sol::state& lua = games[hdl].lua;

        lua.script_file(scriptPath);
        sol::protected_function f = lua["getLegalMoves"];

        if (f.valid()) {
            sol::protected_function_result res = f(chessboard);
            if (!res.valid()) {
                sol::error err = res;
                std::cerr << "Error calling Lua function: " << err.what() << std::endl;
            }
        } 

        json response;
        chessboard.to_json(response);
        s->send(hdl, response.dump(), msg->get_opcode());
        return;
    }
    else if (type == "LoadGame") {
        sol::state& lua = games[hdl].lua;
        Chessboard& chessboard = games[hdl].chessboard;

        chessboard.setPieceAt(0, 0, Piece{ {0, 0}, "rook", "Chess_rlt45.svg", { {1, 0, true}, {0, 1, true}, {-1, 0, true}, {0, -1, true} }, { }, false });

        setup_lua_api(lua, chessboard);

        json response;
        chessboard.to_json(response);
        s->send(hdl, response.dump(), msg->get_opcode());
        return;
    }

    s->send(hdl, "{big working:'wow'}", msg->get_opcode());
}
void on_open(websocketpp::connection_hdl hdl){
   sol::state lua;
   games[hdl] = luaRoomState{ "default", std::move(lua) };
}

void on_close(websocketpp::connection_hdl hdl){
    games.erase(hdl);
}


int main() {
    server chessServer;
    try {
        chessServer.set_access_channels(websocketpp::log::alevel::all);
        chessServer.clear_access_channels(websocketpp::log::alevel::frame_payload);

        chessServer.set_pong_timeout(10000);

        chessServer.set_pong_handler([](websocketpp::connection_hdl, std::string) {
            return true;
        });

        chessServer.init_asio();
        chessServer.set_http_handler([&chessServer](websocketpp::connection_hdl hdl) {
            auto con = chessServer.get_con_from_hdl(hdl);
            con->append_header("Access-Control-Allow-Origin", "*");
        });

        chessServer.set_message_handler(bind(&on_message, &chessServer, std::placeholders::_1, std::placeholders::_2));
        // chessServer.set_open_handler(bind(&on_open, std::placeholders::_1));
        // chessServer.set_close_handler(bind(&on_close, std::placeholders::_1));
        chessServer.set_reuse_addr(true);
        chessServer.listen(9002);
        chessServer.start_accept();

        chessServer.run();
    } catch (websocketpp::exception const & e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

}


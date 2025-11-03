#include <nlohmann/json.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include "chessboard.h"
#include <vector>
#include <map>
#include <sol/sol.hpp>
#include <filesystem>
#include "luaController.h"

typedef websocketpp::server<websocketpp::config::asio> server;
using json = nlohmann::json;

std::map<
    websocketpp::connection_hdl,
    luaRoomState,
    std::owner_less<websocketpp::connection_hdl>> games;


void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg){
    std::string payload = msg->get_payload();
    json j = json::parse(payload);
    std::string type = j["type"];
    
    if (!j.contains("payload") || !j["payload"].is_object()) {
        json response;
        response["type"] = "Error";
        response["payload"]["message"] = "Missing or invalid payload";
        s->send(hdl, response.dump(), msg->get_opcode());
        return;
    }

    if (type == "ChessboardState") {
        Chessboard& chessboard = games[hdl].chessboard; 

        std::filesystem::path scriptPath = std::filesystem::current_path() / "backend" / "lua" / "regularChess.lua";
        sol::state& lua = games[hdl].lua;


        int fromRow = j["payload"]["from"]["row"];
        int fromCol = j["payload"]["from"]["col"];
        int toRow = j["payload"]["to"]["row"];
        int toCol = j["payload"]["to"]["col"];
        chessboard.movePiece(fromRow, fromCol, toRow, toCol);


        lua.script_file(scriptPath);
        sol::protected_function f = lua["getLegalMoves"];

        if (f.valid()) {
            sol::protected_function_result res = f(chessboard);
            if (!res.valid()) {
                sol::error err = res;
                std::cerr << "Error calling Lua function: " << err.what() << std::endl;
            }
        } 
        chessboard.calculateRepeatMoves();
        json response;
        chessboard.to_json(response);
        s->send(hdl, response.dump(), msg->get_opcode());
        return;
    }

    else if (type == "LoadGame") {
        sol::state& lua = games[hdl].lua;
        Chessboard& chessboard = games[hdl].chessboard;

        setup_lua_api(lua, chessboard);

        json response;
        chessboard.to_json(response);
        s->send(hdl, response.dump(), msg->get_opcode());
        return;
    }
    else if (type == "createRoom") {

        if (!j["payload"].contains("roomName") || !j["payload"]["roomName"].is_string()) {
            std::cout << "Invalid room creation request" << std::endl;
            json response;
            response["type"] = "Error";
            response["payload"]["message"] = "Missing or invalid roomName";
            s->send(hdl, response.dump(), msg->get_opcode());
            return;
        }
        std::string roomName = j["payload"]["roomName"];
        games[hdl].roomName = roomName;

        std::cout << "Room created: " << roomName << std::endl;
        json response;
        s->send(hdl, response.dump(), msg->get_opcode());
        return;
    }
    else if (type == "joinRoom") 
    {
        std::string roomName = j["payload"]["roomName"];
        std::string playerName = j["payload"]["playerName"];
        games[hdl].roomName = roomName;

        if (games[hdl].players[0].empty()) {
            games[hdl].players[0] = playerName;
        } else if (games[hdl].players[1].empty()) {
            games[hdl].players[1] = playerName;
        } else {
            json response;
            response["type"] = "Error";
            response["payload"]["message"] = "Room is full";
            s->send(hdl, response.dump(), msg->get_opcode());
            return;
        }

        json response;
        response["type"] = "RoomJoined";
        response["payload"]["roomName"] = roomName;
        s->send(hdl, response.dump(), msg->get_opcode());
        return;
    }
    else if (type == "listRooms") {
        std::vector<std::string> roomList;
        for (const auto& pair : games) {
            roomList.push_back(pair.second.roomName);
        }

        json response;
        response["type"] = "RoomList";
        response["payload"]["rooms"] = roomList;
        s->send(hdl, response.dump(), msg->get_opcode());
        return;
    }

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
        chessServer.set_open_handler(bind(&on_open, std::placeholders::_1));
        chessServer.set_close_handler(bind(&on_close, std::placeholders::_1));
        chessServer.set_reuse_addr(true);
        chessServer.listen(9002);
        chessServer.start_accept();

        chessServer.run();
    } catch (websocketpp::exception const & e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

}


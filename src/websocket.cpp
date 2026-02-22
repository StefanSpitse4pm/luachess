#include "Chess/chessboard.h"
#include "Handlers/Games/GameHandler.h"
#include "Handlers/Rooms/Player.h"
#include "Handlers/Rooms/RoomHandler.h"

#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>
#include <vector>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
using json = nlohmann::json;
std::map<websocketpp::connection_hdl, luaRoomState, std::owner_less<websocketpp::connection_hdl>> games;
std::map<websocketpp::connection_hdl, std::unique_ptr<Player>, std::owner_less<websocketpp::connection_hdl>> players;
RoomHandler roomHandler;
GameHandler gameHandler;

void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg)
{
    std::string payload = msg->get_payload();
    json j = json::parse(payload);
    std::string type = j["type"];

    ActionContext ctx;
    ctx.action = j["payload"]["action"];
    ctx.serverPtr = s;
    ctx.sessionContext.hdl = hdl;
    if (!j.contains("payload") || !j["payload"].is_object())
    {
        json response;
        response["type"] = "Error";
        response["payload"]["message"] = "Missing or invalid payload";
        s->send(hdl, response.dump(), msg->get_opcode());
        return;
    }

    // if (type == "ChessboardState")
    // {
    //     Chessboard& chessboard = games[hdl].chessboard;
    //
    //     std::filesystem::path scriptPath = std::filesystem::current_path() / "lua" / "regularChess.lua";
    //     sol::state& lua = games[hdl].lua;
    //
    //     int fromRow = j["payload"]["from"]["row"];
    //     int fromCol = j["payload"]["from"]["col"];
    //     int toRow = j["payload"]["to"]["row"];
    //     int toCol = j["payload"]["to"]["col"];
    //     chessboard.movePiece(fromRow, fromCol, toRow, toCol);
    //     lua.script_file(scriptPath);
    //     sol::protected_function f = lua["getLegalMoves"];
    //     if (f.valid())
    //     {
    //         sol::protected_function_result res = f(chessboard);
    //         if (!res.valid())
    //         {
    //             sol::error err = res;
    //             std::cerr << "Error calling Lua function: " << err.what() << std::endl;
    //         }
    //     }
    //     chessboard.calculateRepeatMoves();
    //     json response = chessboard.to_json();
    //     s->send(hdl, response.dump(), msg->get_opcode());
    //     return;
    // }

    if (type == "Game")
    {
        if (j["payload"].contains("gameType"))
        {
            ctx.gameContext.gameType = j["payload"]["gameType"];
        }

        if (j["payload"].contains("move"))
        {
            ctx.gameContext.send->fromCol = j["payload"]["move"]["from"]["col"];
            ctx.gameContext.send->fromRow = j["payload"]["move"]["from"]["row"];
            ctx.gameContext.send->toCol = j["payload"]["move"]["to"]["col"];
            ctx.gameContext.send->toRow = j["payload"]["move"]["to"]["row"];
        }

        gameHandler.router(ctx.action, ctx);
        return;
    }

    if (type == "Room")
    {

        if (j["payload"].contains("roomName"))
        {
            ctx.roomContext.desiredRoomName = j["payload"]["roomName"];
        }

        if (j["payload"].contains("username"))
        {
            std::string username = j["payload"]["username"];
            // Create or reuse a Player for this connection
            if (players.find(hdl) == players.end() || players[hdl]->get_username() != username)
            {
                players[hdl] = std::make_unique<Player>(username);
            }
            ctx.sessionContext.player = players[hdl].get();
        }

        roomHandler.router(ctx.action, ctx);
    }
}

void on_open(const websocketpp::connection_hdl& hdl)
{
    sol::state lua;
    games[hdl] = luaRoomState{std::move(lua)};
}

void on_close(const websocketpp::connection_hdl& hdl)
{
    games.erase(hdl);
    players.erase(hdl);
}

int main()
{
    server chessServer;
    try
    {
        chessServer.set_access_channels(websocketpp::log::alevel::all);
        chessServer.clear_access_channels(websocketpp::log::alevel::frame_payload);

        chessServer.set_pong_timeout(10000);

        chessServer.set_pong_handler([](websocketpp::connection_hdl, std::string) { return true; });

        chessServer.init_asio();
        chessServer.set_http_handler(
            [&chessServer](websocketpp::connection_hdl hdl)
            {
                auto con = chessServer.get_con_from_hdl(hdl);
                con->append_header("Access-Control-Allow-Origin", "*");
            }
        );

        chessServer.set_message_handler(bind(&on_message, &chessServer, std::placeholders::_1, std::placeholders::_2));
        chessServer.set_open_handler(bind(&on_open, std::placeholders::_1));
        chessServer.set_close_handler(bind(&on_close, std::placeholders::_1));
        chessServer.set_reuse_addr(true);
        chessServer.listen(9002);
        chessServer.start_accept();

        chessServer.run();
    }
    catch (websocketpp::exception const& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

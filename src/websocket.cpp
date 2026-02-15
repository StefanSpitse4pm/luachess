#include "Handlers/Games/GameHandler.h"
#include "Handlers/Rooms/RoomHandler.h"
#include "chessboard.h"

#include <filesystem>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>
#include <vector>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
using json = nlohmann::json;

std::map<websocketpp::connection_hdl, luaRoomState, std::owner_less<websocketpp::connection_hdl>> games;
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
    ctx.userContext.hdl = hdl;
    if (!j.contains("payload") || !j["payload"].is_object())
    {
        json response;
        response["type"] = "Error";
        response["payload"]["message"] = "Missing or invalid payload";
        s->send(hdl, response.dump(), msg->get_opcode());
        return;
    }

    if (type == "ChessboardState")
    {
        Chessboard& chessboard = games[hdl].chessboard;

        std::filesystem::path scriptPath = std::filesystem::current_path() / "lua" / "regularChess.lua";
        sol::state& lua = games[hdl].lua;

        int fromRow = j["payload"]["from"]["row"];
        int fromCol = j["payload"]["from"]["col"];
        int toRow = j["payload"]["to"]["row"];
        int toCol = j["payload"]["to"]["col"];
        chessboard.movePiece(fromRow, fromCol, toRow, toCol); lua.script_file(scriptPath); sol::protected_function f = lua["getLegalMoves"];
        if (f.valid())
        {
            sol::protected_function_result res = f(chessboard);
            if (!res.valid())
            {
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

    if (type == "Game")
    {

        // sol::state& lua = games[hdl].lua;
        // Chessboard& chessboard = games[hdl].chessboard;
        //
        // setup_lua_api(lua, chessboard);
        //
        // json response;
        // chessboard.to_json(response);
        // s->send(hdl, response.dump(), msg->get_opcode());
        // return;
    }

    if (type == "Room")
    {
        if (j["payload"].contains("username"))
        {
            ctx.userContext.username = j["payload"]["username"];
        }
        else
        {
            s->send(hdl, R"({"type": "Error", "payload": {"message": "Missing username"}})", msg->get_opcode());
            return;
        }

        if (j["payload"].contains("roomName"))
        {
            ctx.roomContext.roomName = j["payload"]["roomName"];
        }
        else
        {
            s->send(hdl, R"({"type": "Error", "payload": {"message": "Missing roomName"}})", msg->get_opcode());
            return;
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

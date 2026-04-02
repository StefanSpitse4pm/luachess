#include "Chess/chessboard.h"
#include "Handlers/Games/GameHandler.h"
#include "Handlers/Rooms/RoomHandler.h"
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>
#include <vector>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
using json = nlohmann::json;
RoomHandler roomHandler;
GameHandler gameHandler(roomHandler);

template <std::ranges::range R>
    requires std::same_as<std::ranges::range_value_t<R>, SessionContext>
void notify(const R& sessions, const std::string& message, server* serverPtr)
{
    for (const auto& session : sessions)
    {
        try
        {
            serverPtr->send(session.hdl, message, websocketpp::frame::opcode::text);
        }
        catch (const websocketpp::exception& e)
        {
            std::cerr << "Failed to send notification to a session: " << e.what() << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to send notification (std::exception): " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Failed to send notification (unknown error)" << std::endl;
        }
    }
}

void on_message(server* s, const websocketpp::connection_hdl& hdl, const server::message_ptr& msg)
{
    std::string payload = msg->get_payload();
    json j = json::parse(payload);
    const std::string type = j["type"];

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
    if (type == "Game")
    {
        if (j["payload"].contains("gameType"))
        {
            ctx.gameContext.gameType = j["payload"]["gameType"];
        }

        if (j["payload"].contains("roomName"))
        {
            ctx.roomContext.desiredRoomName = j["payload"]["roomName"];
        }

        if (j["payload"].contains("gameId"))
        {
            const auto& rawGameId = j["payload"]["gameId"];
            if (rawGameId.is_number_unsigned())
            {
                ctx.gameContext.gameId = rawGameId.get<std::uint32_t>();
            }
            else if (rawGameId.is_number_integer())
            {
                const auto v = rawGameId.get<std::int64_t>();
                if (v >= 0)
                {
                    ctx.gameContext.gameId = static_cast<std::uint32_t>(v);
                }
            }
            else if (rawGameId.is_string())
            {
                ctx.gameContext.gameId = static_cast<std::uint32_t>(std::stoul(rawGameId.get<std::string>()));
            }
        }

        if (j["payload"].contains("move"))
        {
            if (const auto& move = j["payload"]["move"]; move.is_object() && move.contains("fromRow") &&
                                                         move.contains("fromCol") && move.contains("toRow") &&
                                                         move.contains("toCol"))
            {
                ctx.gameContext.send = new sendMove{
                    move["fromRow"].get<int>(), move["fromCol"].get<int>(), move["toRow"].get<int>(),
                    move["toCol"].get<int>()
                };
            }
        }

        try
        {
            json response = gameHandler.action(ctx.action, ctx);
            s->send(hdl, response.dump(), msg->get_opcode());
        }
        catch (const std::exception& e)
        {
            json response;
            response["type"] = "Error";
            response["payload"]["message"] = e.what();
            s->send(hdl, response.dump(), msg->get_opcode());
        }
    }

    if (type == "Room")
    {
        if (j["payload"].contains("username"))
        {
            ctx.sessionContext.player = new Player(j["payload"]["username"]);
        }

        if (j["payload"].contains("roomName"))
        {
            ctx.roomContext.desiredRoomName = j["payload"]["roomName"];
        }
        try
        {
            json response = roomHandler.action(ctx.action, ctx);
            s->send(hdl, response.dump(), msg->get_opcode());
        }
        catch (const std::exception& e)
        {
            json response;
            response["type"] = "Error";
            response["payload"]["message"] = e.what();
            s->send(hdl, response.dump(), msg->get_opcode());
        }
    }

    // This is here to send any pending notifications that were generated during the handling of the message.
    // This ensures that clients receive updates about changes in game state, room status, or other relevant events
    // after their action has been processed.
    if (!ctx.pendingNotifications.empty())
    {
        for (const auto& notification : ctx.pendingNotifications)
        {
            notify(notification.recipients, notification.message, s);
        }
    }
}

int main()
{
    server chessServer;
    try
    {
        chessServer.set_access_channels(websocketpp::log::alevel::all);
        chessServer.clear_access_channels(websocketpp::log::alevel::frame_payload);

        chessServer.set_pong_timeout(10000);

        chessServer.set_pong_handler([](const websocketpp::connection_hdl&, const std::string&) { return true; });

        chessServer.init_asio();
        chessServer.set_http_handler(
            [&chessServer](websocketpp::connection_hdl hdl)
            {
                auto con = chessServer.get_con_from_hdl(std::move(hdl));
                con->append_header("Access-Control-Allow-Origin", "*");
            }
        );

        chessServer.set_message_handler([&](const websocketpp::connection_hdl& hdl, const server::message_ptr& msg)
                                        { on_message(&chessServer, hdl, msg); });

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

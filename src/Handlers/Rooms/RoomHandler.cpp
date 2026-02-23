//
// Created by stefanspitse on 1/31/26.
//

#include "RoomHandler.h"

#include "../ActionContext.h"
#include "Room.h"

void RoomHandler::router(const std::string action, const ActionContext& ctx)
{
    static const std::unordered_map<std::string, ActionFn> actionMap = {
        {"CreateRoom", [this](const ActionContext& a_ctx) { createRoom(a_ctx); }},
        {"JoinRoom", [this](const ActionContext& a_ctx) { joinRoom(a_ctx); }},
        {"ListRooms", [this](const ActionContext& a_ctx) { listRooms(a_ctx); }},
        {"LeaveRoom", [this](const ActionContext& a_ctx) { leaveRoom(a_ctx); }},
    };
    auto it = actionMap.find(action);
    if (it != actionMap.end())
    {
        try
        {
            it->second(ctx);
        }
        catch (...)
        {
            ctx.serverPtr->send(
                ctx.sessionContext.hdl,
                R"({"type": "Error", "payload": {"message": "Cant find that action"}})",
                websocketpp::frame::opcode::text
            );
        }
    }
    else
    {
        throw std::invalid_argument("Unknown action: " + action);
    }
}

void RoomHandler::createRoom(const ActionContext& ctx)
{
    int newRoomId = static_cast<int>(rooms.size()) + 1;
    auto newRoom = std::make_unique<Room>(newRoomId, ctx.roomContext.desiredRoomName);
    newRoom->addUser(ctx.sessionContext);
    rooms.insert(std::make_pair(std::move(newRoom), std::make_unique<SessionContext>(ctx.sessionContext)));
    std::string roomJson = rooms.begin()->first->toJson().dump();
    ctx.serverPtr->send(ctx.sessionContext.hdl, roomJson, websocketpp::frame::opcode::text);
}

void RoomHandler::joinRoom(const ActionContext& ctx)
{

    if (ctx.roomContext.desiredRoomName.empty())
    {
        ctx.serverPtr->send(ctx.sessionContext.hdl, R"({"type": "Error", "payload": {"message": "Missing Room name"}})", websocketpp::frame::opcode::text);
        return;
    }

    if (ctx.sessionContext.player->get_username().empty())
    {
        ctx.serverPtr->send(ctx.sessionContext.hdl, R"({"type": "Error", "payload": {"message": "Missing username"}})", websocketpp::frame::opcode::text);
        return;
    }

    if (ctx.roomContext.room != nullptr)
    {
        Room* room = ctx.roomContext.room;
        room->addUser(ctx.sessionContext);

        std::string roomJson = room->toJson().dump();
        for (const auto& playerCtx : room->getSessionContexts())
        {
            ctx.serverPtr->send(playerCtx.hdl, roomJson, websocketpp::frame::opcode::text);
        }
        return;
    }

    for (const auto& room: rooms)
    {
        if (room.first && room.first->get_room_name() == ctx.roomContext.desiredRoomName)
        {
            room.first->addUser(ctx.sessionContext);

            std::string roomJson = room.first->toJson().dump();
            for (const auto& playerCtx : room.first->getSessionContexts())
            {
                ctx.serverPtr->send(playerCtx.hdl, roomJson, websocketpp::frame::opcode::text);
            }
            return;
        }
    }
    // TODO return error
    throw std::invalid_argument("Room not found");
}

void RoomHandler::leaveRoom(const ActionContext& ctx)
{
    if (ctx.roomContext.desiredRoomName.empty())
    {
        ctx.serverPtr->send(ctx.sessionContext.hdl, R"({"type": "Error", "payload": {"message": "Missing Room name"}})", websocketpp::frame::opcode::text);
        return;
    }

    if (ctx.sessionContext.player->get_username().empty())
    {
        ctx.serverPtr->send(ctx.sessionContext.hdl, R"({"type": "Error", "payload": {"message": "Missing username"}})", websocketpp::frame::opcode::text);
        return;
    }

    if (ctx.roomContext.room != nullptr)
    {
        Room* room = ctx.roomContext.room;
        room->removeUser(ctx.sessionContext);

        std::string roomJson = room->toJson().dump();
        for (const auto& playerCtx : room->getSessionContexts())
        {
            ctx.serverPtr->send(playerCtx.hdl, roomJson, websocketpp::frame::opcode::text);
        }
        return;
    }

    for (const auto& room : rooms)
    {
        if (room.first && room.first->get_room_name() == ctx.roomContext.desiredRoomName)
        {
            room.first->removeUser(ctx.sessionContext);

            std::string roomJson = room.first->toJson().dump();
            for (const auto& playerCtx : room.first->getSessionContexts())
            {
                ctx.serverPtr->send(playerCtx.hdl, roomJson, websocketpp::frame::opcode::text);
            }
            return;
        }
    }
    throw std::invalid_argument("Room not found");
}

void RoomHandler::listRooms(const ActionContext& ctx) const
{
    nlohmann::json response;
    response["rooms"] = nlohmann::json::array();

    for (const auto& room : rooms)
    {
        if (room.first)
        {
            response["rooms"].push_back(room.first->toJson());
        }
    }

    ctx.serverPtr->send(ctx.sessionContext.hdl, response.dump(), websocketpp::frame::opcode::text);
}

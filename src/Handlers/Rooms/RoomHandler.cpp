//
// Created by stefanspitse on 1/31/26.
//
#include <algorithm>
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
            sendError(ctx, "An error occurred while processing the action: " + action);
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
    rooms.push_back(std::move(newRoom));

    std::string roomJson = rooms.back()->toJson().dump();
    ctx.serverPtr->send(ctx.sessionContext.hdl, roomJson, websocketpp::frame::opcode::text);
}

// TODO figure out why compiler wants this to Const
void RoomHandler::joinRoom(const ActionContext& ctx)
{

    if (ctx.roomContext.desiredRoomName.empty())
    {
        sendError(ctx, "Missing Room name");
        return;
    }

    if (ctx.sessionContext.player->get_username().empty())
    {
        sendError(ctx, "Missing username");
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

    for (const auto& room : rooms)
    {
        if (room && room->getRoomName() == ctx.roomContext.desiredRoomName)
        {
            room->addUser(ctx.sessionContext);

            std::string roomJson = room->toJson().dump();
            for (const auto& playerCtx : room->getSessionContexts())
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
        sendError(ctx, "Missing Room name");
        return;
    }

    if (ctx.sessionContext.player->get_username().empty())
    {
        sendError(ctx, "Missing username");
        return;
    }

    if (ctx.roomContext.room != nullptr)
    {
        Room* room = ctx.roomContext.room;
        room->removeUser(ctx.sessionContext);

        const std::string roomJson = room->toJson().dump();
        for (const auto& [player, hdl] : room->getSessionContexts())
        {
            ctx.serverPtr->send(hdl, roomJson, websocketpp::frame::opcode::text);
        }
        return;
    }

    for (const auto& room : rooms)
    {
        if (room && room->getRoomName() == ctx.roomContext.desiredRoomName)
        {
            room->removeUser(ctx.sessionContext);

            std::string roomJson = room->toJson().dump();
            for (const auto& playerCtx : room->getSessionContexts())
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
        if (room)
        {
            response["rooms"].push_back(room->toJson());
        }
    }

    ctx.serverPtr->send(ctx.sessionContext.hdl, response.dump(), websocketpp::frame::opcode::text);
}

Room RoomHandler::findRoomByName(const std::string& roomName) const
{
    for (const auto& room : rooms)
    {
        if (room && room->getRoomName() == roomName)
        {
            return *room;
        }
    }
    throw std::invalid_argument("Room not found");
}

void RoomHandler::removeRoom(Room room)
{
    auto it = std::ranges::remove_if(
                  rooms, [&room](const std::unique_ptr<Room>& r) { return r && r->getId() == room.getId(); }
    ).begin();
    if (it != rooms.end())
    {
        rooms.erase(it, rooms.end());
    }
}

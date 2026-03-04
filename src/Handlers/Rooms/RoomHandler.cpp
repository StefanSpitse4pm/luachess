//
// Created by stefanspitse on 1/31/26.
//
#include "RoomHandler.h"
#include "../ActionContext.h"
#include "Room.h"
#include <algorithm>

nlohmann::json RoomHandler::router(std::string action, const ActionContext& ctx)
{
    static const std::unordered_map<std::string, ActionFn> actionMap = {
        {"CreateRoom", [this](const ActionContext& a_ctx) -> nlohmann::json { return createRoom(a_ctx); }},
        {"JoinRoom", [this](const ActionContext& a_ctx) -> nlohmann::json { return joinRoom(a_ctx); }},
        {"ListRooms", [this](const ActionContext& a_ctx) -> nlohmann::json { return listRooms(a_ctx); }},
        {"LeaveRoom", [this](const ActionContext& a_ctx) -> nlohmann::json { return leaveRoom(a_ctx); }},
    };
    auto it = actionMap.find(action);
    if (it != actionMap.end())
    {
        try
        {
            return it->second(ctx);
        }
        catch (...)
        {
            throw std::runtime_error("An error occurred while processing the action: " + action);
        }
    }
    else
    {
        throw std::invalid_argument("Unknown action: " + action);
    }
    throw std::runtime_error("This should never be reached");
}

nlohmann::json RoomHandler::createRoom(const ActionContext& ctx)
{
    int newRoomId = static_cast<int>(rooms.size()) + 1;
    auto newRoom = std::make_unique<Room>(newRoomId, ctx.roomContext.desiredRoomName);
    newRoom->addUser(ctx.sessionContext);
    rooms.push_back(std::move(newRoom));

    nlohmann::json roomJson = rooms.back()->toJson();
    return roomJson;
}

// TODO figure out why compiler wants this to Const
nlohmann::json RoomHandler::joinRoom(const ActionContext& ctx)
{
    if (ctx.roomContext.desiredRoomName.empty())
    {
        throw std::invalid_argument("Missing Room name");
    }

    if (ctx.sessionContext.player->get_username().empty())
    {
        throw std::invalid_argument("Missing username");
    }

    if (ctx.roomContext.room != nullptr)
    {
        Room* room = ctx.roomContext.room;
        room->addUser(ctx.sessionContext);

        std::string roomJson = room->toJson().dump();
        ctx.pendingNotifications.push_back({room->getSessionContexts(), std::move(roomJson)});
        return room->toJson();
    }

    for (const auto& room : rooms)
    {
        if (room && room->getRoomName() == ctx.roomContext.desiredRoomName)
        {
            room->addUser(ctx.sessionContext);

            std::string roomJson = room->toJson().dump();
            ctx.pendingNotifications.push_back({room->getSessionContexts(), std::move(roomJson)});
            return room->toJson();
        }
    }
    // TODO return error
    throw std::invalid_argument("Room not found");
}

nlohmann::json RoomHandler::listRooms(const ActionContext& ctx) const
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

    return response;
}

nlohmann::json RoomHandler::leaveRoom(const ActionContext& ctx)
{
    if (ctx.roomContext.desiredRoomName.empty())
    {
        throw std::invalid_argument("Missing Room name");
    }

    if (ctx.sessionContext.player->get_username().empty())
    {
        throw std::invalid_argument("Missing username");
    }

    if (ctx.roomContext.room != nullptr)
    {
        Room* room = ctx.roomContext.room;
        room->removeUser(ctx.sessionContext);

        std::string roomJson = room->toJson().dump();
        ctx.pendingNotifications.push_back({room->getSessionContexts(), std::move(roomJson)});
        return room->toJson();
    }

    for (const auto& room : rooms)
    {
        if (room && room->getRoomName() == ctx.roomContext.desiredRoomName)
        {
            room->removeUser(ctx.sessionContext);

            std::string roomJson = room->toJson().dump();
            ctx.pendingNotifications.push_back({room->getSessionContexts(), std::move(roomJson)});
            return room->toJson();
        }
    }
    throw std::invalid_argument("Room not found");
}

nlohmann::json RoomHandler::removeRoom(Room room)
{
    auto it = std::ranges::remove_if(
                  rooms, [&room](const std::unique_ptr<Room>& r) { return r && r->getId() == room.getId(); }
    ).begin();
    if (it != rooms.end())
    {
        rooms.erase(it, rooms.end());
    }

    nlohmann::json response;
    response["removed"] = room.getId();
    return response;
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

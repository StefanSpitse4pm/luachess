// MIT License
//
// Copyright (c) 2026 Stefan Spitse
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//
// Created by stefanspitse on 1/31/26.
//
#include "RoomHandler.h"
#include "../ActionContext.h"
#include "Room.h"
#include <algorithm>

nlohmann::json RoomHandler::action(std::string action, const ActionContext& ctx)
{
    static const std::unordered_map<std::string, ActionFn> actionMap = {
        {"CreateRoom", [this](const ActionContext& a_ctx) -> nlohmann::json { return createRoom(a_ctx); }},
        {"JoinRoom", [this](const ActionContext& a_ctx) -> nlohmann::json { return joinRoom(a_ctx); }},
        {"ListRooms", [this](const ActionContext& a_ctx) -> nlohmann::json { return listRooms(a_ctx); }},
        {"LeaveRoom", [this](const ActionContext& a_ctx) -> nlohmann::json { return leaveRoom(a_ctx); }},
    };
    return route(actionMap, action)(ctx);
}

nlohmann::json RoomHandler::createRoom(const ActionContext& ctx)
{
    int newRoomId = static_cast<int>(rooms.size()) + 1;
    auto newRoom = std::make_unique<Room>(newRoomId, ctx.roomContext.desiredRoomName);
    newRoom->addUser(ctx.sessionContext);
    makePendingNotificationRoom(ctx, newRoom);
    rooms.push_back(std::move(newRoom));
    const nlohmann::json playerPublicID = {{"publicPlayerId", ctx.sessionContext.player->getPublicId()}};
    return playerPublicID;
}

void RoomHandler::makePendingNotificationRoom(const ActionContext& ctx, const std::unique_ptr<Room>& room) const
{
    std::string roomJson = room->toJson().dump();
    ctx.pendingNotifications.push_back({room->getSessionContexts(), std::move(roomJson)});
}

nlohmann::json RoomHandler::joinRoom(const ActionContext& ctx) const
{
    if (ctx.roomContext.desiredRoomName.empty())
    {
        throw std::invalid_argument("Missing Room name");
    }

    if (ctx.sessionContext.player->getUsername().empty())
    {
        throw std::invalid_argument("Missing username");
    }

    if (ctx.roomContext.room != nullptr)
    {
        Room* room = ctx.roomContext.room;
        room->addUser(ctx.sessionContext);

        std::string roomJson = room->toJson().dump();
        ctx.pendingNotifications.push_back({room->getSessionContexts(), std::move(roomJson)});
    }

    for (const auto& room : rooms)
    {
        if (room && room->getRoomName() == ctx.roomContext.desiredRoomName)
        {
            room->addUser(ctx.sessionContext);
            makePendingNotificationRoom(ctx, room);
            const nlohmann::json playerPublicID = {{"publicPlayerId", ctx.sessionContext.player->getPublicId()}};

            return playerPublicID;
        }
    }
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

    if (ctx.sessionContext.player->getUsername().empty())
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

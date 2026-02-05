//
// Created by stefanspitse on 1/31/26.
//

#include "RoomHandler.h"

#include "ActionContext.h"
#include "Room.h"

void RoomHandler::router(const std::string action, const ActionContext& ctx)
{
    static const std::unordered_map<std::string, ActionFn> actionMap = {
        {"CreateRoom", [this](const ActionContext& a_ctx) { createRoom(a_ctx); }},
        {"JoinRoom", [this](const ActionContext& a_ctx) { joinRoom(a_ctx); }},
        {"ListRooms", [this](const ActionContext& a_ctx) { listRooms(a_ctx); }},
    };
    auto it = actionMap.find(action);
    if (it != actionMap.end())
    {
        it->second(ctx);
    }
    else
    {
        throw std::invalid_argument("Unknown action: " + action);
    }
}

void RoomHandler::createRoom(const ActionContext& ctx)
{
    std::lock_guard<std::mutex> lock(roomsMutex);
    std::cout << "Creating room: " << ctx.roomContext.roomName << " for user: " << ctx.userContext.username
              << std::endl;
    int newRoomId = static_cast<int>(rooms.size()) + 1;
    auto newRoom = std::make_unique<Room>(newRoomId, ctx.roomContext.roomName);
    newRoom->addUser(ctx.userContext.username, ctx.userContext.hdl);
    assert(newRoom != nullptr);
    rooms.push_back(std::move(newRoom));
    assert(rooms.back() != nullptr);
}

// TODO figure out why compiler wants this to Const
void RoomHandler::joinRoom(const ActionContext& ctx)
{
    std::lock_guard<std::mutex> lock(roomsMutex);
    for (const auto& room : rooms)
    {
        if (room && room->get_room_name() == ctx.roomContext.roomName)
        {
            room->addUser(ctx.userContext.username, ctx.userContext.hdl);
            return;
        }
    }
    // TODO return error and success
    throw std::invalid_argument("Room not found");
}

void RoomHandler::listRooms(const ActionContext& ctx) const
{
    nlohmann::json response;
    response["rooms"] = nlohmann::json::array();

    std::lock_guard lock(roomsMutex);
    for (const auto& room : rooms)
    {
        if (room)
        {
            response["rooms"].push_back(room->toJson());
        }
    }

    std::cout << response.dump() << std::endl;
    ctx.serverPtr->send(ctx.userContext.hdl, response.dump(), websocketpp::frame::opcode::text);
}

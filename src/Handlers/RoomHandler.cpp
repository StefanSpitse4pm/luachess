//
// Created by stefanspitse on 1/31/26.
//

#include "RoomHandler.h"
#include "Room.h"
using ActionFn = void (RoomHandler::*)();

RoomHandler::~RoomHandler() = default;


void RoomHandler::router(std::string action) {

    static const std::unordered_map<std::string, ActionFn> actionMap = {
        {std::string("createRoom"), &RoomHandler::createRoom},
        {std::string("joinRoom"), &RoomHandler::joinRoom},
        {std::string("listRooms"), &RoomHandler::listRooms},
    };
}

void RoomHandler::createRoom(const std::string& roomName, const std::string& username, const websocketpp::connection_hdl& hdl) {
    int newRoomId = static_cast<int>(rooms.size()) + 1;
    auto newRoom = std::make_unique<Room>(newRoomId, roomName);
    newRoom->addUser(username, hdl);
    rooms.push_back(std::move(newRoom));
}

void RoomHandler::joinRoom(const std::string& roomName, const std::string& username, const websocketpp::connection_hdl& hdl) {
    for (const auto& room : rooms) {
        if (room && room->get_room_name() == roomName) {
            room->addUser(username, hdl);
            return;
        }
    }
    throw std::invalid_argument("Room not found");
}



nlohmann::json RoomHandler::listRooms() const {
    nlohmann::json response;
    response["rooms"] = nlohmann::json::array();
    for (const auto& room : rooms) {
        if (room) {
            response["rooms"].push_back(room->toJson());
        }
    }
    return response;
}

//
// Created by stefanspitse on 1/31/26.
//

#include "RoomHandler.h"
#include "Room.h"


RoomHandler::~RoomHandler() = default;

void RoomHandler::router(std::string action) {
}

void RoomHandler::createRoom(const std::string& roomName, const std::string& username, websocketpp::connection_hdl) {
}

void RoomHandler::joinRoom(const std::string& roomName, const std::string& username, websocketpp::connection_hdl) {
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

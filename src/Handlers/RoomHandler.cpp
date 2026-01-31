//
// Created by stefanspitse on 1/31/26.
//

#include "RoomHandler.h"
#include <nlohmann/json.hpp>
#include "Room.h"


void RoomHandler::router(std::string action) {
}

void RoomHandler::createRoom(const std::string& roomName, const std::string& username, websocketpp::connection_hdl) {
}

void RoomHandler::joinRoom(const std::string& roomName, const std::string& username, websocketpp::connection_hdl) {
}

nlohmann::json RoomHandler::listRooms() {
    nlohmann::json response;
    for (auto& room : rooms) {
        response["rooms"].push_back(room->toJson());

    }

}

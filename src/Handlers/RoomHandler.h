//
// Created by stefanspitse on 1/31/26.
//

#ifndef LUACHESS_ROOM_H
#define LUACHESS_ROOM_H
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <websocketpp/common/chrono.hpp>
#include <websocketpp/common/connection_hdl.hpp>

#include "Handler.h"
#include "Room.h"


class Room;

class RoomHandler : public Handler {
    public:

    RoomHandler() = default;
    ~RoomHandler() override = default;

    void router(std::string action) override;
    void createRoom(const std::string& roomName, const std::string& username, websocketpp::connection_hdl);
    void joinRoom(const std::string& roomName, const std::string& username, websocketpp::connection_hdl);
    nlohmann::json listRooms();
    private:
    std::vector<std::unique_ptr<Room>> rooms{};
};


#endif //LUACHESS_ROOM_H
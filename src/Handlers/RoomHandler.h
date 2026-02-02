//
// Created by stefanspitse on 1/31/26.
//

#ifndef LUACHESS_ROOMHANDLER_H
#define LUACHESS_ROOMHANDLER_H
#include <vector>
#include <nlohmann/json.hpp>
#include <websocketpp/common/connection_hdl.hpp>

#include "Handler.h"
#include "Room.h"


class RoomHandler : public Handler {
    public:

    RoomHandler();
    ~RoomHandler();

    void router(std::string action) override;
    void createRoom(const std::string& roomName, const std::string& username, const websocketpp::connection_hdl&);
    void joinRoom(const std::string& roomName, const std::string& username, const websocketpp::connection_hdl&);
    nlohmann::json listRooms() const;
    private:
    std::vector<std::unique_ptr<Room>> rooms{};
};


#endif //LUACHESS_ROOMHANDLER_H

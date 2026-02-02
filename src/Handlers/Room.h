//
// Created by stefanspitse on 1/31/26.
//

#ifndef LUACHESS_ROOM_H
#define LUACHESS_ROOM_H
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <utility>
#include <websocketpp/common/connection_hdl.hpp>


class Room {
    public:
    Room(int id, std::string room_name)
            : id(id),
              roomName(std::move(room_name)) {
        }

    ~Room() = default;

    [[nodiscard]] int get_id() const
    {
        return id;
    }

    void set_id(int id) {
        this->id = id;
    }

    [[nodiscard]] std::string get_room_name() const
    {
        return roomName;
    }

    void set_room_name(const std::string &room_name)
    {
        roomName = room_name;
    }


    void addUser(const std::string& username, const websocketpp::connection_hdl& connection_hdl);
    nlohmann::json toJson();

    private:
    int id;
    std::string roomName;
    std::vector<websocketpp::connection_hdl> playerHdl;
    std::vector<std::string> playerNames;
};


#endif //LUACHESS_ROOM_H


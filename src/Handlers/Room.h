//
// Created by stefanspitse on 1/31/26.
//

#ifndef LUACHESS_ROOM_H
#define LUACHESS_ROOM_H
#include "ActionContext.h"

#include <nlohmann/json.hpp>
#include <string>
#include <utility>
#include <vector>
#include <websocketpp/common/connection_hdl.hpp>

class Room
{
  public:
    Room(int id, std::string room_name) : id(id), roomName(std::move(room_name))
    {
    }

    ~Room() = default;

    [[nodiscard]] std::string get_room_name() const
    {
        return roomName;
    }

    void set_room_name(const std::string& room_name)
    {
        roomName = room_name;
    }

    void addUser(const std::string& username, const websocketpp::connection_hdl& connection_hdl);
    void removeUser(UserContext userContext);
    [[nodiscard]] nlohmann::json toJson() const;
    [[nodiscard]] const std::vector<UserContext>& getPlayerContexts() const;

  private:
    int id;
    std::string roomName;
    std::vector<UserContext> playerContexts;
};

#endif // LUACHESS_ROOM_H

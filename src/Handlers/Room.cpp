//
// Created by stefanspitse on 1/31/26.
//

#include "Room.h"

#include <nlohmann/json.hpp>

constexpr int MAX_PLAYER_COUNT = 2;

void Room::addUser(const std::string& username, const websocketpp::connection_hdl& connection_hdl)
{
    if (username.empty())
    {
        throw std::invalid_argument("username is empty");
    }

    if (playerNames.size() + 1 > MAX_PLAYER_COUNT)
    {
        throw std::out_of_range("get_player_hdl() > MAX_PLAYER_COUNT");
    }

    playerNames.push_back(username);
    playerHdl.push_back(connection_hdl);
}

nlohmann::json Room::toJson() const
{
    return {
        {"roomName", roomName},
        {"roomSize", MAX_PLAYER_COUNT},
        {"filledSpots", playerNames.size()},
    };
}

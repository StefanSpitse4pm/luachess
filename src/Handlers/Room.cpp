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

    if (playerContexts.size() + 1 > MAX_PLAYER_COUNT)
    {
        throw std::out_of_range("get_player_hdl() > MAX_PLAYER_COUNT");
    }

    playerContexts.emplace_back(username, connection_hdl);
}

nlohmann::json Room::toJson() const
{
    std::vector<std::string> usernames;
    usernames.reserve(playerContexts.size());
    for (const auto& [username, hdl] : playerContexts)    {
        usernames.push_back(username);
    }

    return {
        {"roomName", roomName},
        {"roomSize", MAX_PLAYER_COUNT},
        {"filledSpots", playerContexts.size()},
        {"players", usernames},
    };
}


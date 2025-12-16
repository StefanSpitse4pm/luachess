#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <array>
#include <iostream>
#include <websocketpp/server.hpp>
#include <stdexcept>

const int MAX_PLAYER_COUNT = 2;


class Room {
    public:
        Room(std::string nameRoom) {
            this->roomName = nameRoom;
        }

        void addUser(std::string name, websocketpp::connection_hdl hdl) {
            if (name.empty()) {
                throw new std::invalid_argument("Name cannot be empty");
            }

            if (filledSpots >= MAX_PLAYER_COUNT) {
                throw new std::out_of_range("group is full");
            }
            player_names[filledSpots++] = name;
            player_hdls[filledSpots] = hdl;
            std::cout << name << " size of " << filledSpots << " to 2" << '\n';
            return;
        }

        json toJson(){
           return {
            {"roomName", this->roomName},
            {"roomSize", MAX_PLAYER_COUNT},
            {"filledSpots", filledSpots},
            {"users", player_names},
           };
        }

    private:
        int id;
        size_t filledSpots = 0;
        std::string roomName;
        std::array<websocketpp::connection_hdl, 2> player_hdls;
        std::array<std::string, 2> player_names;
};

#endif
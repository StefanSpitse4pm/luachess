#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <array>
#include <websocketpp/server.hpp>
#include <stdexcept>

const int MAX_PLAYER_COUNT = 2

class Room {
    public:
        Room(std::string nameRoomMaker) {

        }

        void addUser(std::string name, websocketpp::connection_hdl hdl) {
            if (name.empty()) {
                throw new std::invalid_argument("Name cannot be empty");
            }

            


        }

    private:
        int id;
        std::string roomName;
        std::array<websocketpp::connection_hdl, 2> player_hdls;
        std::array<std::string, 2> player_names;
};

#endif
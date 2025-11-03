#ifndef LUACONTROLLER_H
#define LUACONTROLLER_H

#include <sol/sol.hpp>
#include "chessboard.h"

struct luaRoomState {
    std::string roomName;
    sol::state lua;
    std::array<std::string, 2> players;
    Chessboard chessboard{8, 8};
};

void setup_lua_api(sol::state& lua, Chessboard& chessboard);

#endif
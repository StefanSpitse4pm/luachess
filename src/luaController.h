#ifndef LUACONTROLLER_H
#define LUACONTROLLER_H

#include "chessboard.h"
#include <sol/sol.hpp>

struct luaRoomState
{
    sol::state lua;
    Chessboard chessboard{8, 8};
};

void setup_lua_api(sol::state& lua, Chessboard& chessboard);

#endif

#ifndef GAME_STATE_CALCULATOR
#define GAME_STATE_CALCULATOR
#include <iostream>
#include <sol/sol.hpp>

struct lua_State;
class gameStateCalculator {
private:
    sol::state *luaState;
    std::string fileLocation;
    int move[1]; //0: dx, 1: dy.
public:
    gameStateCalculator();
    ~gameStateCalculator();
};

#endif
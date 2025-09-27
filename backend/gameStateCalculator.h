#ifndef GAME_STATE_CALCULATOR
#define GAME_STATE_CALCULATOR
#include <iostream>

struct lua_State;
class gameStateCalculator {
private:
    lua_State *luaState;
    std::string fileLocation;
    int move[1]; //0: x, 1: y.
public:
    gameStateCalculator();
};

#endif
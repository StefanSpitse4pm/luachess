//
// Created by stefanspitse on 2/19/26.
//

#ifndef LUACHESS_LUAENGINE_H
#define LUACHESS_LUAENGINE_H
#include "Engine.h"
#include "sol/sol.hpp"

class LuaEngine : public Engine
{
    // TODO state should be created on constructor
  public:
    LuaEngine() = default;
    void setup(Chessboard& board) override;
    void initialize(std::filesystem::path scriptPath, Chessboard& board) override;
    void executeScript(std::string& functionName, Chessboard& board);

  private:
    sol::state luaState;
};

#endif // LUACHESS_LUAENGINE_H

//
// Created by stefanspitse on 2/19/26.
//

#include "PlayerCreatedLuaGameFactory.h"

#include "Engine/LuaEngine.h"
#include "../../Chess/chessboard.h"

std::unique_ptr<Game> PlayerCreatedLuaGameFactory::createGame(ActionContext)
{
    LuaEngine engine = LuaEngine();
    Chessboard board = Chessboard(8,8);
    auto newGame = std::make_unique<Game>(engine, board, "scripts/default_chess.lua");
    return std::move(newGame);
}

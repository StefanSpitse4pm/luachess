//
// Created by stefanspitse on 2/19/26.
//

#include "PlayerCreatedLuaGameFactory.h"

#include "../Chess/chessboard.h"
#include "Engine/LuaEngine.h"

std::unique_ptr<Game> PlayerCreatedLuaGameFactory::createGame(ActionContext)
{
    auto engine = std::make_unique<LuaEngine>();
    auto board = std::make_unique<Chessboard>(8, 8);
    auto newGame = std::make_unique<Game>(
        std::move(engine), std::move(board), std::filesystem::current_path() / "lua" / "regularChess.lua"
    );

    return newGame;
}

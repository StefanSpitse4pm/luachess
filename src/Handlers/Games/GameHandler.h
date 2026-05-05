// MIT License
//
// Copyright (c) 2026 Stefan Spitse
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//
// Created by stefanspitse on 2/14/26.
//

#ifndef LUACHESS_GAMEHANDLER_H
#define LUACHESS_GAMEHANDLER_H

#include "../ActionContext.h"
#include "../Handler.h"
#include "../Rooms/Player.h"
#include "../Rooms/RoomHandler.h"
#include "Decorator/TurnOrderDecorator.h"
#include "GameFactory.h"
#include "LuaGame.h"
#include "PlayerCreatedLuaGameFactory.h"

class GameHandler : public Handler
{
  public:
    GameHandler(RoomHandler& roomHandler) : roomHandler(roomHandler)
    {
        auto factory = std::make_unique<PlayerCreatedLuaGameFactory>();
        factories.push_back(std::move(factory));
    };

    nlohmann::json action(std::string action, const ActionContext& ctx) override;
    nlohmann::json startGame(const ActionContext& ctx);
    nlohmann::json getBoardState(ActionContext ctx);
    Game& getGameByGameId(const ActionContext& ctx);
    nlohmann::json onMove(const ActionContext& ctx);

  private:
    std::unique_ptr<Game> decorateGame(std::unique_ptr<LuaGame> game) const;

    std::vector<std::unique_ptr<Game>> games;
    std::vector<std::unique_ptr<GameFactory>> factories;
    RoomHandler& roomHandler;
};

#endif // LUACHESS_GAMEHANDLER_H

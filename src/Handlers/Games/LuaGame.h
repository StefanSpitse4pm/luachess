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
// Created by stefanspitse on 2/19/26.
//

#ifndef LUACHESS_GAME_H
#define LUACHESS_GAME_H
#include "../../Chess/chessboard.h"
#include "../ActionContext.h"
#include "Engine/Engine.h"
#include "Game.h"

#include <atomic>
#include <memory>
#include <string>
#include <utility>

class LuaGame : public Game
{
  public:
    LuaGame(std::unique_ptr<Engine> engine, std::unique_ptr<Chessboard> chessboard, std::string filepath)
        : engine(std::move(engine)), chessboard(std::move(chessboard)), filepath(std::move(filepath)), id(nextId++)
    {
    }

    void start() override;
    void stop() override;
    void executeScript(std::string functionName) const;
    [[nodiscard]] json applyMove(const sendMove& move) const override;

    [[nodiscard]] Chessboard& getChessboard() const
    {
        return *chessboard;
    }

    void addPlayers(const std::vector<SessionContext>& players)
    {
        playerSessionContexts.insert(playerSessionContexts.end(), players.begin(), players.end());
    }

    [[nodiscard]] const std::vector<SessionContext>& getSessionContexts() const override
    {
        return playerSessionContexts;
    }

    [[nodiscard]] uint32_t getId() const
    {
        return id;
    }

    [[nodiscard]] json toJson() const
    {
        return {{"id", id}};
    }
  protected:
    std::vector<SessionContext> playerSessionContexts;

  private:
    std::unique_ptr<Engine> engine;
    std::unique_ptr<Chessboard> chessboard;
    std::filesystem::path filepath;
    inline static std::atomic<uint32_t> nextId{1};
    uint32_t id = 1;
};

#endif // LUACHESS_GAME_H

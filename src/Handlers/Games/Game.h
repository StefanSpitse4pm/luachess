//
// Created by stefanspitse on 2/19/26.
//

#ifndef LUACHESS_GAME_H
#define LUACHESS_GAME_H
#include "../../Chess/chessboard.h"
#include "../ActionContext.h"
#include "Engine/Engine.h"

#include <atomic>
#include <memory>
#include <string>
#include <utility>

class Game
{
  public:
    Game(std::unique_ptr<Engine> engine, std::unique_ptr<Chessboard> chessboard, std::string filepath)
        : engine(std::move(engine)), chessboard(std::move(chessboard)), filepath(std::move(filepath)), id(nextId++)
    {
    }

    void start() const;
    void stop();
    void executeScript(std::string functionName) const;

    [[nodiscard]] Chessboard& getChessboard() const
    {
        return *chessboard;
    }

    void addPlayers(const std::vector<SessionContext>& players)
    {
        playerSessionContexts.insert(playerSessionContexts.end(), players.begin(), players.end());
    }

    [[nodiscard]] const std::vector<SessionContext>& getSessionContexts() const
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

  private:
    std::unique_ptr<Engine> engine;
    std::unique_ptr<Chessboard> chessboard;
    std::filesystem::path filepath;
    std::vector<SessionContext> playerSessionContexts;
    inline static std::atomic<uint32_t> nextId{1};
    uint32_t id = 1;
};

#endif // LUACHESS_GAME_H

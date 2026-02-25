//
// Created by stefanspitse on 2/19/26.
//

#ifndef LUACHESS_GAME_H
#define LUACHESS_GAME_H
#include "Engine/Engine.h"
#include "../../Chess/chessboard.h"
#include "../ActionContext.h"

#include <memory>
#include <atomic>
#include <string>
#include <utility>



class Game
{
    public:
        Game(std::unique_ptr<Engine> engine, std::unique_ptr<Chessboard> chessboard, std::string  filepath)
        : engine(std::move(engine)), chessboard(std::move(chessboard)), filepath(std::move(filepath)), id(nextId++){}

        void start() const;
        void stop();

        [[nodiscard]] Chessboard& getChessboard() const
        {
            return *chessboard;
        }

        void addPlayers(const std::vector<SessionContext>& players)
        {
            sessionContexts.insert(sessionContexts.end(), players.begin(), players.end());
        }

        [[nodiscard]] const std::vector<SessionContext>& getSessionContexts() const
        {
            return sessionContexts;
        }

        [[nodiscard]] uint32_t getId() const
        {
            return id;
        }

      private:
        std::unique_ptr<Engine> engine;
        std::unique_ptr<Chessboard> chessboard;
        std::filesystem::path filepath;
        std::vector<SessionContext> sessionContexts; // players
        inline static std::atomic<uint32_t> nextId{1};
        uint32_t id;

};

#endif // LUACHESS_GAME_H

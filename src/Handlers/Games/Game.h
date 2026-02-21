//
// Created by stefanspitse on 2/19/26.
//

#ifndef LUACHESS_GAME_H
#define LUACHESS_GAME_H
#include "Engine/Engine.h"
#include "../../Chess/chessboard.h"

#include <memory>
#include <string>
#include <utility>

class Game
{
    public:
        Game(std::unique_ptr<Engine> engine, std::unique_ptr<Chessboard> chessboard, std::string  filepath)
        : engine(std::move(engine)), chessboard(std::move(chessboard)), filepath(std::move(filepath)){}

        void start() const;
        void stop();

        [[nodiscard]] Chessboard& getChessboard() const
        {
            return *chessboard;
        }

      private:
        std::unique_ptr<Engine> engine;
        std::unique_ptr<Chessboard> chessboard;
        std::filesystem::path filepath;
};

#endif // LUACHESS_GAME_H

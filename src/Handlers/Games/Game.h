//
// Created by stefanspitse on 2/19/26.
//

#ifndef LUACHESS_GAME_H
#define LUACHESS_GAME_H
#include "Engine/Engine.h"

#include <string>
#include <utility>

class Game
{
    public:
        Game(Engine& engine, Chessboard& chessboard, std::string  filepath)
        : engine(engine), chessboard(chessboard), filepath(std::move(filepath)){}

        void start() const;
        void stop();

        [[nodiscard]] Chessboard& getChessboard() const
        {
            return chessboard;
        }

      private:
        Engine& engine;
        Chessboard& chessboard;
        std::filesystem::path filepath;
};

#endif // LUACHESS_GAME_H

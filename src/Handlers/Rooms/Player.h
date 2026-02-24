//
// Created by stefanspitse on 2/18/26.
//

#ifndef LUACHESS_PLAYER_H
#define LUACHESS_PLAYER_H
#include <string>
#include <utility>
#include <atomic>

class Player
{
    public:
    explicit Player(std::string  username) : username(std::move(username)), id(nextId++)
      {
      }

      [[nodiscard]] std::string get_username() const
    {
        return username;
    }

      [[nodiscard]] uint32_t get_id() const
    {
        return id;
    }

  private:
        std::string username{};
        uint32_t id;
        inline static std::atomic<uint32_t> nextId{1};
};

#endif // LUACHESS_PLAYER_H


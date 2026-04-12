//
// Created by stefanspitse on 4/12/26.
//

#ifndef LUACHESS_TURNORDER_H
#define LUACHESS_TURNORDER_H
#include <vector>
#include "../Rooms/Player.h"
#include "../ActionContext.h"
#include <memory>
#include <websocketpp/connection.hpp>

class turnOrder {
  public:
    turnOrder(std::vector<std::unique_ptr<Player>>& players, std::unique_ptr<Player>& player)
        : players(std::move(players)), player(std::move(player))
    {
    }
    void turnTo(SessionContext player);
    void defaultTurnOrder(bool);


  private:
        std::vector<std::unique_ptr<Player>> players;
        std::unique_ptr<Player> player;
};



#endif //LUACHESS_TURNORDER_H

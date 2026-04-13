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

class TurnOrder {
  public:
    TurnOrder(std::vector<std::unique_ptr<Player>>& players, Player& player)
        : players(std::move(players)), currentPlayer(&player)
    {
    }
    [[nodiscard]] Player getCurrentPlayer() const
    {
        return *currentPlayer;
    }

    void turnTo(Player& player);
    void defaultTurnOrder(bool);


  private:
        std::vector<std::unique_ptr<Player>> players;
        Player* currentPlayer;

        void setCurrentPlayer(Player& current_player)
        {
            currentPlayer = &current_player;
        }
};



#endif //LUACHESS_TURNORDER_H

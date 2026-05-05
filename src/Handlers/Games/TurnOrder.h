//
// Created by stefanspitse on 4/12/26.
//

#ifndef LUACHESS_TURNORDER_H
#define LUACHESS_TURNORDER_H
#include "../ActionContext.h"
#include "../Rooms/Player.h"
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <vector>
#include <websocketpp/connection.hpp>

class TurnOrder
{
  public:
    TurnOrder(std::vector<std::unique_ptr<Player>>& players, Player& player)
        : players(std::move(players)), currentPlayer(&player)
    {
    }

    [[nodiscard]] Player& getCurrentPlayer() const
    {
        return *currentPlayer;
    }

    std::ranges::borrowed_iterator_t<std::vector<std::unique_ptr<Player>>&> isPlayerInTurnOrder(const Player& player);
    void turnTo(Player& player);
    void defaultTurnOrder();
    [[nodiscard]] nlohmann::json toJson() const;

  private:
    std::vector<std::unique_ptr<Player>> players;
    Player* currentPlayer;

    void setCurrentPlayer(Player& current_player)
    {
        currentPlayer = &current_player;
    }
};

#endif // LUACHESS_TURNORDER_H

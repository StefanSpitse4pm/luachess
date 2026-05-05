/*
MIT License

Copyright (c) 2026 Stefan Spitse

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

//
// Created by stefanspitse on 4/12/26.
//

#include "TurnOrder.h"

#include <nlohmann/json.hpp>

std::ranges::borrowed_iterator_t<std::vector<std::unique_ptr<Player>>&>
TurnOrder::isPlayerInTurnOrder(const Player& player)
{
    return std::ranges::find_if(
        this->players, [&](const std::unique_ptr<Player>& uPlayer) { return uPlayer->getId() == player.getId(); }
    );
}

void TurnOrder::turnTo(Player& player)
{
    if (player.getId() == getCurrentPlayer().getId())
    {
        return;
    }
    const auto it = isPlayerInTurnOrder(player);
    if (it != this->players.end())
    {
        setCurrentPlayer(player);
    }
    else
    {
        throw std::invalid_argument("Player is not inside players list");
    }
}

void TurnOrder::defaultTurnOrder()
{
    const auto it = isPlayerInTurnOrder(this->getCurrentPlayer());
    if (it != this->players.end())
    {
        if (int index = std::distance(players.begin(), it); index == this->players.size() - 1)
        {
            turnTo(*this->players[0]);
        }
        else
        {
            index += 1;
            turnTo(*this->players[index]);
        }
    }
}

nlohmann::json TurnOrder::toJson() const
{
    return {{"activePlayer", getCurrentPlayer().getUsername()}};
}

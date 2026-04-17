//
// Created by stefanspitse on 4/10/26.
//

#include "GameDecorator.h"

#include <nlohmann/json.hpp>

void GameDecorator::start()
{
    this->wrapped.start();
}

void GameDecorator::stop()
{
    this->wrapped.stop();
}

nlohmann::json GameDecorator::applyMove(const sendMove& move) const
{
    return this->wrapped.applyMove(move);
}

uint32_t GameDecorator::getId() const
{
    return this->wrapped.getId();
}

nlohmann::json GameDecorator::toJson() const
{
    return this->wrapped.toJson();
}

nlohmann::json GameDecorator::getBoardState() const
{
    return this->wrapped.getBoardState();
}

const std::vector<SessionContext>& GameDecorator::getSessionContexts() const
{
    return this->wrapped.getSessionContexts();
}

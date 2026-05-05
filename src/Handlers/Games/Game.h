//
// Created by stefanspitse on 4/7/26.
//
#pragma once
#include "../ActionContext.h"
#include "Engine/Engine.h"
#include <cstdint>
#include <nlohmann/json_fwd.hpp>
#define LUACHESS_GAMEDECORATOR_H

class Game
{
  public:
    virtual ~Game() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
    [[nodiscard]] virtual nlohmann::json applyMove(const sendMove& move) const = 0;
    [[nodiscard]] virtual uint32_t getId() const = 0;
    [[nodiscard]] virtual nlohmann::json toJson() const = 0;
    [[nodiscard]] virtual nlohmann::json getBoardState() const = 0;
    [[nodiscard]] virtual const std::vector<SessionContext>& getSessionContexts() const = 0;
    virtual Player& getPlayerByPublicID(std::string pid) = 0;
    [[nodiscard]] virtual Engine& getEngine() const = 0;
};

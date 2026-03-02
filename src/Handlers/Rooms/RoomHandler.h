//
// Created by stefanspitse on 1/31/26.
//

#ifndef LUACHESS_ROOMHANDLER_H
#define LUACHESS_ROOMHANDLER_H
#include <mutex>
#include <nlohmann/json.hpp>
#include <vector>
#include <websocketpp/common/connection_hdl.hpp>

#include "../ActionContext.h"
#include "../Handler.h"
#include "Room.h"

class RoomHandler : public Handler
{
  public:
    RoomHandler() = default;
    ~RoomHandler() override = default;

    nlohmann::json router(std::string action, const ActionContext& ctx) override;
    nlohmann::json createRoom(const ActionContext& ctx);
    nlohmann::json joinRoom(const ActionContext& ctx);
    [[nodiscard]] nlohmann::json listRooms(const ActionContext& ctx) const;
    nlohmann::json leaveRoom(const ActionContext& ctx);
    nlohmann::json removeRoom(Room room);
    [[nodiscard]] Room findRoomByName(const std::string& roomName) const;

  private:
    std::vector<std::unique_ptr<Room>> rooms{};
};

#endif // LUACHESS_ROOMHANDLER_H

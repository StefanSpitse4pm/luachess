//
// Created by stefanspitse on 1/31/26.
//

#ifndef LUACHESS_ROOMHANDLER_H
#define LUACHESS_ROOMHANDLER_H
#include <mutex>
#include <nlohmann/json.hpp>
#include <vector>
#include <websocketpp/common/connection_hdl.hpp>

#include "ActionContext.h"
#include "Handler.h"
#include "Room.h"

class RoomHandler : public Handler
{
  public:
    RoomHandler() = default;
    ~RoomHandler() override = default;

    using ActionFn = std::function<void(const ActionContext&)>;

    void router(std::string action, const ActionContext& ctx) override;
    void createRoom(const ActionContext& ctx);
    void joinRoom(const ActionContext& ctx);
    void listRooms(const ActionContext& ctx) const;

  private:
    std::vector<std::unique_ptr<Room>> rooms{};
};

#endif // LUACHESS_ROOMHANDLER_H

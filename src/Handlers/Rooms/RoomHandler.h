//
// Created by stefanspitse on 1/31/26.
//

#ifndef LUACHESS_ROOMHANDLER_H
#define LUACHESS_ROOMHANDLER_H
#include <nlohmann/json.hpp>
#include <map>

#include "../ActionContext.h"
#include "../Handler.h"
#include "Room.h"

class RoomHandler : public Handler
{
  public:
    RoomHandler() = default;
    ~RoomHandler() override = default;


    void router(std::string action, const ActionContext& ctx) override;
    void createRoom(const ActionContext& ctx);
    void joinRoom(const ActionContext& ctx);
    void listRooms(const ActionContext& ctx) const;
    void leaveRoom(const ActionContext& ctx);

  private:
    std::map<std::unique_ptr<Room>, std::unique_ptr<SessionContext>> rooms{};
};

#endif // LUACHESS_ROOMHANDLER_H

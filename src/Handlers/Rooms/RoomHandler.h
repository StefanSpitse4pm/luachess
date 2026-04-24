// MIT License
//
// Copyright (c) 2026 Stefan Spitse
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
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

    nlohmann::json action(std::string action, const ActionContext& ctx) override;
    nlohmann::json createRoom(const ActionContext& ctx);
    void makePendingNotificationRoom(const ActionContext& ctx, const std::unique_ptr<Room>& room) const;
    nlohmann::json joinRoom(const ActionContext& ctx) const;
    [[nodiscard]] nlohmann::json listRooms(const ActionContext& ctx) const;
    nlohmann::json leaveRoom(const ActionContext& ctx);
    nlohmann::json removeRoom(Room room);
    [[nodiscard]] Room findRoomByName(const std::string& roomName) const;

  private:
    std::vector<std::unique_ptr<Room>> rooms{};
};

#endif // LUACHESS_ROOMHANDLER_H

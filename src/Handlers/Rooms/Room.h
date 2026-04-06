/*
* MIT License

* Copyright (c) [year] [fullname]

* Permission is hereby granted, free of charge, to any person obtaining a copy
*of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:

* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
//
// Created by stefanspitse on 1/31/26.
//

#ifndef LUACHESS_ROOM_H
#define LUACHESS_ROOM_H
#include "../ActionContext.h"

#include <nlohmann/json.hpp>
#include <string>
#include <utility>
#include <vector>
#include <websocketpp/common/connection_hdl.hpp>

class Room
{
  public:
    Room(int id, std::string room_name) : id(nextId++), roomName(std::move(room_name))
    {
    }

    ~Room() = default;

    [[nodiscard]] std::string getRoomName() const
    {
        return roomName;
    }

    void setRoomName(const std::string& room_name)
    {
        roomName = room_name;
    }

    [[nodiscard]] uint32_t getId() const
    {
        return id;
    }

    [[nodiscard]] int getMaxPlayerCount() const
    {
        return MAX_PLAYER_COUNT;
    }

    [[nodiscard]] std::vector<SessionContext> getSessionContexts() const
    {
        return sessionContexts;
    }

    void addUser(const SessionContext& sessionContext);
    void removeUser(SessionContext userContext);
    bool isReady() const;
    [[nodiscard]] nlohmann::json toJson() const;

  private:
    int MAX_PLAYER_COUNT = 2;
    uint32_t id;
    inline static std::atomic<uint32_t> nextId{1};
    std::string roomName;
    std::vector<SessionContext> sessionContexts;
};

#endif // LUACHESS_ROOM_H

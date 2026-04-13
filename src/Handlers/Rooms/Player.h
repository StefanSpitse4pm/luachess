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
// Created by stefanspitse on 2/18/26.
//

#ifndef LUACHESS_PLAYER_H
#define LUACHESS_PLAYER_H
#include <atomic>
#include <string>
#include <utility>

class Player
{
  public:
    explicit Player(std::string username) : username(std::move(username)), id(nextId++)
    {
    }

    [[nodiscard]] std::string get_username() const
    {
        return username;
    }

    [[nodiscard]] uint32_t get_id() const
    {
        return id;
    }

  private:
    std::string username{};
    uint32_t id;
    inline static std::atomic<uint32_t> nextId{1};
};

#endif // LUACHESS_PLAYER_H

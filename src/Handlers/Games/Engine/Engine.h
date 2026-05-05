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
// Created by stefanspitse on 2/19/26.
//

#ifndef LUACHESS_ENGINE_H
#define LUACHESS_ENGINE_H
#include "../TurnOrder.h"
#include <filesystem>
class Chessboard;

class Engine
{
  public:
    virtual ~Engine() = default;
    virtual void setup() = 0;
    virtual void initialize(std::filesystem::path scriptPath, Chessboard& board) = 0;
    virtual void executeScript(std::string& functionName, Chessboard& board) = 0;
    virtual void addChessboard(Chessboard& board) = 0;
    virtual void addPieces() = 0;
    virtual void addTurnOrder(TurnOrder& turnOrder) = 0;
};

#endif // LUACHESS_ENGINE_H

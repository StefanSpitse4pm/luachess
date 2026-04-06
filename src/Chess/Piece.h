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
// Created by stefanspitse on 2/16/26.
//

#ifndef LUACHESS_PIECE_H
#define LUACHESS_PIECE_H
#include "Move.h"
#include <array>
#include <string>
#include <utility>
#include <vector>

class Piece
{
  public:
    std::array<int, 2> position; // position[0] = row, position[1] = col
    std::string type;
    std::string image;
    std::vector<Move> possibleMoves;
    std::vector<Move> possibleTakes;
    bool canJumpOverPieces = false;
    std::string color;

    Piece(const std::array<int, 2>& position, std::string type, std::string image, std::string color)
        : position(position), type(std::move(type)), image(std::move(image)), color(std::move(color))
    {
    }

    void addMove(int dx, int dy, bool repeat = false, bool basedOnLastMove = false)
    {
        possibleMoves.push_back({dx, dy, repeat, basedOnLastMove});
    }

    void addTake(int dx, int dy, bool repeat = false, bool basedOnLastMove = false)
    {
        possibleTakes.push_back({dx, dy, repeat, basedOnLastMove});
    }

    void clearMoves()
    {
        possibleMoves.clear();
        possibleTakes.clear();
    }
};
#endif // LUACHESS_PIECE_H

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

    void addMove(int dx, int dy, bool repeat = false, bool basedOnLastMove = false);
    void addTake(int dx, int dy, bool repeat = false, bool basedOnLastMove = false);
    void clearMoves();
};
#endif // LUACHESS_PIECE_H

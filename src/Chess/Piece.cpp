//
// Created by stefanspitse on 5/15/26.
//
#include "Piece.h"

void Piece::addMove(int dx, int dy, bool repeat, bool basedOnLastMove)
{
    possibleMoves.push_back({dx, dy, repeat, basedOnLastMove});
}

void Piece::addTake(int dx, int dy, bool repeat, bool basedOnLastMove)
{
    possibleTakes.push_back({dx, dy, repeat, basedOnLastMove});
}

void Piece::clearMoves()
{
    possibleMoves.clear();
    possibleTakes.clear();
}

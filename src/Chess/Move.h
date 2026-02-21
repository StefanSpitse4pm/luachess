//
// Created by stefanspitse on 2/16/26.
//
#pragma once

struct Move
{
    int dx{};
    int dy{};
    bool repeat = false;
    bool basedOnLastMove = false;
};

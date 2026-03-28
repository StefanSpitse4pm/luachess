//
// Created by stefanspitse on 2/19/26.
//

#ifndef LUACHESS_ENGINE_H
#define LUACHESS_ENGINE_H
#include <filesystem>
class Chessboard;

class Engine
{
  public:
    virtual ~Engine() = default;
    virtual void setup(Chessboard& board) = 0;
    virtual void initialize(std::filesystem::path scriptPath, Chessboard& board) = 0;
    virtual void executeScript(std::string& functionName, Chessboard& board) = 0;
};

#endif // LUACHESS_ENGINE_H

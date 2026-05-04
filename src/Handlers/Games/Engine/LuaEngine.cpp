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

#include "LuaEngine.h"
#include "../../../Chess/chessboard.h"
#include "../TurnOrder.h"


void LuaEngine::setup(Chessboard& board)
{
    luaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table, sol::lib::string, sol::lib::math);

    addChessboard();
    addPieces();

    luaState.set_function(
        "createPiece", [](const std::string& type, const std::string& image, int row, int col, std::string color)
        { return Piece({col, row}, type, image, std::move(color)); }
    );
}

void LuaEngine::addChessboard()
{
    luaState.new_usertype<Chessboard>(
        "Chessboard", "isOccupied", &Chessboard::isOccupied, "getPieceAt", &Chessboard::getPieceAt, "setPieceAt",
        &Chessboard::setPieceAt, "movePiece", &Chessboard::movePiece, "rows", sol::property(&Chessboard::getRows),
        "cols", sol::property(&Chessboard::getCols), "calculateRepeatMoves", &Chessboard::unrollRepeatMoves
    );
}

void LuaEngine::addPieces()
{
    luaState.new_usertype<Piece>(
        "Piece", "position", &Piece::position, "type", &Piece::type, "image", &Piece::image, "canJumpOverPieces",
        &Piece::canJumpOverPieces, "clearMoves", &Piece::clearMoves, "possibleMoves", &Piece::possibleMoves,
        "possibleTakes", &Piece::possibleTakes, "addMove", &Piece::addMove, "addTake", &Piece::addTake, "color",
        &Piece::color
    );

    luaState.new_usertype<Move>(
        "Move", "dx", &Move::dx, "dy", &Move::dy, "repeat", &Move::repeat, "basedOnLastMove", &Move::basedOnLastMove
    );
}

void LuaEngine::addTurnOrder(TurnOrder& turnOrder)
{
    luaState.new_usertype<TurnOrder>("TurnOrder","defaultTurnOrder",&TurnOrder::defaultTurnOrder);
    luaState["TurnOrder"] = &turnOrder;
}

void LuaEngine::initialize(const std::filesystem::path scriptPath, Chessboard& board)
{
    std::cerr << "Loading lua: " << scriptPath << "\n";
    luaState.script_file(scriptPath);
    const sol::protected_function setupFunc = luaState["setup"];

    if (!setupFunc.valid())
    {
        throw std::invalid_argument("Lua setup function not found in script: " + scriptPath.string());
    }

    if (const sol::protected_function_result result = setupFunc(board); !result.valid())
    {
        const sol::error err = result;
        std::cerr << "Error calling Lua setup function: " << err.what() << std::endl;
    }
    board.unrollRepeatMoves();
}

void LuaEngine::executeScript(std::string& functionName, Chessboard& board)
{

    if (const sol::protected_function func = luaState[functionName]; func.valid())
    {
        if (const sol::protected_function_result result = func(board); !result.valid())
        {
            const sol::error err = result;
            throw std::invalid_argument("Error calling Lua function '" + functionName + "': " + err.what());
        }
    }
    else
    {
        throw std::invalid_argument("Lua function '" + functionName + "' not found.");
    }
}

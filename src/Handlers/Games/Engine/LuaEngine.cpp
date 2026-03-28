//
// Created by stefanspitse on 2/19/26.
//

#include "LuaEngine.h"
#include "../../../Chess/chessboard.h"

void LuaEngine::setup(Chessboard& board)
{
    luaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table, sol::lib::string, sol::lib::math);

    luaState.new_usertype<Chessboard>(
        "Chessboard", "isOccupied", &Chessboard::isOccupied, "getPieceAt", &Chessboard::getPieceAt, "setPieceAt",
        &Chessboard::setPieceAt, "movePiece", &Chessboard::movePiece, "rows", sol::property(&Chessboard::getRows),
        "cols", sol::property(&Chessboard::getCols), "calculateRepeatMoves", &Chessboard::calculateRepeatMoves
    );

    luaState.new_usertype<Piece>(
        "Piece", "position", &Piece::position, "type", &Piece::type, "image", &Piece::image, "canJumpOverPieces",
        &Piece::canJumpOverPieces, "clearMoves", &Piece::clearMoves, "possibleMoves", &Piece::possibleMoves,
        "possibleTakes", &Piece::possibleTakes, "addMove", &Piece::addMove, "addTake", &Piece::addTake, "color",
        &Piece::color
    );

    luaState.new_usertype<Move>(
        "Move", "dx", &Move::dx, "dy", &Move::dy, "repeat", &Move::repeat, "basedOnLastMove", &Move::basedOnLastMove
    );

    luaState.set_function(
        "createPiece", [](const std::string& type, const std::string& image, int row, int col, std::string color)
        { return Piece({col, row}, type, image, std::move(color)); }
    );
}

void LuaEngine::initialize(const std::filesystem::path scriptPath, Chessboard& board)
{
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
    board.calculateRepeatMoves();
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

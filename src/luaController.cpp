#include "Chess/Piece.h"
#include "Chess/chessboard.h"
#include <sol/sol.hpp>
#include <utility>

void setup_lua_api(sol::state& lua, Chessboard& chessboard)
{
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table, sol::lib::string, sol::lib::math);

    lua.new_usertype<Chessboard>(
        "Chessboard", "isOccupied", &Chessboard::isOccupied, "getPieceAt", &Chessboard::getPieceAt, "setPieceAt",
        &Chessboard::setPieceAt, "movePiece", &Chessboard::movePiece, "rows", sol::property(&Chessboard::getRows),
        "cols", sol::property(&Chessboard::getCols), "calculateRepeatMoves", &Chessboard::calculateRepeatMoves
    );

    lua.new_usertype<Piece>(
        "Piece", "position", &Piece::position, "type", &Piece::type, "image", &Piece::image, "canJumpOverPieces",
        &Piece::canJumpOverPieces, "clearMoves", &Piece::clearMoves, "possibleMoves", &Piece::possibleMoves,
        "possibleTakes", &Piece::possibleTakes, "addMove", &Piece::addMove, "addTake", &Piece::addTake, "color",
        &Piece::color
    );

    lua.new_usertype<Move>(
        "Move", "dx", &Move::dx, "dy", &Move::dy, "repeat", &Move::repeat, "basedOnLastMove", &Move::basedOnLastMove
    );

    lua.set_function(
        "createPiece",
        [](const std::string& type, const std::string& image, int row, int col, std::string color)
        {
            return Piece({col, row}, type, image, std::move(color));
        }
    );

    const std::filesystem::path scriptPath = std::filesystem::current_path() / "lua" / "regularChess.lua";

    lua.script_file(scriptPath);
    sol::protected_function setupFunc = lua["setup"];
    if (setupFunc.valid())
    {
        sol::protected_function_result result = setupFunc(chessboard);
        if (!result.valid())
        {
            sol::error err = result;
            std::cerr << "Error calling Lua setup function: " << err.what() << std::endl;
        }
    }
    chessboard.calculateRepeatMoves();
}

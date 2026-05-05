#include "chessboard.h"
#include <iostream>
#include <ranges>

struct Chessboard::repeatingMoves
{
    std::vector<size_t> indices;
    std::vector<Move> ToExpand;
};

json Chessboard::toJson()
{
    json j = json::array();
    for (int r = 0; r < rows_; ++r)
    {
        for (int c = 0; c < cols_; ++c)
        {
            if (board[r][c].has_value())
            {
                const Piece& piece = board[r][c].value();
                json pieceJson;
                pieceJson["position"] = {{"row", r}, {"col", c}};
                pieceJson["type"] = piece.type;
                pieceJson["canJumpOverPieces"] = piece.canJumpOverPieces;
                pieceJson["image"] = piece.image;

                pieceJson["possibleMoves"] = json::array();
                for (const auto& move : piece.possibleMoves)
                {
                    pieceJson["possibleMoves"].push_back(
                        {{"dx", move.dx},
                         {"dy", move.dy},
                         {"repeat", move.repeat},
                         {"basedOnLastMove", move.basedOnLastMove}}
                    );
                }

                pieceJson["possibleTakes"] = json::array();
                for (const auto& take : piece.possibleTakes)
                {
                    pieceJson["possibleTakes"].push_back(
                        {{"dx", take.dx},
                         {"dy", take.dy},
                         {"repeat", take.repeat},
                         {"basedOnLastMove", take.basedOnLastMove}}
                    );
                }

                j.push_back(pieceJson);
            }
        }
    }
    return j;
}

void Chessboard::unrollRepeatMoves()
{
    foreachPiece(
        [this](Piece& piece)
        {
            repeatingMoves repeatMoves;
            repeatingMoves repeatTakes;
            repeatMoves = findRepeatingMoves(piece.possibleMoves, repeatMoves);
            repeatTakes = findRepeatingMoves(piece.possibleTakes, repeatTakes);
            piece.possibleMoves = removeRepeatingMoves(piece.possibleMoves, repeatMoves);
            piece.possibleTakes = removeRepeatingMoves(piece.possibleTakes, repeatTakes);

            repeatUntilBlocked(repeatMoves.ToExpand, piece);
        }
    );
}

void Chessboard::repeatUntilBlocked(const std::vector<Move>& repeatMovesToExpand, Piece& piece) const
{
    for (const Move& repeatMove : repeatMovesToExpand)
    {
        int step = 1;
        while (true)
        {
            int newRow = piece.position[0] + repeatMove.dy * step;
            int newCol = piece.position[1] + repeatMove.dx * step;
            if (newRow < 0 || newRow >= rows_ || newCol < 0 || newCol >= cols_)
            {
                break;
            }

            if (isOccupied(newRow, newCol))
            {
                break;
            }

            piece.possibleMoves.push_back(
                {repeatMove.dx * step, repeatMove.dy * step, false, repeatMove.basedOnLastMove}
            );
            step++;
        }
    }
}

Chessboard::repeatingMoves Chessboard::findRepeatingMoves(const std::vector<Move>& moves, repeatingMoves& repeat)
{
    for (size_t i = 0; i < moves.size(); ++i)
    {
        if (moves[i].repeat)
        {
            repeat.ToExpand.push_back(moves[i]);
            repeat.indices.push_back(i);
        }
    }
    return repeat;
}

std::vector<Move> Chessboard::removeRepeatingMoves(std::vector<Move>& moves, repeatingMoves& repeat)
{
    for (const unsigned long& repeatMovesIndice : std::views::reverse(repeat.indices))
    {
        moves.erase(moves.begin() + repeatMovesIndice);
    }
    return moves;
}

pieces = {
    pawn = {
        get_moves = function(piece, board)
            print("cool")
        end
    }
}

function getLegalMoves(board)
    for x = 0, board.rows - 1 do
        for y = 0, board.cols - 1 do
            if board:getPieceAt(x, y) ~= nil then
                print("Piece at", x, y)
            end
        end
    end
end
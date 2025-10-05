pieces = {
    pawn = {
        get_moves = function(piece, board)
            print("cool")
        end
    }
}

function getLegalMoves(board)
    for x = 1, board.rows do 
        for y = 1, board.cols do
            if board:getPieceAt(x, y) then
                print("Piece at ", x, y)
            end
        end
    end
end
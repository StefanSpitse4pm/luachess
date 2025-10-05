function setup(board)
    local piece = createPiece("rook", "Chess_rlt45.svg", 5, 5)
    piece:addMove(-1, -1, false, true)
    board:setPieceAt(piece)
    print(board:isOccupied(0, 0))
end
---@diagnostic disable: undefined-doc-param, undefined-doc-name
---@class Piece
---@field position: {number, number}
---@field type: string
---@field image: string
---@field movePatterns: { {number, number, boolean, boolean} }
---@field attackPatterns: { {number, number, boolean, boolean} }
---@field canJumpOverPieces: boolean
---@method addMove(number, number)
---@method possibleMoves: { {number, number} }


---@class Chessboard
---@field rows: number
---@field cols: number
---@field board: table<number, table<number, Piece?>>

---@param piece Piece
---@param row number
---@param col number
---@return boolean

function createPiece(type, image, row, col, color)
    return {
        type = type,
        color = color,
        image = image,
        position = {row, col},
        movePatterns = {},
        attackPatterns = {},
        canJumpOverPieces = false,
        addMove = function(dx, dy, repeat, basedOnLastMove)
            table.insert(this.movePatterns, {dx, dy, repeat, basedOnLastMove})
        end,
        addTake = function(dx, dy, repeat, basedOnLastMove)
            table.insert(this.attackPatterns, {dx, dy, repeat, basedOnLastMove})
        end
    }
end
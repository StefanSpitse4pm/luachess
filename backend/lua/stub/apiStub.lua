---@diagnostic disable: undefined-doc-param, undefined-doc-name
---@class Piece
---@field position {int, int}
---@field type string
---@field image string
---@field movePatterns { {int, int, boolean, boolean} }
---@field attackPatterns { {int, int, boolean, boolean} }
---@field canJumpOverPieces boolean
---@method addMove(int, int)
---@method possibleMoves 


---@class Chessboard
---@field rows int
---@field cols int
---@field board table<int, table<int, Piece?>>

---@param piece Piece
---@param row int
---@param col int
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
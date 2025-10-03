@class Piece
---@field position {int, int}
---@field type string
---@field image string
---@field movePatterns { {int, int, boolean, boolean} }
---@field attackPatterns { {int, int, boolean, boolean} }
---@field canJumpOverPieces boolean

---@class Chessboard
---@field rows int
---@field cols int
---@field board table<int, table<int, Piece?>>

---@param piece Piece
---@param row int
---@param col int
---@return boolean

function isOccupied(row, col) end
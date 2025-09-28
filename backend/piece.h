#ifndef PIECE_H
#define PIECE_H

#include <iostream>
#include <vector>
struct Move {
    int dx;
    int dy;
    bool repeat = false;
    bool basedOnLastMove = false;
};

class Piece {
public:
    Piece(int x, int y, std::string identifier, bool canJumpOverPieces,std::string image, std::vector<Move> possibleMoves = {}, std::vector<Move> possibleTakes = {});
    ~Piece();
    int getX() const { return x; }
    void setX(int newX) { x = newX; }

    int getY() const { return y; }
    void setY(int newY) { y = newY; }

    std::string getIdentifier() const { return identifier; }
    void setIdentifier(const std::string& newIdentifier) { identifier = newIdentifier; }

    bool getCanJumpOverPieces() const { return canJumpOverPieces; }
    void setCanJumpOverPieces(bool canJump) { canJumpOverPieces = canJump; }

    std::string getImage() const { return image; }
    void setImage(const std::string& newImage) { image = newImage; }

    std::vector<Move> getPossibleMoves() const { return possibleMoves; }
    void setPossibleMoves(const std::vector<Move>& moves) { possibleMoves = moves; }

    std::vector<Move> getPossibleTakes() const { return possibleTakes; }
    void setPossibleTakes(const std::vector<Move>& takes) { possibleTakes = takes; }
private:
    int x;
    int y;
    bool canJumpOverPieces = false;
    std::string identifier;
    std::vector<Move> possibleMoves;
    std::vector<Move> possibleTakes;
    std::string image;
};

#endif
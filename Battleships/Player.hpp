#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "GameBoard.hpp"
#include <string>
#include <vector>

class Player {
protected:
    std::string name;
    GameBoard myBoard;
    GameBoard enemyBoard;
    std::vector<Ship> ships;

public:
    Player(const std::string& name);
    virtual ~Player() = default;
    virtual void placeShips() = 0;
    virtual void makeMove(Player& enemy) = 0;
    bool allShipsSunk() const;
};

#endif
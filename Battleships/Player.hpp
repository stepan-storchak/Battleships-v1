#pragma once

#include "GameBoard.hpp"
#include "Coordinate.hpp"
#include "ShotResult.hpp"
#include "Ship.hpp"
#include <string>
#include <vector>
#include <memory>

class Player {
protected:
    std::string name;
    GameBoard myBoard;
    GameBoard enemyBoard;
    std::vector<Ship> ships;
    static int playerCount;

public:
    Player(const std::string& name);

    virtual ~Player() = default;

    static int getPlayerCount() { return playerCount; }

    virtual void placeShips() = 0;
    virtual void makeMove(Player& enemy) = 0;
    virtual bool makeMoveWithResult(Player& enemy) = 0;
    virtual void markAreaAroundDestroyedShip(Player& enemy, const Coordinate& hitCoord) = 0;

    virtual std::string getPlayerType() const { return "Base Player"; }
    virtual void displayInfo() const;

    bool allShipsSunk() const;
    ShotResult getShotResult(const Coordinate& coord);

    const std::string& getName() const { return this->name; }
    void setName(const std::string& name) { this->name = name; }

    const GameBoard& getMyBoard() const { return this->myBoard; }
    const GameBoard& getEnemyBoard() const { return this->enemyBoard; }
    const std::vector<Ship>& getShips() const { return this->ships; }
    void addShip(const Ship& ship);
};
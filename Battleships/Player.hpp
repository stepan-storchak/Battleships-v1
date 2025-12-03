#pragma once
#include "GameBoard.hpp"
#include "Coordinate.hpp"
#include "ShotResult.hpp"
#include "Ship.hpp"
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <list>
#include <array>
#include <algorithm>

class Player {
protected:
    std::string name;
    GameBoard myBoard;
    GameBoard enemyBoard;
    std::vector<Ship> ships;
    static int playerCount;

    std::map<std::string, int> statistics;
    std::list<Coordinate> recentMoves;
    std::array<int, 4> shipCountBySize{ 0, 0, 0, 0 };

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

    void updateStatistics(const std::string& key, int value);
    void addRecentMove(const Coordinate& coord);
    void updateShipCount(int size);

    template<typename T>
    int countShipsByCondition(T condition) const {
        return std::count_if(ships.begin(), ships.end(), condition);
    }

    template<typename T>
    std::vector<Ship> findShips(T predicate) const {
        std::vector<Ship> result;
        std::copy_if(ships.begin(), ships.end(), std::back_inserter(result), predicate);
        return result;
    }

    void sortShipsBySize();
    void removeDestroyedShips();

    const std::list<Coordinate>& getRecentMoves() const { return recentMoves; }
    const std::array<int, 4>& getShipCountBySize() const { return shipCountBySize; }
};
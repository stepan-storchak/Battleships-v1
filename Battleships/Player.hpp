#pragma once
#include "GameBoard.hpp"
#include "Coordinate.hpp"
#include "ShotResult.hpp"
#include "Ship.hpp"
#include <string>
#include <vector>
#include <map>

class Player {
protected:
    std::string name;
    GameBoard myBoard;
    GameBoard enemyBoard;
    std::vector<Ship> ships;
    static int playerCount;
    std::map<std::string, int> statistics;  
    
public:
    Player(const std::string& name);
    virtual ~Player() = default;
    
    static int getPlayerCount() { return playerCount; }
    
    virtual bool makeMoveWithResult(Player& enemy) = 0;
    virtual void placeShips() = 0;
    virtual void makeMove(Player& enemy) = 0;
    virtual void markAreaAroundDestroyedShip(Player& enemy, 
                                           const Coordinate& hitCoord) = 0;
    virtual std::string getPlayerType() const { return "Base Player"; }
    
    bool allShipsSunk() const;
    ShotResult getShotResult(const Coordinate& coord);
    
    const std::string& getName() const { return name; }
    const GameBoard& getMyBoard() const { return myBoard; }
    const GameBoard& getEnemyBoard() const { return enemyBoard; }
    const std::vector<Ship>& getShips() const { return ships; }
    
    void addShip(const Ship& ship);
    void updateStatistics(const std::string& key, int value);
    

};
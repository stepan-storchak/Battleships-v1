#include "Player.hpp"
#include <iostream>
#include <stdexcept>

int Player::playerCount = 0;


Player::Player(const std::string& name) : name(name), myBoard(), enemyBoard() {
    playerCount++;  
}

bool Player::allShipsSunk() const {
    for (const auto& ship : this->ships) {
        if (!ship.isSunk()) {
            return false;
        }
    }
    return true;
}


ShotResult Player::getShotResult(const Coordinate& coord) {
    try {      
        ShotResult result = this->myBoard.receiveShot(coord);

        if (result == ShotResult::Hit) {
            for (auto& ship : this->ships) {
                for (const auto& shipCoord : ship.getCoordinates()) {
                    if (shipCoord == coord) { 
                        bool wasSunk = ship.takeHit(coord);
                        if (wasSunk) {
                            result = ShotResult::Sunk;
                            this->myBoard.markAreaAroundSunkShip(ship);
                        }
                        break;
                    }
                }
                if (result == ShotResult::Sunk) break;
            }
        }
        return result;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при обработке выстрела: " << e.what() << std::endl;
        return ShotResult::Miss;
    }
}

void Player::addShip(const Ship& ship) {
    this->ships.push_back(ship);
}
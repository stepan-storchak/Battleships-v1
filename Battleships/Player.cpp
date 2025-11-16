#include "Player.hpp"

Player::Player(const std::string& name) : name(name) {}

bool Player::allShipsSunk() const {
    for (const auto& ship : ships) {
        if (!ship.isSunk()) {
            return false;
        }
    }
    return true;
}

ShotResult Player::getShotResult(const Coordinate& coord) {
    ShotResult result = myBoard.receiveShot(coord);

    if (result == ShotResult::Hit) {
        for (auto& ship : ships) {
            if (ship.takeHit(coord)) {
                result = ShotResult::Sunk;
                break;
            }
        }
    }

    return result;
}

void Player::addShip(const Ship& ship) {
    ships.push_back(ship);
}
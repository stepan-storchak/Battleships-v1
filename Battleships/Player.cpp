#include "Player.hpp"


int Player::playerCount = 0;

Player::Player(const std::string& name) : name(name) {
    playerCount++;
    statistics["hits"] = 0;
    statistics["misses"] = 0;
    statistics["ships_destroyed"] = 0;
}

bool Player::allShipsSunk() const {
    return std::all_of(ships.begin(), ships.end(),
        [](const Ship& ship) { return ship.isSunk(); });
}

ShotResult Player::getShotResult(const Coordinate& coord) {
    ShotResult result = myBoard.receiveShot(coord);

    if (result == ShotResult::Hit || result == ShotResult::Sunk) {
        auto it = std::find_if(ships.begin(), ships.end(),
            [&coord](const Ship& ship) {
                const auto& coords = ship.getCoordinates();
                return std::any_of(coords.begin(), coords.end(),
                    [&coord](const Coordinate& c) { return c == coord; });
            });

        if (it != ships.end()) {
            bool wasSunk = it->takeHit(coord);
            if (wasSunk) {
                result = ShotResult::Sunk;
                myBoard.markAreaAroundSunkShip(*it);
                statistics["ships_destroyed"]++;
            }
        }
    }

    return result;
}

void Player::addShip(const Ship& ship) {
    ships.push_back(ship);
}

void Player::updateStatistics(const std::string& key, int value) {
    statistics[key] += value;
}


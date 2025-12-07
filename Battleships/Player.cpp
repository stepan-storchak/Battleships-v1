#include "Player.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <numeric>

int Player::playerCount = 0;

Player::Player(const std::string& name) : name(name), myBoard(), enemyBoard() {
    playerCount++;
    statistics["hits"] = 0;
    statistics["misses"] = 0;
    statistics["ships_destroyed"] = 0;
}

bool Player::allShipsSunk() const {
    return std::all_of(ships.begin(), ships.end(), [](const Ship& ship) {
        return ship.isSunk();
        });
}

ShotResult Player::getShotResult(const Coordinate& coord) {
    try {
        ShotResult result = this->myBoard.receiveShot(coord);

        if (result == ShotResult::Hit || result == ShotResult::Sunk) {
            auto it = std::find_if(ships.begin(), ships.end(), [&coord](const Ship& ship) {
                const auto& coords = ship.getCoordinates();
                return std::any_of(coords.begin(), coords.end(), [&coord](const Coordinate& c) {
                    return c == coord;
                    });
                });

            if (it != ships.end()) {
                bool wasSunk = it->takeHit(coord);
                if (wasSunk) {
                    result = ShotResult::Sunk;
                    this->myBoard.markAreaAroundSunkShip(*it);
                    statistics["ships_destroyed"]++;
                }
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
    updateShipCount(ship.getSize());
}

void Player::displayInfo() const {
    std::cout << "Информация об игроке:" << std::endl;
    std::cout << "Имя: " << this->name << std::endl;
    std::cout << "Тип: " << getPlayerType() << std::endl;
    std::cout << "Количество кораблей: " << this->ships.size() << std::endl;

    int sunkCount = std::count_if(this->ships.begin(), this->ships.end(),
        [](const Ship& ship) { return ship.isSunk(); });
    std::cout << "Уничтожено кораблей: " << sunkCount << std::endl;

    std::cout << "Статистика:" << std::endl;
    for (const auto& stat : statistics) {
        std::cout << "  " << stat.first << ": " << stat.second << std::endl;
    }
}

void Player::updateStatistics(const std::string& key, int value) {
    statistics[key] += value;
}

void Player::addRecentMove(const Coordinate& coord) {
    recentMoves.push_back(coord);
    if (recentMoves.size() > 10) {
        recentMoves.pop_front();
    }
}

void Player::updateShipCount(int size) {
    if (size >= 1 && size <= 4) {
        shipCountBySize[size - 1]++;
    }
}

void Player::sortShipsBySize() {
    std::sort(ships.begin(), ships.end(),
        [](const Ship& a, const Ship& b) {
            return a.getSize() > b.getSize();
        });
}

void Player::removeDestroyedShips() {
    auto it = std::remove_if(ships.begin(), ships.end(),
        [](const Ship& ship) { return ship.isSunk(); });
    ships.erase(it, ships.end());
}
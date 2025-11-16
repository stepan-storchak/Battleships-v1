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
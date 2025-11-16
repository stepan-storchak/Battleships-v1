#include "GameBoard.hpp"
#include <iostream>

GameBoard::GameBoard() : grid(BOARD_SIZE, std::vector<CellState>(BOARD_SIZE, CellState::Empty)) {}

bool GameBoard::isValidPlacement(const Ship& ship) const {
    const auto& coords = ship.getCoordinates();
    for (const auto& coord : coords) {
        if (coord.x < 0 || coord.x >= BOARD_SIZE || coord.y < 0 || coord.y >= BOARD_SIZE) {
            return false;
        }
        if (grid[coord.y][coord.x] != CellState::Empty) {
            return false;
        }
    }
    return true;
}

bool GameBoard::placeShip(const Ship& ship) {
    if (!isValidPlacement(ship)) return false;

    // Упрощенная реализация для первого коммита
    return true;
}

ShotResult GameBoard::receiveShot(const Coordinate& coord) {
    if (coord.x < 0 || coord.x >= BOARD_SIZE || coord.y < 0 || coord.y >= BOARD_SIZE) {
        return ShotResult::Miss;
    }
    return ShotResult::Miss; // Заглушка
}

void GameBoard::display(bool showShips) const {
    std::cout << "  A B C D E F G H I J" << std::endl;
    for (int y = 0; y < BOARD_SIZE; ++y) {
        std::cout << (y + 1) << " ";
        for (int x = 0; x < BOARD_SIZE; ++x) {
            std::cout << ". ";
        }
        std::cout << std::endl;
    }
}
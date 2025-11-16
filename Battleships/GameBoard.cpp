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

    CellState shipState;
    switch (ship.getSize()) {
    case 1: shipState = CellState::Ship1; break;
    case 2: shipState = CellState::Ship2; break;
    case 3: shipState = CellState::Ship3; break;
    case 4: shipState = CellState::Ship4; break;
    default: return false;
    }

    for (const auto& coord : ship.getCoordinates()) {
        grid[coord.y][coord.x] = shipState;
    }

    return true;
}

ShotResult GameBoard::receiveShot(const Coordinate& coord) {
    if (coord.x < 0 || coord.x >= BOARD_SIZE || coord.y < 0 || coord.y >= BOARD_SIZE) {
        return ShotResult::Miss;
    }

    CellState& cell = grid[coord.y][coord.x];

    switch (cell) {
    case CellState::Empty:
        cell = CellState::Miss;
        return ShotResult::Miss;
    case CellState::Ship1:
    case CellState::Ship2:
    case CellState::Ship3:
    case CellState::Ship4:
        cell = CellState::Hit;
        return ShotResult::Hit;
    default:
        return ShotResult::Miss;
    }
}

void GameBoard::display(bool showShips) const {
    std::cout << "  A B C D E F G H I J" << std::endl;
    for (int y = 0; y < BOARD_SIZE; ++y) {
        std::cout << (y + 1) << " ";
        for (int x = 0; x < BOARD_SIZE; ++x) {
            char symbol = '.';
            switch (grid[y][x]) {
            case CellState::Empty: symbol = '.'; break;
            case CellState::Miss: symbol = 'O'; break;
            case CellState::Hit: symbol = 'X'; break;
            case CellState::Ship1: symbol = showShips ? '1' : '.'; break;
            case CellState::Ship2: symbol = showShips ? '2' : '.'; break;
            case CellState::Ship3: symbol = showShips ? '3' : '.'; break;
            case CellState::Ship4: symbol = showShips ? '4' : '.'; break;
            }
            std::cout << symbol << " ";
        }
        std::cout << std::endl;
    }
}


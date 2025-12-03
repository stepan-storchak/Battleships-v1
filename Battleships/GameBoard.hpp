#pragma once
#include "CellState.hpp"
#include "Coordinate.hpp"
#include "Ship.hpp"
#include "ShotResult.hpp"
#include <vector>
#include <array>
#include <algorithm>

class GameBoard {
private:
    static const int BOARD_SIZE = 10;
    std::array<std::array<CellState, BOARD_SIZE>, BOARD_SIZE> grid;

public:
    GameBoard();

    bool isValidPlacement(const Ship& ship) const;
    bool placeShip(const Ship& ship);
    ShotResult receiveShot(const Coordinate& coord);
    bool isCellEmpty(const Coordinate& coord) const;
    void markAreaAroundSunkShip(const Ship& ship);
    void display(bool showShips) const;
    CellState getCellState(const Coordinate& coord) const;
    void setCellState(const Coordinate& coord, CellState state);
    void clearBoard();

    static int getBoardSize() { return BOARD_SIZE; }

    std::array<CellState, BOARD_SIZE> getRow(int y) const {
        if (y >= 0 && y < BOARD_SIZE) {
            return grid[y];
        }
        return std::array<CellState, BOARD_SIZE>();
    }

    std::array<CellState, BOARD_SIZE> getColumn(int x) const {
        std::array<CellState, BOARD_SIZE> column;
        for (int y = 0; y < BOARD_SIZE; ++y) {
            column[y] = grid[y][x];
        }
        return column;
    }

    int countCells(CellState state) const {
        int count = 0;
        for (const auto& row : grid) {
            count += std::count(row.begin(), row.end(), state);
        }
        return count;
    }

    std::vector<Coordinate> findCells(CellState state) const {
        std::vector<Coordinate> result;
        for (int y = 0; y < BOARD_SIZE; ++y) {
            for (int x = 0; x < BOARD_SIZE; ++x) {
                if (grid[y][x] == state) {
                    result.push_back(Coordinate(x, y));
                }
            }
        }
        return result;
    }

    bool anyHit() const {
        for (const auto& row : grid) {
            if (std::any_of(row.begin(), row.end(),
                [](CellState state) { return state == CellState::Hit; })) {
                return true;
            }
        }
        return false;
    }
};
#pragma once

#include "CellState.hpp"
#include "Coordinate.hpp"
#include "Ship.hpp"
#include "ShotResult.hpp"
#include <vector>

class GameBoard {
private:
    std::vector<std::vector<CellState>> grid; 
    static const int BOARD_SIZE = 10;         

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
};

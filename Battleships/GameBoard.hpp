#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

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
    void display(bool showShips) const;
    static int getBoardSize() { return BOARD_SIZE; }
};

#endif
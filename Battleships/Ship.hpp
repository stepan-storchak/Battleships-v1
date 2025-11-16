#ifndef SHIP_HPP
#define SHIP_HPP

#include "Coordinate.hpp"
#include <vector>

enum class Orientation { Horizontal, Vertical };

class Ship {
private:
    int size;
    std::vector<Coordinate> coordinates;
    std::vector<bool> hits;
    Orientation orientation;

public:
    Ship(int size, const Coordinate& startCoord, Orientation orientation);
    bool isSunk() const;
    bool takeHit(const Coordinate& coord);
    const std::vector<Coordinate>& getCoordinates() const;
    int getSize() const;
};

#endif
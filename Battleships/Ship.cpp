#include "Ship.hpp"

Ship::Ship(int size, const Coordinate& start, Orientation orientation,
    const std::string& name)
    : size(size), name(name) {

    coordinates.resize(size);
    hits.resize(size, false);

    int dx = 0, dy = 0;
    if (orientation == Orientation::Horizontal) dx = 1;
    else dy = 1;

    for (int i = 0; i < size; ++i) {
        coordinates[i] = start + Coordinate(dx * i, dy * i);
    }
}

bool Ship::isSunk() const {
    return std::all_of(hits.begin(), hits.end(), [](bool hit) { return hit; });
}

bool Ship::takeHit(const Coordinate& coord) {
    for (int i = 0; i < size; ++i) {
        if (coordinates[i] == coord) {
            hits[i] = true;
            return isSunk();
        }
    }
    return false;
}
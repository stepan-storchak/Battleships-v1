#include "Ship.hpp"

Ship::Ship(int size, const Coordinate& startCoord, Orientation orientation)
    : size(size), orientation(orientation) {
    coordinates.reserve(size);
    hits.resize(size, false);

    for (int i = 0; i < size; ++i) {
        if (orientation == Orientation::Horizontal) {
            coordinates.push_back(Coordinate(startCoord.x + i, startCoord.y));
        }
        else {
            coordinates.push_back(Coordinate(startCoord.x, startCoord.y + i));
        }
    }
}

bool Ship::isSunk() const {
    for (bool hit : hits) {
        if (!hit) return false;
    }
    return true;
}

bool Ship::takeHit(const Coordinate& coord) {
    for (size_t i = 0; i < coordinates.size(); ++i) {
        if (coordinates[i] == coord) {
            hits[i] = true;
            return isSunk();
        }
    }
    return false;
}

const std::vector<Coordinate>& Ship::getCoordinates() const {
    return coordinates;
}

int Ship::getSize() const {
    return size;
}
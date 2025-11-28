#include "Ship.hpp"
#include <algorithm>
#include <stdexcept>

Ship::Ship(int size, const Coordinate& start, Orientation orientation, const std::string& name)
    : size(size), name(name), shipId(std::make_shared<int>(0)) {

    coordinates.resize(size);
    hits.resize(size, false);

    int dx = 0, dy = 0;
    if (orientation == Orientation::Horizontal) dx = 1;
    else dy = 1;

    for (int i = 0; i < size; ++i) {
        coordinates[i] = start + Coordinate(dx * i, dy * i);
    }
}

Ship::Ship(const Ship& other)
    : size(other.size), coordinates(other.coordinates),
    hits(other.hits), name(other.name),
    shipId(std::make_shared<int>(*(other.shipId))) {
}

Ship& Ship::operator=(const Ship& other) {
    if (this != &other) {
        size = other.size;
        coordinates = other.coordinates;
        hits = other.hits;
        name = other.name;
        shipId = std::make_shared<int>(*(other.shipId));
    }
    return *this;
}

Ship& Ship::operator=(const IShip& other) {
    this->name = other.getName() + " (assigned)";
    return *this;
}

bool Ship::isSunk() const {
    for (bool hit : hits) {
        if (!hit) return false;
    }
    return true;
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


Ship* Ship::shallowClone() const {
    Ship* clone = new Ship(*this);

    clone->shipId = this->shipId;
    return clone;
}

Ship* Ship::deepClone() const {
    return new Ship(*this); 
}

std::string Ship::getFullInfo() const {
    return getDescription() + " [ID: " + std::to_string(*shipId) + "]";
}

void Ship::setShipId(int id) {
    *shipId = id;
}

int Ship::getShipId() const {
    return *shipId;
}
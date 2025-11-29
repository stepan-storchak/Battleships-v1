#pragma once
#include "Coordinate.hpp"
#include <vector>
#include <string>

class IShip {
public:
    virtual ~IShip() = default;

    virtual const std::vector<Coordinate>& getCoordinates() const = 0;
    virtual int getSize() const = 0;
    virtual const std::string& getName() const = 0;
    virtual bool isSunk() const = 0;
    virtual bool takeHit(const Coordinate& coord) = 0;
    virtual std::string getFullInfo() const = 0;

    virtual std::string getType() const { return "Base Ship"; }
};
#pragma once
#include "Coordinate.hpp"
#include "Orientation.hpp"
#include "IShip.hpp"
#include <vector>
#include <string>

class Ship : public IShip {
private:
    int size;
    std::vector<Coordinate> coordinates;
    std::vector<bool> hits;
    std::string name;

public:
    Ship(int size, const Coordinate& start, Orientation orientation, const std::string& name = "");

    Ship(const Ship& other);

    Ship& operator=(const Ship& other);

    Ship& operator=(const IShip& other);

    friend std::string getShipInfo(const Ship& ship);

    const std::vector<Coordinate>& getCoordinates() const override { return coordinates; }
    int getSize() const override { return size; }
    const std::string& getName() const override { return name; }
    bool isSunk() const override;
    bool takeHit(const Coordinate& coord) override;
    std::string getFullInfo() const override;

    std::string getType() const override { return "Standard Ship"; }

    Ship* clone() const;

    std::string getDescription() const {
        return name + " (размер: " + std::to_string(size) + ", тип: " + getType() + ")";
    }
};

inline std::string getShipInfo(const Ship& ship) {
    return ship.getDescription();
}
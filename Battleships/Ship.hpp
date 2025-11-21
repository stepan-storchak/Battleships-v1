#pragma once
#include "Coordinate.hpp"
#include "Orientation.hpp"
#include <vector>
#include <string>

class Ship {
private:
    int size;                          
    std::vector<Coordinate> coordinates;
    std::vector<bool> hits;           
    std::string name;                  

public:
    Ship(int size, const Coordinate& start, Orientation orientation, const std::string& name = "");

    Ship(const Ship& other);

    bool operator==(const Ship& other) const;
    Ship& operator=(const Ship& other);

    friend std::string getShipInfo(const Ship& ship);

    const std::vector<Coordinate>& getCoordinates() const { return coordinates; }
    int getSize() const { return size; }
    const std::string& getName() const { return name; }

    bool isSunk() const;
    bool takeHit(const Coordinate& coord);

    std::string getFullInfo() const {
        return name + " (размер: " + std::to_string(size) + ")";
    }
};

inline std::string getShipInfo(const Ship& ship) {
    return ship.getFullInfo();
}
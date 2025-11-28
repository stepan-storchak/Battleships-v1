#pragma once
#include "Coordinate.hpp"
#include "Orientation.hpp"
#include "IShip.hpp"
#include <vector>
#include <string>
#include <memory>

class Ship : public IShip {
private:
    int size;
    std::vector<Coordinate> coordinates;
    std::vector<bool> hits;
    std::string name;
    std::shared_ptr<int> shipId; 

public:
    Ship(int size, const Coordinate& start, Orientation orientation, const std::string& name = "");

   
    Ship(const Ship& other);

    ~Ship() = default;

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

    Ship* shallowClone() const; 
    Ship* deepClone() const;    
    void setShipId(int id);
    int getShipId() const;

    std::string getDescription() const {
        return name + " (размер: " + std::to_string(size) + ", тип: " + getType() + ")";
    }
};

inline std::string getShipInfo(const Ship& ship) {
    return ship.getDescription();
}
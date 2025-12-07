#pragma once
#include <iostream>
#include <string>

class Coordinate   {
public:
    int x;
    int y;

    Coordinate(int x = 0, int y = 0) : x(x), y(y) {}
    Coordinate(const Coordinate& other) : x(other.x), y(other.y) {}


    bool operator==(const Coordinate& other) const {
        return x == other.x && y == other.y;
    }


    bool operator!=(const Coordinate& other) const {
        return !(*this == other);
    }

    Coordinate operator+(const Coordinate& other) const {
        return Coordinate(x + other.x, y + other.y);
    }

    Coordinate operator-(const Coordinate& other) const {
        return Coordinate(x - other.x, y - other.y);
    }

    Coordinate& operator=(const Coordinate& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
        }
        return *this;
    }

    
};

inline std::ostream& operator<<(std::ostream& os, const Coordinate& coord) {
    os << static_cast<char>('A' + coord.x) << coord.y + 1;
    return os;
}


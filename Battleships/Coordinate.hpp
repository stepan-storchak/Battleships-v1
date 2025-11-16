#ifndef COORDINATE_HPP
#define COORDINATE_HPP

struct Coordinate {
    int x;
    int y;

    Coordinate(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Coordinate& other) const {
        return x == other.x && y == other.y;
    }
};

#endif
#pragma once

/**
 * @class Coordinate
 * @brief  ласс дл€ представлени€ координат на игровом поле
 *
 * »нкапсулирует пару координат (x, y) и предоставл€ет оператор сравнени€
 * дл€ удобства работы с координатами в контейнерах STL
 */
class Coordinate {
public:
    int x; ///<  оордината по горизонтали (0-9)
    int y; ///<  оордината по вертикали (0-9)

    Coordinate(int x = 0, int y = 0) : x(x), y(y) {}

    /**
     * @brief ќператор сравнени€ координат
     */
    bool operator==(const Coordinate& other) const {
        return x == other.x && y == other.y;
    } 
};
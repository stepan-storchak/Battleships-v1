#pragma once
#include <iostream>
#include <string>

/**
 * @class Coordinate
 * @brief Класс для представления координат на игровом поле
 *
 * Инкапсулирует пару координат (x, y) и предоставляет оператор сравнения
 * для удобства работы с координатами в контейнерах STL.
 * Демонстрирует перегрузку операторов и конструктор копирования.
 */
class Coordinate {
public:
    int x;  ///< Координата по горизонтали (0-9)
    int y;  ///< Координата по вертикали (0-9)

    // Конструкторы
    Coordinate(int x = 0, int y = 0) : x(x), y(y) {}

    // Конструктор копирования (требование лабораторной)
    Coordinate(const Coordinate& other) : x(other.x), y(other.y) {}

    // Перегрузка операторов (требование лабораторной - 6 операторов)
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

    // Оператор присваивания
    Coordinate& operator=(const Coordinate& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
        }
        return *this;
    }

    // Метод для преобразования в строку (работа с std::string)
    std::string toString() const {
        return std::string(1, 'A' + x) + std::to_string(y + 1);
    }
};

// Дружественная функция для вывода (требование лабораторной)
inline std::ostream& operator<<(std::ostream& os, const Coordinate& coord) {
    os << static_cast<char>('A' + coord.x) << coord.y + 1;
    return os;
}

// Дружественная функция для преобразования в строку (требование лабораторной)
inline std::string coordinateToString(const Coordinate& coord) {
    return coord.toString();
}
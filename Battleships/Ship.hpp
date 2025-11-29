#pragma once
#include "Coordinate.hpp"
#include "Orientation.hpp"
#include <vector>
#include <string>

/**
 * @class Ship
 * @brief Класс для представления корабля в игре
 *
 * Инкапсулирует логику корабля: размер, координаты, состояние попаданий.
 * Демонстрирует перегрузку операторов и конструктор копирования.
 */
class Ship {
private:
    int size;                           ///< Размер корабля (1-4)
    std::vector<Coordinate> coordinates; ///< Координаты всех палуб корабля
    std::vector<bool> hits;             ///< Состояние попаданий в палубы
    std::string name;                   ///< Имя корабля (используем std::string)

public:
    Ship(int size, const Coordinate& start, Orientation orientation, const std::string& name = "");

    // Конструктор копирования (требование лабораторной)
    Ship(const Ship& other);

    // Перегрузка операторов (требование лабораторной)
    bool operator==(const Ship& other) const;
    Ship& operator=(const Ship& other);

    // Дружественная функция (требование лабораторной)
    friend std::string getShipInfo(const Ship& ship);

    // Геттеры
    const std::vector<Coordinate>& getCoordinates() const { return coordinates; }
    int getSize() const { return size; }
    const std::string& getName() const { return name; }

    bool isSunk() const;
    bool takeHit(const Coordinate& coord);

    // Работа со std::string - конкатенация (требование лабораторной)
    std::string getFullInfo() const {
        return name + " (размер: " + std::to_string(size) + ")";
    }
};

// Дружественная функция для вывода информации о корабле
inline std::string getShipInfo(const Ship& ship) {
    return ship.getFullInfo();
}
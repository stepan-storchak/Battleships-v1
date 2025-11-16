#ifndef SHIP_HPP
#define SHIP_HPP

#include "Coordinate.hpp"
#include <vector>

/**
 * @enum Orientation
 * @brief Перечисление ориентаций корабля
 */
enum class Orientation { Horizontal, Vertical };

/**
 * @class Ship
 * @brief Класс для представления корабля в игре
 *
 * Инкапсулирует состояние корабля: размер, координаты, ориентацию и попадания.
 * Реализует логику проверки уничтожения корабля и обработки попаданий.
 */
class Ship {
private:
    int size;                           ///< Количество палуб корабля
    std::vector<Coordinate> coordinates;///< Координаты всех палуб корабля
    std::vector<bool> hits;             ///< Вектор попаданий по палубам
    Orientation orientation;            ///< Ориентация корабля

public:
    /**
     * @brief Конструктор корабля
     * @param size Размер корабля (1-4 палубы)
     * @param startCoord Начальная координата корабля
     * @param orientation Ориентация корабля
     */
    Ship(int size, const Coordinate& startCoord, Orientation orientation);

    /**
     * @brief Проверяет, уничтожен ли корабль
     * @return true если все палубы подбиты
     */
    bool isSunk() const;

    /**
     * @brief Обрабатывает попадание в корабль
     * @param coord Координата попадания
     * @return true если попадание привело к уничтожению корабля
     */
    bool takeHit(const Coordinate& coord);

    // Геттеры для доступа к приватным полям
    const std::vector<Coordinate>& getCoordinates() const { return coordinates; }
    int getSize() const { return size; }
    Orientation getOrientation() const { return orientation; }
};

#endif
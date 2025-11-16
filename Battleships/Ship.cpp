#include "Ship.hpp"
#include <algorithm>

/**
 * @brief Конструктор корабля - инициализирует координаты на основе размера и ориентации
 */
Ship::Ship(int size, const Coordinate& startCoord, Orientation orientation)
    : size(size), orientation(orientation) {
    coordinates.reserve(size);
    hits.resize(size, false);

    // Генерация координат всех палуб корабля
    for (int i = 0; i < size; ++i) {
        if (orientation == Orientation::Horizontal) {
            coordinates.push_back(Coordinate(startCoord.x + i, startCoord.y));
        }
        else {
            coordinates.push_back(Coordinate(startCoord.x, startCoord.y + i));
        }
    }
}

/**
 * @brief Проверяет состояние корабля - уничтожен или нет
 * @return true если все палубы получили попадания
 */
bool Ship::isSunk() const {
    for (bool hit : hits) {
        if (!hit) return false;
    }
    return true;
}

/**
 * @brief Обрабатывает попадание в корабль
 * @param coord Координата, по которой произведен выстрел
 * @return true если это попадание привело к уничтожению корабля
 */
bool Ship::takeHit(const Coordinate& coord) {
    for (size_t i = 0; i < coordinates.size(); ++i) {
        if (coordinates[i] == coord) {
            hits[i] = true;
            return isSunk();
        }
    }
    return false;
}
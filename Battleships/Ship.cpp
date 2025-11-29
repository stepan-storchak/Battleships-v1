#include "Ship.hpp"
#include <algorithm>
#include <stdexcept>

/**
 * @brief Конструктор корабля
 * @param size Размер корабля
 * @param start Начальная координата
 * @param orientation Ориентация корабля
 * @param name Имя корабля
 */
Ship::Ship(int size, const Coordinate& start, Orientation orientation, const std::string& name)
    : size(size), name(name) {

    coordinates.resize(size);
    hits.resize(size, false);

    // Определяем направление размещения корабля
    int dx = 0, dy = 0;
    if (orientation == Orientation::Horizontal) dx = 1;
    else dy = 1;

    // Заполняем координаты всех палуб корабля
    for (int i = 0; i < size; ++i) {
        coordinates[i] = start + Coordinate(dx * i, dy * i);
    }
}

/**
 * @brief Конструктор копирования (требование лабораторной)
 * @param other Корабль для копирования
 */
Ship::Ship(const Ship& other)
    : size(other.size), coordinates(other.coordinates),
    hits(other.hits), name(other.name) {
}

/**
 * @brief Перегрузка оператора сравнения (требование лабораторной)
 * @param other Другой корабль для сравнения
 * @return true если корабли идентичны
 */
bool Ship::operator==(const Ship& other) const {
    if (this->coordinates.size() != other.coordinates.size()) return false;
    for (size_t i = 0; i < this->coordinates.size(); ++i) {
        if (this->coordinates[i] != other.coordinates[i]) return false;
    }
    return this->size == other.size;
}

/**
 * @brief Перегрузка оператора присваивания (требование лабораторной)
 * @param other Корабль для копирования
 * @return Ссылка на текущий объект
 */
Ship& Ship::operator=(const Ship& other) {
    if (this != &other) {
        size = other.size;
        coordinates = other.coordinates;
        hits = other.hits;
        name = other.name;
    }
    return *this;
}

/**
 * @brief Проверяет, уничтожен ли корабль
 * @return true если все палубы поражены
 */
bool Ship::isSunk() const {
    for (bool hit : hits) {
        if (!hit) return false;
    }
    return true;
}

/**
 * @brief Обрабатывает попадание в корабль
 * @param coord Координата попадания
 * @return true если корабль уничтожен после этого попадания
 */
bool Ship::takeHit(const Coordinate& coord) {
    for (int i = 0; i < size; ++i) {
        if (coordinates[i] == coord) {  // Используем перегруженный оператор ==
            hits[i] = true;
            return isSunk();
        }
    }
    return false;
}
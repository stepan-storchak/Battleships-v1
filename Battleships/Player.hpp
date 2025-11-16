#pragma once

#include "GameBoard.hpp"
#include "Coordinate.hpp"
#include "ShotResult.hpp"
#include "Ship.hpp"
#include <string>
#include <vector>
#include <memory>

/**
 * @class Player
 * @brief Абстрактный базовый класс для представления игрока
 *
 * Определяет общий интерфейс для всех типов игроков (человек, ИИ).
 * Реализует паттерн Template Method - общая логика в базовом классе,
 * специфическая реализация в производных классах.
 * Демонстрирует статические поля и методы, использование this.
 */
class Player {
protected:
    std::string name;           ///< Имя игрока
    GameBoard myBoard;          ///< Собственное поле игрока с кораблями
    GameBoard enemyBoard;       ///< Поле противника для отслеживания выстрелов
    std::vector<Ship> ships;    ///< Коллекция кораблей игрока
    static int playerCount;     ///< Статическое поле для подсчета игроков

public:
    Player(const std::string& name);
    virtual ~Player() = default;

    // Статический метод (требование лабораторной)
    static int getPlayerCount() { return playerCount; }

    // Абстрактные методы, которые должны быть реализованы в производных классах
    virtual void placeShips() = 0;
    virtual void makeMove(Player& enemy) = 0;
    virtual bool makeMoveWithResult(Player& enemy) = 0;
    virtual void markAreaAroundDestroyedShip(Player& enemy, const Coordinate& hitCoord) = 0;

    // Общие методы для всех игроков
    bool allShipsSunk() const;
    ShotResult getShotResult(const Coordinate& coord);

    // Геттеры с использованием this для ясности (требование лабораторной)
    const std::string& getName() const { return this->name; }
    void setName(const std::string& name) { this->name = name; }

    const GameBoard& getMyBoard() const { return this->myBoard; }
    const GameBoard& getEnemyBoard() const { return this->enemyBoard; }
    const std::vector<Ship>& getShips() const { return this->ships; }
    void addShip(const Ship& ship);
};
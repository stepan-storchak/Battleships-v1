#pragma once
#include "Player.hpp"
#include "Color.hpp"
#include <iostream>

/**
 * @class HumanPlayer
 * @brief Класс для представления игрока-человека
 *
 * Наследует от Player и реализует интерфейс для взаимодействия
 * с пользователем через консоль. Реализует паттерн Strategy
 * для различных стратегий расстановки кораблей.
 * Демонстрирует вызов конструктора базового класса.
 */
class HumanPlayer : public Player {
public:
    // Вызов конструктора базового класса (требование лабораторной)
    HumanPlayer(const std::string& name) : Player(name) {}

    void placeShips() override;
    void makeMove(Player& enemy) override;
    bool makeMoveWithResult(Player& enemy) override;
    void markAreaAroundDestroyedShip(Player& enemy, const Coordinate& hitCoord) override;

private:
    Coordinate inputCoordinate() const;
    void manualPlacement();
    bool automaticPlacement();
    void markSurroundingCells(const Coordinate& center);
    bool isValidShipPlacement(int size, const Coordinate& start, Orientation orientation) const;
};
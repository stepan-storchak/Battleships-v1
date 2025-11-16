#pragma once

#include "CellState.hpp"
#include "Coordinate.hpp"
#include "Ship.hpp"
#include "ShotResult.hpp"
#include <vector>

/**
 * @class GameBoard
 * @brief Класс для представления игрового поля
 *
 * Инкапсулирует логику игрового поля: размещение кораблей, обработку выстрелов,
 * валидацию расстановки и визуализацию. Реализует паттерн Model в архитектуре MVC.
 */
class GameBoard {
private:
    std::vector<std::vector<CellState>> grid; ///< Двумерная матрица состояний клеток
    static const int BOARD_SIZE = 10;         ///< Размер поля (10x10)

public:
    GameBoard();

    /**
     * @brief Проверяет возможность размещения корабля
     * @param ship Корабль для проверки
     * @return true если корабль можно разместить
     */
    bool isValidPlacement(const Ship& ship) const;

    /**
     * @brief Размещает корабль на поле
     * @param ship Корабль для размещения
     * @return true если размещение успешно
     */
    bool placeShip(const Ship& ship);

    /**
     * @brief Обрабатывает выстрел по полю
     * @param coord Координата выстрела
     * @return Результат выстрела
     */
    ShotResult receiveShot(const Coordinate& coord);

    /**
     * @brief Проверяет, свободна ли клетка и окружающие клетки
     * @param coord Координата для проверки
     * @return true если клетка и соседи пусты
     */
    bool isCellEmpty(const Coordinate& coord) const;

    /**
     * @brief Помечает область вокруг уничтоженного корабля
     * @param ship Уничтоженный корабль
     */
    void markAreaAroundSunkShip(const Ship& ship);

    /**
     * @brief Отображает поле в консоли
     * @param showShips Флаг отображения кораблей
     */
    void display(bool showShips) const;

    /**
     * @brief Возвращает состояние клетки
     * @param coord Координата клетки
     * @return Состояние клетки
     */
    CellState getCellState(const Coordinate& coord) const;

    /**
     * @brief Устанавливает состояние клетки
     * @param coord Координата клетки
     * @param state Новое состояние
     */
    void setCellState(const Coordinate& coord, CellState state);

    /**
     * @brief Очищает поле (сбрасывает все клетки в Empty)
     */
    void clearBoard();

    static int getBoardSize() { return BOARD_SIZE; } ///< Геттер размера поля
};

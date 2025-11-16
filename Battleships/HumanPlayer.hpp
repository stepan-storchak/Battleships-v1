#ifndef HUMANPLAYER_HPP
#define HUMANPLAYER_HPP

#include "Player.hpp"

/**
 * @class HumanPlayer
 * @brief Класс для представления игрока-человека
 *
 * Наследует от Player и реализует интерфейс для взаимодействия
 * с пользователем через консоль. Реализует паттерн Strategy
 * для различных стратегий расстановки кораблей.
 */
class HumanPlayer : public Player {
public:
    HumanPlayer(const std::string& name);

    /**
     * @brief Реализация расстановки кораблей для человека
     * @override Предоставляет выбор между ручной и автоматической расстановкой
     */
    void placeShips() override;

    /**
     * @brief Реализация хода человека
     * @param enemy Противник для атаки
     * @override Запрашивает координаты у пользователя
     */
    void makeMove(Player& enemy) override;

    /**
     * @brief Совершает ход с возвратом результата
     * @param enemy Противник для атаки
     * @return true если выстрел был попаданием
     * @override Дополняет базовую логику пользовательским интерфейсом
     */
    bool makeMoveWithResult(Player& enemy) override;

    /**
     * @brief Маркирует область вокруг уничтоженного корабля противника
     * @param enemy Противник, чей корабль уничтожен
     * @param hitCoord Координата попадания
     * @override Обновляет поле противника для визуализации
     */
    void markAreaAroundDestroyedShip(Player& enemy, const Coordinate& hitCoord) override;

private:
    /**
     * @brief Ввод координат от пользователя
     * @return Валидные координаты для выстрела
     */
    Coordinate inputCoordinate() const;

    /**
     * @brief Ручная расстановка кораблей с пользовательским вводом
     */
    void manualPlacement();

    /**
     * @brief Автоматическая расстановка кораблей
     * @return true если расстановка успешна
     */
    bool automaticPlacement();

    /**
     * @brief Проверяет возможность размещения корабля
     * @param size Размер корабля
     * @param start Начальная координата
     * @param orientation Ориентация корабля
     * @return true если размещение допустимо
     */
    bool isValidShipPlacement(int size, const Coordinate& start, Orientation orientation) const;
};

#endif
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "GameBoard.hpp"
#include "Coordinate.hpp"
#include "ShotResult.hpp"
#include "Ship.hpp"
#include <string>
#include <vector>

/**
 * @class Player
 * @brief Абстрактный базовый класс для представления игрока
 *
 * Определяет общий интерфейс для всех типов игроков (человек, ИИ).
 * Реализует паттерн Template Method - общая логика в базовом классе,
 * специфическая реализация в производных классах.
 */
class Player {
protected:
    std::string name;           ///< Имя игрока
    GameBoard myBoard;          ///< Собственное поле игрока с кораблями
    GameBoard enemyBoard;       ///< Поле противника для отслеживания выстрелов
    std::vector<Ship> ships;    ///< Коллекция кораблей игрока

public:
    Player(const std::string& name);
    virtual ~Player() = default;

    /**
     * @brief Абстрактный метод расстановки кораблей
     * @pure Реализуется в производных классах
     */
    virtual void placeShips() = 0;

    /**
     * @brief Абстрактный метод совершения хода
     * @param enemy Противник, по которому производится выстрел
     * @pure Реализуется в производных классах
     */
    virtual void makeMove(Player& enemy) = 0;

    /**
     * @brief Абстрактный метод совершения хода с возвратом результата
     * @param enemy Противник для атаки
     * @return true если выстрел был попаданием
     * @pure Реализуется в производных классах
     */
    virtual bool makeMoveWithResult(Player& enemy) = 0;

    /**
     * @brief Проверяет, все ли корабли игрока уничтожены
     * @return true если флот полностью уничтожен
     */
    bool allShipsSunk() const;

    /**
     * @brief Обрабатывает выстрел противника по своему полю
     * @param coord Координата выстрела
     * @return Результат выстрела
     */
    ShotResult getShotResult(const Coordinate& coord);

    // Геттеры для доступа к состоянию игрока
    const std::string& getName() const { return name; }
    const GameBoard& getMyBoard() const { return myBoard; }
    const GameBoard& getEnemyBoard() const { return enemyBoard; }
    const std::vector<Ship>& getShips() const { return ships; }

    /**
     * @brief Добавляет корабль в коллекцию игрока
     * @param ship Корабль для добавления
     */
    void addShip(const Ship& ship);

    /**
     * @brief Абстрактный метод для маркировки области вокруг уничтоженного корабля
     * @param enemy Противник, чей корабль уничтожен
     * @param hitCoord Координата попадания
     * @pure Реализуется в производных классах
     */
    virtual void markAreaAroundDestroyedShip(Player& enemy, const Coordinate& hitCoord) = 0;
};

#endif
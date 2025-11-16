#include "Player.hpp"
#include <iostream>

/**
 * @brief Конструктор игрока - инициализирует имя и игровые поля
 */
Player::Player(const std::string& name) : name(name), myBoard(), enemyBoard() {}

/**
 * @brief Проверяет состояние флота игрока
 * @return true если все корабли имеют статус "уничтожен"
 */
bool Player::allShipsSunk() const {
    for (const auto& ship : ships) {
        if (!ship.isSunk()) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Обрабатывает входящую атаку противника
 * @param coord Координата атаки
 * @return Результат выстрела (промах/попадание/уничтожение)
 *
 * Реализует полный цикл обработки выстрела: обновление поля,
 * проверка попадания в корабли, обработка уничтожения корабля.
 */
ShotResult Player::getShotResult(const Coordinate& coord) {
    // Получаем результат выстрела от доски
    ShotResult result = myBoard.receiveShot(coord);

    // Если было попадание, проверяем какой корабль был поражен
    if (result == ShotResult::Hit) {
        for (auto& ship : ships) {
            // Проверяем, принадлежит ли координата этому кораблю
            for (const auto& shipCoord : ship.getCoordinates()) {
                if (shipCoord.x == coord.x && shipCoord.y == coord.y) {
                    // Попадание в корабль
                    bool wasSunk = ship.takeHit(coord);
                    if (wasSunk) {
                        result = ShotResult::Sunk;
                        // Помечаем область вокруг уничтоженного корабля на СВОЕМ поле
                        myBoard.markAreaAroundSunkShip(ship);
                    }
                    break;
                }
            }
            if (result == ShotResult::Sunk) break;
        }
    }

    return result;
}

/**
 * @brief Добавляет корабль в коллекцию игрока
 * @param ship Корабль для добавления
 */
void Player::addShip(const Ship& ship) {
    ships.push_back(ship);
}
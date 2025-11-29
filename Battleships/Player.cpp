#include "Player.hpp"
#include <iostream>
#include <stdexcept>

// Инициализация статического поля (требование лабораторной)
int Player::playerCount = 0;

/**
 * @brief Конструктор игрока - инициализирует имя и игровые поля
 * @param name Имя игрока
 *
 * Увеличивает статический счетчик игроков (требование лабораторной)
 */
Player::Player(const std::string& name) : name(name), myBoard(), enemyBoard() {
    playerCount++;  // Увеличиваем счетчик игроков
}

/**
 * @brief Проверяет состояние флота игрока
 * @return true если все корабли имеют статус "уничтожен"
 */
bool Player::allShipsSunk() const {
    for (const auto& ship : this->ships) {
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
 * Демонстрирует обработку исключений (требование лабораторной).
 */
ShotResult Player::getShotResult(const Coordinate& coord) {
    try {
        // Получаем результат выстрела от доски
        ShotResult result = this->myBoard.receiveShot(coord);

        // Если было попадание, проверяем какой корабль был поражен
        if (result == ShotResult::Hit) {
            for (auto& ship : this->ships) {
                // Проверяем, принадлежит ли координата этому кораблю
                for (const auto& shipCoord : ship.getCoordinates()) {
                    if (shipCoord == coord) {  // Используем перегруженный оператор ==
                        // Попадание в корабль
                        bool wasSunk = ship.takeHit(coord);
                        if (wasSunk) {
                            result = ShotResult::Sunk;
                            // Помечаем область вокруг уничтоженного корабля на СВОЕМ поле
                            this->myBoard.markAreaAroundSunkShip(ship);
                        }
                        break;
                    }
                }
                if (result == ShotResult::Sunk) break;
            }
        }
        return result;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при обработке выстрела: " << e.what() << std::endl;
        return ShotResult::Miss;
    }
}

/**
 * @brief Добавляет корабль в коллекцию игрока
 * @param ship Корабль для добавления
 */
void Player::addShip(const Ship& ship) {
    this->ships.push_back(ship);
}
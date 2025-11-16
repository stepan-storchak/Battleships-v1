#pragma once

#include "Player.hpp"
#include <vector>

/**
 * @class AIPlayer
 * @brief Класс для представления компьютерного противника
 *
 * Наследует от Player и реализует искусственный интеллект для игры.
 * Использует стратегию с двумя режимами: случайные выстрелы и прицельная охота.
 * Реализует паттерн State для переключения между режимами поведения.
 */
class AIPlayer : public Player {
private:
    Coordinate lastHit;                 ///< Координата последнего успешного попадания
    bool huntMode;                      ///< Флаг режима преследования раненого корабля
    std::vector<Coordinate> possibleTargets; ///< Очередь приоритетных целей для стрельбы

public:
    AIPlayer(const std::string& name = "Computer");

    /**
     * @brief Реализация автоматической расстановки кораблей для ИИ
     * @override Использует алгоритм случайной расстановки с валидацией
     */
    void placeShips() override;

    /**
     * @brief Реализация хода искусственного интеллекта
     * @param enemy Противник для атаки
     * @override Использует интеллектуальный выбор целей
     */
    void makeMove(Player& enemy) override;

    /**
     * @brief Совершает ход с возвратом результата и обновлением стратегии
     * @param enemy Противник для атаки
     * @return true если выстрел был попаданием
     * @override Дополняет логику хода стратегическим анализом
     */
    bool makeMoveWithResult(Player& enemy) override;

    /**
     * @brief Маркирует область вокруг уничтоженного корабля
     * @param enemy Противник, чей корабль уничтожен
     * @param hitCoord Координата попадания
     * @override Обновляет внутреннее представление поля противника
     */
    void markAreaAroundDestroyedShip(Player& enemy, const Coordinate& hitCoord) override;

private:
    /**
     * @brief Генерирует умный ход на основе текущей стратегии
     * @return Координата для выстрела
     */
    Coordinate generateSmartMove();

    /**
     * @brief Обновляет стратегию ИИ на основе результата выстрела
     * @param result Результат последнего выстрела
     * @param coord Координата последнего выстрела
     */
    void updateStrategy(const ShotResult& result, const Coordinate& coord);

    /**
     * @brief Генерирует возможные цели вокруг попадания
     * @param hitCoord Координата успешного попадания
     */
    void generatePossibleTargets(const Coordinate& hitCoord);
};

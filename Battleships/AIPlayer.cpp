#include "AIPlayer.hpp"
#include "Color.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <thread>
#include <chrono>

/**
 * @brief Конструктор ИИ-игрока - инициализирует генератор случайных чисел
 */
AIPlayer::AIPlayer(const std::string& name) : Player(name), huntMode(false) {
    std::srand(std::time(0));
}

/**
 * @brief Реализация алгоритма автоматической расстановки кораблей
 *
 * Использует алгоритм проб и ошибок с ограничением попыток.
 * Демонстрирует принцип инкапсуляции - скрывает сложность
 * алгоритма за простым интерфейсом.
 */
void AIPlayer::placeShips() {
    int attempts = 0;
    const int MAX_ATTEMPTS = 10;

    while (attempts < MAX_ATTEMPTS) {
        // Очищаем доску и корабли
        myBoard.clearBoard();
        ships.clear();

        int shipSizes[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
        bool success = true;

        for (int size : shipSizes) {
            bool placed = false;
            int placementAttempts = 0;

            while (!placed && placementAttempts < 100) {
                int x = std::rand() % 10;
                int y = std::rand() % 10;
                Orientation orientation = (std::rand() % 2 == 0) ? Orientation::Horizontal : Orientation::Vertical;

                Ship ship(size, Coordinate(x, y), orientation);
                if (myBoard.isValidPlacement(ship)) {
                    if (myBoard.placeShip(ship)) {
                        ships.push_back(ship);
                        placed = true;
                    }
                }
                placementAttempts++;
            }

            if (!placed) {
                success = false;
                break;
            }
        }

        if (success) {
            return;
        }

        attempts++;
    }

    // Если не удалось расставить, используем простой метод
    std::cout << "Компьютер не смог расставить корабли оптимально." << std::endl;
}

/**
 * @brief Делегирует выполнение хода методу с возвратом результата
 */
void AIPlayer::makeMove(Player& enemy) {
    makeMoveWithResult(enemy);
}

/**
 * @brief Реализация интеллектуального хода компьютера
 *
 * Сочетает два режима поведения:
 * - Hunt Mode: случайные выстрелы по непроверенным клеткам
 * - Target Mode: прицельная стрельба вокруг раненого корабля
 * Реализует конечный автомат для управления стратегией.
 */
bool AIPlayer::makeMoveWithResult(Player& enemy) {
    std::cout << "\n=== Ход компьютера ===" << std::endl;

    // Имитация "размышления" компьютера
    Color::setColor(Color::YELLOW);
    std::cout << "Компьютер думает";
    for (int i = 0; i < 3; ++i) {
        std::cout << ".";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << std::endl;
    Color::resetColor();

    Coordinate target;
    if (huntMode && !possibleTargets.empty()) {
        target = generateSmartMove();
    }
    else {
        // Случайный выстрел
        do {
            target.x = std::rand() % 10;
            target.y = std::rand() % 10;
        } while (enemyBoard.getCellState(target) != CellState::Empty);
    }

    Color::setColor(Color::BLUE);
    std::cout << "Компьютер стреляет в " << static_cast<char>('A' + target.x) << target.y + 1 << std::endl;
    Color::resetColor();

    ShotResult result = enemy.getShotResult(target);
    updateStrategy(result, target);

    bool wasHit = false;

    // Обновляем поле противника
    switch (result) {
    case ShotResult::Miss:
        enemyBoard.setCellState(target, CellState::Miss);
        Color::setColor(Color::BLUE);
        std::cout << "Промах!" << std::endl;
        Color::resetColor();
        wasHit = false;
        break;
    case ShotResult::Hit:
        enemyBoard.setCellState(target, CellState::Hit);
        Color::setColor(Color::YELLOW);
        std::cout << "Попадание!" << std::endl;
        Color::resetColor();
        wasHit = true;
        break;
    case ShotResult::Sunk:
        enemyBoard.setCellState(target, CellState::Hit);
        Color::setColor(Color::RED);
        std::cout << "Уничтожен корабль!" << std::endl;
        Color::resetColor();
        wasHit = true;

        // Закрашиваем область вокруг уничтоженного корабля на поле противника
        markAreaAroundDestroyedShip(enemy, target);
        break;
    }

    return wasHit;
}

/**
 * @brief Обновляет внутреннее представление поля после уничтожения корабля
 *
 * Реализует логику исключения бесперспективных целей из
 дальнейшего рассмотрения.
 */
void AIPlayer::markAreaAroundDestroyedShip(Player& enemy, const Coordinate& hitCoord) {
    // Находим уничтоженный корабль и закрашиваем область вокруг него
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            Coordinate around(hitCoord.x + dx, hitCoord.y + dy);
            if (around.x >= 0 && around.x < 10 && around.y >= 0 && around.y < 10) {
                // Помечаем клетку как промах на поле противника
                if (enemyBoard.getCellState(around) == CellState::Empty) {
                    enemyBoard.setCellState(around, CellState::Miss);
                }
            }
        }
    }

    // Дополнительно ищем все клетки уничтоженного корабля и закрашиваем вокруг них
    for (int radius = 1; radius <= 3; radius++) {
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                Coordinate around(hitCoord.x + dx, hitCoord.y + dy);
                if (around.x >= 0 && around.x < 10 && around.y >= 0 && around.y < 10) {
                    CellState state = enemyBoard.getCellState(around);
                    if (state == CellState::Hit) {
                        // Нашли часть корабля, закрашиваем вокруг нее
                        for (int dy2 = -1; dy2 <= 1; ++dy2) {
                            for (int dx2 = -1; dx2 <= 1; ++dx2) {
                                Coordinate around2(around.x + dx2, around.y + dy2);
                                if (around2.x >= 0 && around2.x < 10 && around2.y >= 0 && around2.y < 10) {
                                    if (enemyBoard.getCellState(around2) == CellState::Empty) {
                                        enemyBoard.setCellState(around2, CellState::Miss);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief Генерирует координаты для интеллектуальной атаки
 * @return Координата для выстрела
 *
 * Реализует стратегию приоритетного выбора целей:
 * - Сначала цели из очереди приоритетов
 * - Затем случайные непроверенные клетки
 */
Coordinate AIPlayer::generateSmartMove() {
    if (possibleTargets.empty()) {
        // Если нет приоритетных целей, возвращаем случайную
        Coordinate target;
        do {
            target.x = std::rand() % 10;
            target.y = std::rand() % 10;
        } while (enemyBoard.getCellState(target) != CellState::Empty);
        return target;
    }

    // Берем первую цель из списка и удаляем ее
    Coordinate target = possibleTargets.front();
    possibleTargets.erase(possibleTargets.begin());
    return target;
}

/**
 * @brief Адаптирует стратегию ИИ на основе результатов стрельбы
 *
 * Реализует конечный автомат с двумя состояниями:
 * - Hunt: поиск кораблей случайными выстрелами
 * - Target: прицельное уничтожение найденного корабля
 */
void AIPlayer::updateStrategy(const ShotResult& result, const Coordinate& coord) {
    if (result == ShotResult::Hit || result == ShotResult::Sunk) {
        huntMode = true;
        lastHit = coord;
        generatePossibleTargets(coord);
    }

    if (result == ShotResult::Sunk) {
        huntMode = false;
        possibleTargets.clear();

        // Помечаем область вокруг уничтоженного корабля
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                Coordinate around(coord.x + dx, coord.y + dy);
                if (around.x >= 0 && around.x < 10 && around.y >= 0 && around.y < 10) {
                    if (enemyBoard.getCellState(around) == CellState::Empty) {
                        enemyBoard.setCellState(around, CellState::Miss);
                    }
                }
            }
        }
    }
}

/**
 * @brief Генерирует кандидатов для прицельной стрельбы
 *
 * Создает очередь целей в четырех направлениях от попадания
 * для определения ориентации и продолжения корабля.
 */
void AIPlayer::generatePossibleTargets(const Coordinate& hitCoord) {
    // Генерируем возможные цели вокруг попадания
    std::vector<Coordinate> directions = {
        Coordinate(1, 0), Coordinate(-1, 0), Coordinate(0, 1), Coordinate(0, -1)
    };

    for (const auto& dir : directions) {
        Coordinate newTarget(hitCoord.x + dir.x, hitCoord.y + dir.y);
        if (newTarget.x >= 0 && newTarget.x < 10 && newTarget.y >= 0 && newTarget.y < 10) {
            if (enemyBoard.getCellState(newTarget) == CellState::Empty) {
                possibleTargets.push_back(newTarget);
            }
        }
    }
}
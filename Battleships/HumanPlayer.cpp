#include "HumanPlayer.hpp"
#include "Color.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <stdexcept>

namespace {
    const int shipSizes[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
    const int BOARD_SIZE = 10;
    const int MAX_SINGLE_PLACEMENT_TRIES = 100;
    const int AROUND_OFFSET = 1;
}

void HumanPlayer::placeShips() {
    std::cout << "Расстановка кораблей для игрока: " << this->name << std::endl;
    int choice;
    while (true) {
        std::cout << "Выберите способ расстановки:\n";
        Color::setColor(Color::GREEN);
        std::cout << "1 - Автоматическая расстановка\n";
        Color::setColor(Color::YELLOW);
        std::cout << "2 - Ручная расстановка\n";
        Color::resetColor();
        std::cout << "Ваш выбор: ";
        std::cin >> choice;
        if (choice == 1) {
            if (automaticPlacement()) {
                break;
            }
            else {
                Color::setColor(Color::RED);
                std::cout << "Автоматическая расстановка не удалась. Попробуйте ручную расстановку.\n";
                Color::resetColor();
            }
        }
        else if (choice == 2) {
            manualPlacement();
            break;
        }
        else {
            Color::setColor(Color::RED);
            std::cout << "Неверный выбор! Попробуйте снова.\n";
            Color::resetColor();
        }
    }
}

void HumanPlayer::makeMove(Player& enemy) {
    makeMoveWithResult(enemy);
}


bool HumanPlayer::makeMoveWithResult(Player& enemy) {
    std::cout << "\n=== Ход игрока " << this->name << " ===" << std::endl;

    Color::setColor(Color::GREEN);
    std::cout << "Ваше поле:" << std::endl;
    Color::resetColor();
    this->myBoard.display(true);

    Color::setColor(Color::BLUE);
    std::cout << "\nПоле противника:" << std::endl;
    Color::resetColor();
    this->enemyBoard.display(false);

    Coordinate target = inputCoordinate();
    ShotResult result = enemy.getShotResult(target);
    bool wasHit = false;

    switch (result) {
    case ShotResult::Miss:
        this->enemyBoard.setCellState(target, CellState::Miss);
        Color::setColor(Color::BLUE);
        std::cout << "Промах!" << std::endl;
        Color::resetColor();
        wasHit = false;
        break;
    case ShotResult::Hit:
        this->enemyBoard.setCellState(target, CellState::Hit);
        Color::setColor(Color::YELLOW);
        std::cout << "Попадание!" << std::endl;
        Color::resetColor();
        wasHit = true;
        break;
    case ShotResult::Sunk:
        this->enemyBoard.setCellState(target, CellState::Hit);
        Color::setColor(Color::RED);
        std::cout << "Уничтожен корабль!" << std::endl;
        Color::resetColor();
        wasHit = true;
        markAreaAroundDestroyedShip(enemy, target);
        break;
    }
    return wasHit;
}

void HumanPlayer::markSurroundingCells(const Coordinate& center) {
    for (int dy = -AROUND_OFFSET; dy <= AROUND_OFFSET; ++dy) {
        for (int dx = -AROUND_OFFSET; dx <= AROUND_OFFSET; ++dx) {
            Coordinate around(center.x + dx, center.y + dy);
            if (around.x >= 0 && around.x < BOARD_SIZE && around.y >= 0 && around.y < BOARD_SIZE) {
                if (this->enemyBoard.getCellState(around) == CellState::Empty) {
                    this->enemyBoard.setCellState(around, CellState::Miss);
                }
            }
        }
    }
}

void HumanPlayer::markAreaAroundDestroyedShip(Player& enemy, const Coordinate& hitCoord) {
    markSurroundingCells(hitCoord);
    for (int radius = 1; radius <= 3; radius++) {
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                Coordinate around(hitCoord.x + dx, hitCoord.y + dy);
                if (around.x >= 0 && around.x < BOARD_SIZE && around.y >= 0 && around.y < BOARD_SIZE) {
                    if (this->enemyBoard.getCellState(around) == CellState::Hit) {
                        markSurroundingCells(around);
                    }
                }
            }
        }
    }
}

Coordinate HumanPlayer::inputCoordinate() const {
    char letter;
    int number;
    while (true) {
        std::cout << "Введите координаты для выстрела (например, A1): ";
        std::cin >> letter >> number;
        int x = toupper(letter) - 'A';
        int y = number - 1;
        if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
            Coordinate coord(x, y);
            CellState state = this->enemyBoard.getCellState(coord);
            if (state == CellState::Empty) {
                return coord;
            }
            else {
                Color::setColor(Color::RED);
                std::cout << "Вы уже стреляли в эту клетку!" << std::endl;
                Color::resetColor();
            }
        }
        else {
            Color::setColor(Color::RED);
            std::cout << "Неверные координаты! Используйте формат A1-J10." << std::endl;
            Color::resetColor();
        }
    }
}

void HumanPlayer::manualPlacement() {
    const char* shipNames[] = { "четырехпалубный", "трехпалубный", "трехпалубный",
                              "двухпалубный", "двухпалубный", "двухпалубный",
                              "однопалубный", "однопалубный", "однопалубный", "однопалубный" };

    for (int i = 0; i < BOARD_SIZE; ++i) {
        int size = shipSizes[i];
        bool placed = false;

        while (!placed) {
            Color::setColor(Color::YELLOW);
            std::cout << "\nРазмещение " << shipNames[i] << " корабля (" << size << " палубы)" << std::endl;
            Color::resetColor();
            myBoard.display(true);

            std::cout << "Введите начальную координату (например, A1): ";
            char letter;
            int number;
            std::cin >> letter >> number;

            int x = toupper(letter) - 'A';
            int y = number - 1;

            if (size > 1) {
                char orient;
                std::cout << "Выберите ориентацию (H - горизонтально, V - вертикально): ";
                std::cin >> orient;

                Orientation orientation = (toupper(orient) == 'H') ? Orientation::Horizontal : Orientation::Vertical;

                Ship ship(size, Coordinate(x, y), orientation);
                if (isValidShipPlacement(size, Coordinate(x, y), orientation)) {
                    if (myBoard.placeShip(ship)) {
                        ships.push_back(ship);
                        placed = true;
                        Color::setColor(Color::GREEN);
                        std::cout << "Корабль размещен успешно!" << std::endl;
                        Color::resetColor();
                    }
                }
                else {
                    Color::setColor(Color::RED);
                    std::cout << "Неверное размещение корабля! Попробуйте снова." << std::endl;
                    Color::resetColor();
                }
            }
            else {
                Ship ship(size, Coordinate(x, y), Orientation::Horizontal);
                if (isValidShipPlacement(size, Coordinate(x, y), Orientation::Horizontal)) {
                    if (myBoard.placeShip(ship)) {
                        ships.push_back(ship);
                        placed = true;
                        Color::setColor(Color::GREEN);
                        std::cout << "Корабль размещен успешно!" << std::endl;
                        Color::resetColor();
                    }
                }
                else {
                    Color::setColor(Color::RED);
                    std::cout << "Неверное размещение корабля! Попробуйте снова." << std::endl;
                    Color::resetColor();
                }
            }
        }
    }
}

bool HumanPlayer::automaticPlacement() {
    int attempts = 0;
    const int MAX_ATTEMPTS = 5;

    while (attempts < MAX_ATTEMPTS) {
        myBoard.clearBoard();
        ships.clear();

        bool success = true;

        for (int size : shipSizes) {
            bool placed = false;
            int placementAttempts = 0;

            while (!placed && placementAttempts < MAX_SINGLE_PLACEMENT_TRIES) {
                int x = std::rand() % BOARD_SIZE;
                int y = std::rand() % BOARD_SIZE;
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
            Color::setColor(Color::GREEN);
            std::cout << "Автоматическая расстановка завершена!" << std::endl;
            Color::resetColor();
            myBoard.display(true);

            if (attempts > 0) {
                std::cout << "Потребовалось " << (attempts + 1) << " попыток для успешной расстановки." << std::endl;
            }

            return true;
        }

        attempts++;
    }

    Color::setColor(Color::RED);
    std::cout << "Не удалось автоматически расставить корабли после " << MAX_ATTEMPTS << " попыток." << std::endl;
    Color::resetColor();
    return false;
}

bool HumanPlayer::isValidShipPlacement(int size, const Coordinate& start, Orientation orientation) const {
    Ship tempShip(size, start, orientation);
    return myBoard.isValidPlacement(tempShip);
}
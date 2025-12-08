#include "AIPlayer.hpp"
#include "Color.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <thread>
#include <chrono>

AIPlayer::AIPlayer(const std::string& name) : Player(name), huntMode(false) {
    std::srand(std::time(0));
}

namespace {
    int attempts = 0;
    const int MAX_ATTEMPTS = 10;
    const int BOARD_SIZE = 10;
    const int MAX_SINGLE_PLACEMENT_TRIES = 100;
    const int THINKING_DOTS = 3;
    const int THINKING_DELAY_MS = 500;
    const int shipSizes[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
}

AIPlayer& AIPlayer::operator=(const AIPlayer& other) {
    if (this != &other) {
        this->name = other.name;
        this->myBoard = other.myBoard;
        this->enemyBoard = other.enemyBoard;
        this->ships = other.ships;
        this->lastHit = other.lastHit;
        this->huntMode = other.huntMode;
        this->possibleTargets = other.possibleTargets;
    }
    return *this;
}

void AIPlayer::placeShips() {
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
            return;
        }
        attempts++;
    }

    std::cout << "Компьютер не смог расставить корабли оптимально." << std::endl;
}

void AIPlayer::makeMove(Player& enemy) {
    makeMoveWithResult(enemy);
}

bool AIPlayer::makeMoveWithResult(Player& enemy) {
    std::cout << "\n=== Ход компьютера ===" << std::endl;

    Color::setColor(Color::YELLOW);
    std::cout << "Компьютер думает";
    for (int i = 0; i < THINKING_DOTS; ++i) {
        std::cout << ".";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(THINKING_DELAY_MS));
    }
    std::cout << std::endl;
    Color::resetColor();

    Coordinate target = generateSmartMove();

    Color::setColor(Color::BLUE);
    std::cout << "Компьютер стреляет в " << static_cast<char>('A' + target.x) << target.y + 1 << std::endl;
    Color::resetColor();

    ShotResult result = enemy.getShotResult(target);
    updateStrategy(result, target);

    bool wasHit = false;
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
        markAreaAroundDestroyedShip(enemy, target);
        break;
    }

    return wasHit;
}

void AIPlayer::markAreaAroundDestroyedShip(Player& enemy, const Coordinate& hitCoord) {
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            Coordinate around(hitCoord.x + dx, hitCoord.y + dy);
            if (around.x >= 0 && around.x < BOARD_SIZE && around.y >= 0 && around.y < BOARD_SIZE) {
                if (enemyBoard.getCellState(around) == CellState::Empty) {
                    enemyBoard.setCellState(around, CellState::Miss);
                }
            }
        }
    }
}

Coordinate AIPlayer::generateSmartMove() {
    if (!possibleTargets.empty()) {
        Coordinate target = possibleTargets.front();
        possibleTargets.erase(possibleTargets.begin());
        return target;
    }

    Coordinate target;
    do {
        target.x = std::rand() % BOARD_SIZE;
        target.y = std::rand() % BOARD_SIZE;
    } while (enemyBoard.getCellState(target) != CellState::Empty);

    return target;
}

void AIPlayer::updateStrategy(const ShotResult& result, const Coordinate& coord) {
    if (result == ShotResult::Hit || result == ShotResult::Sunk) {
        huntMode = true;
        lastHit = coord;
        generatePossibleTargets(coord);
    }

    if (result == ShotResult::Sunk) {
        huntMode = false;
        possibleTargets.clear();

        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                Coordinate around(coord.x + dx, coord.y + dy);
                if (around.x >= 0 && around.x < BOARD_SIZE && around.y >= 0 && around.y < BOARD_SIZE) {
                    if (enemyBoard.getCellState(around) == CellState::Empty) {
                        enemyBoard.setCellState(around, CellState::Miss);
                    }
                }
            }
        }
    }
}

void AIPlayer::generatePossibleTargets(const Coordinate& hitCoord) {
    std::vector<Coordinate> directions = {
        Coordinate(1, 0), Coordinate(-1, 0), Coordinate(0, 1), Coordinate(0, -1)
    };

    for (const auto& dir : directions) {
        Coordinate newTarget(hitCoord.x + dir.x, hitCoord.y + dir.y);
        if (newTarget.x >= 0 && newTarget.x < BOARD_SIZE && newTarget.y >= 0 && newTarget.y < BOARD_SIZE) {
            if (enemyBoard.getCellState(newTarget) == CellState::Empty) {
                possibleTargets.push_back(newTarget);
            }
        }
    }
}
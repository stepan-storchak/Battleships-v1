#include "AIPlayer.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

AIPlayer::AIPlayer(const std::string& name) : Player(name), huntMode(false) {
    std::srand(std::time(0));
}

void AIPlayer::placeShips() {
    int shipSizes[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

    for (int size : shipSizes) {
        bool placed = false;
        while (!placed) {
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
        }
    }
}

void AIPlayer::makeMove(Player& enemy) {
    Coordinate target;

    if (huntMode && !possibleTargets.empty()) {
        target = generateSmartMove();
    }
    else {
        do {
            target.x = std::rand() % 10;
            target.y = std::rand() % 10;
        } while (enemyBoard.getCellState(target) != CellState::Empty);
    }

    std::cout << "Компьютер стреляет в " << char('A' + target.x) << (target.y + 1) << std::endl;

    ShotResult result = enemy.getShotResult(target);
    updateStrategy(result, target);

    switch (result) {
    case ShotResult::Miss:
        std::cout << "Промах!" << std::endl;
        enemyBoard.setCellState(target, CellState::Miss);
        break;
    case ShotResult::Hit:
        std::cout << "Попадание!" << std::endl;
        enemyBoard.setCellState(target, CellState::Hit);
        break;
    case ShotResult::Sunk:
        std::cout << "Уничтожен корабль!" << std::endl;
        enemyBoard.setCellState(target, CellState::Hit);
        break;
    }
}

Coordinate AIPlayer::generateSmartMove() {
    if (possibleTargets.empty()) {
        Coordinate target;
        do {
            target.x = std::rand() % 10;
            target.y = std::rand() % 10;
        } while (enemyBoard.getCellState(target) != CellState::Empty);
        return target;
    }

    Coordinate target = possibleTargets.front();
    possibleTargets.erase(possibleTargets.begin());
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
    }
}

void AIPlayer::generatePossibleTargets(const Coordinate& hitCoord) {
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
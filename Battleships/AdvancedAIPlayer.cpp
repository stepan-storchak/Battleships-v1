#include "AdvancedAIPlayer.hpp"
#include "Color.hpp"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

AdvancedAIPlayer& AdvancedAIPlayer::operator=(const AdvancedAIPlayer& other) {
    if (this != &other) {
        AIPlayer::operator=(other);

        this->priorityTargets = other.priorityTargets;
        this->difficultyLevel = other.difficultyLevel;
    }
    return *this;
}

void AdvancedAIPlayer::placeShips() {
    AIPlayer::placeShips();

    if (difficultyLevel >= 2) {
        std::cout << "Продвинутый ИИ использует улучшенную стратегию размещения кораблей" << std::endl;
    }
}

bool AdvancedAIPlayer::makeMoveWithResult(Player& enemy) {
    bool result = AIPlayer::makeMoveWithResult(enemy);

    if (result && difficultyLevel >= 2) {
        analyzeEnemyPattern(enemy);
    }

    return result;
}

void AdvancedAIPlayer::markAreaAroundDestroyedShip(Player& enemy, const Coordinate& hitCoord) {
    AIPlayer::markAreaAroundDestroyedShip(enemy, hitCoord);

    updatePriorityTargets(hitCoord);
}

std::string AdvancedAIPlayer::getPlayerType() const {
    std::string baseType = AIPlayer::getPlayerType();
    return baseType + " (Advanced, Level: " + std::to_string(difficultyLevel) + ")";
}

void AdvancedAIPlayer::displayAdvancedInfo() const {
    std::cout << "=== Расширенная информация о продвинутом ИИ ===" << std::endl;
    std::cout << "Уровень сложности: " << difficultyLevel << std::endl;
    std::cout << "Приоритетных целей: " << priorityTargets.size() << std::endl;
    std::cout << "Режим охоты: " << (huntMode ? "Активен" : "Не активен") << std::endl;
}

void AdvancedAIPlayer::analyzeEnemyPattern(Player& enemy) {
    if (difficultyLevel >= 3) {
        std::cout << "Продвинутый ИИ анализирует стратегию противника..." << std::endl;
    }
}

Coordinate AdvancedAIPlayer::calculateOptimalShot() {
    if (!priorityTargets.empty()) {
        Coordinate target = priorityTargets.back();
        priorityTargets.pop_back();
        return target;
    }

    return AIPlayer::generateSmartMove();
}

void AdvancedAIPlayer::updatePriorityTargets(const Coordinate& hitCoord) {
    std::vector<Coordinate> directions = {
        Coordinate(2, 0), Coordinate(-2, 0), Coordinate(0, 2), Coordinate(0, -2),
        Coordinate(1, 1), Coordinate(1, -1), Coordinate(-1, 1), Coordinate(-1, -1)
    };

    for (const auto& dir : directions) {
        Coordinate newTarget = hitCoord + dir;
        if (newTarget.x >= 0 && newTarget.x < 10 && newTarget.y >= 0 && newTarget.y < 10) {
            if (isHighPriorityTarget(newTarget)) {
                priorityTargets.push_back(newTarget);
            }
        }
    }
}

bool AdvancedAIPlayer::isHighPriorityTarget(const Coordinate& coord) const {
    CellState state = enemyBoard.getCellState(coord);
    return state == CellState::Empty;
}
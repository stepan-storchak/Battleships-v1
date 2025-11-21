#pragma once

#include "AIPlayer.hpp"
#include "Color.hpp"
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

class AdvancedAIPlayer : public AIPlayer {
protected:
    std::vector<Coordinate> priorityTargets;
    int difficultyLevel;

public:
    AdvancedAIPlayer(const std::string& name = "Advanced Computer", int difficulty = 2)
        : AIPlayer(name), difficultyLevel(difficulty)
    {
    }

    AdvancedAIPlayer& operator=(const AdvancedAIPlayer& other) {
        if (this != &other) {
            AIPlayer::operator=(other);

            this->priorityTargets = other.priorityTargets;
            this->difficultyLevel = other.difficultyLevel;
        }
        return *this;
    }

    void placeShips() override {
        AIPlayer::placeShips();

        if (difficultyLevel >= 2) {
            std::cout << "Продвинутый ИИ использует улучшенную стратегию размещения кораблей" << std::endl;
        }
    }

    bool makeMoveWithResult(Player& enemy) override {
        bool result = AIPlayer::makeMoveWithResult(enemy);

        if (result && difficultyLevel >= 2) {
            analyzeEnemyPattern(enemy);
        }

        return result;
    }

    void markAreaAroundDestroyedShip(Player& enemy, const Coordinate& hitCoord) override {
        AIPlayer::markAreaAroundDestroyedShip(enemy, hitCoord);

        updatePriorityTargets(hitCoord);
    }

    std::string getPlayerType() const override {
        std::string baseType = AIPlayer::getPlayerType();
        return baseType + " (Advanced, Level: " + std::to_string(difficultyLevel) + ")";
    }

    void displayAdvancedInfo() const {
        std::cout << "=== Расширенная информация о продвинутом ИИ ===" << std::endl;
        std::cout << "Уровень сложности: " << difficultyLevel << std::endl;
        std::cout << "Приоритетных целей: " << priorityTargets.size() << std::endl;
        std::cout << "Режим охоты: " << (huntMode ? "Активен" : "Не активен") << std::endl;
    }

    void analyzeEnemyPattern(Player& enemy) {
        if (difficultyLevel >= 3) {
            std::cout << "Продвинутый ИИ анализирует стратегию противника..." << std::endl;
        }
    }

    Coordinate calculateOptimalShot() {
        if (!priorityTargets.empty()) {
            Coordinate target = priorityTargets.back();
            priorityTargets.pop_back();
            return target;
        }

        return AIPlayer::generateSmartMove();
    }

protected:
    void updatePriorityTargets(const Coordinate& hitCoord) {
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

    bool isHighPriorityTarget(const Coordinate& coord) const {
        CellState state = enemyBoard.getCellState(coord);
        return state == CellState::Empty;
    }
};
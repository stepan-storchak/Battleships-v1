#pragma once
#include "AIPlayer.hpp"
#include "Color.hpp"
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <list>
#include <numeric>

class AdvancedAIPlayer : public AIPlayer {
protected:
    std::list<Coordinate> priorityTargets;
    int difficultyLevel;
    std::vector<Coordinate> heatMap;

public:
    AdvancedAIPlayer(const AdvancedAIPlayer&) = delete;
    AdvancedAIPlayer(const std::string& name = "Advanced Computer", int difficulty = 2)
        : AIPlayer(name), difficultyLevel(difficulty) {
        initializeHeatMap();
    }

    AdvancedAIPlayer& operator=(const AdvancedAIPlayer& other) {
        if (this != &other) {
            AIPlayer::operator=(other);
            this->priorityTargets = other.priorityTargets;
            this->difficultyLevel = other.difficultyLevel;
            this->heatMap = other.heatMap;
        }
        return *this;
    }

    void placeShips() override {
        AIPlayer::placeShips();
        if (difficultyLevel >= 2) {
            std::cout << "Продвинутый ИИ использует улучшенную стратегию размещения кораблей" << std::endl;
            optimizeShipPlacement();
        }
    }

    bool makeMoveWithResult(Player& enemy) override {
        bool result = AIPlayer::makeMoveWithResult(enemy);
        if (result && difficultyLevel >= 2) {
            analyzeEnemyPattern(enemy);
            updateHeatMap(lastHit);
        }
        return result;
    }

    void markAreaAroundDestroyedShip(Player& enemy, const Coordinate& hitCoord) override {
        AIPlayer::markAreaAroundDestroyedShip(enemy, hitCoord);
        updatePriorityTargets(hitCoord);
        clearHeatMapAround(hitCoord);
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
        std::cout << "Тепловая карта активных зон: " << getHeatMapValue() << std::endl;
    }

    void analyzeEnemyPattern(Player& enemy) {
        if (difficultyLevel >= 3) {
            std::cout << "Продвинутый ИИ анализирует стратегию противника..." << std::endl;
            analyzeShotPattern(enemy);
        }
    }

    Coordinate calculateOptimalShot() {
        if (!priorityTargets.empty()) {
            Coordinate target = priorityTargets.front();
            priorityTargets.pop_front();
            return target;
        }

        if (!heatMap.empty() && difficultyLevel >= 3) {
            auto maxIt = std::max_element(heatMap.begin(), heatMap.end(),
                [](const Coordinate& a, const Coordinate& b) {
                    return a.x + a.y < b.x + b.y;
                });
            if (maxIt != heatMap.end() &&
                enemyBoard.getCellState(*maxIt) == CellState::Empty) {
                return *maxIt;
            }
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
                if (isHighPriorityTarget(newTarget) &&
                    !containsInList(priorityTargets, newTarget)) {
                    priorityTargets.push_back(newTarget);
                }
            }
        }

        priorityTargets.sort([](const Coordinate& a, const Coordinate& b) {
            return a.x + a.y > b.x + b.y;
            });
    }

    bool isHighPriorityTarget(const Coordinate& coord) const {
        CellState state = enemyBoard.getCellState(coord);
        return state == CellState::Empty;
    }

    void initializeHeatMap() {
        for (int y = 0; y < 10; ++y) {
            for (int x = 0; x < 10; ++x) {
                heatMap.push_back(Coordinate(x, y));
            }
        }
    }

    void updateHeatMap(const Coordinate& hit) {
        auto it = std::find(heatMap.begin(), heatMap.end(), hit);
        if (it != heatMap.end()) {
            heatMap.erase(it);
        }
    }

    void clearHeatMapAround(const Coordinate& center) {
        std::vector<Coordinate> toRemove;
        for (const auto& coord : heatMap) {
            if (std::abs(coord.x - center.x) <= 1 &&
                std::abs(coord.y - center.y) <= 1) {
                toRemove.push_back(coord);
            }
        }

        for (const auto& coord : toRemove) {
            auto it = std::find(heatMap.begin(), heatMap.end(), coord);
            if (it != heatMap.end()) {
                heatMap.erase(it);
            }
        }
    }

    void optimizeShipPlacement() {
        std::vector<Ship> sortedShips = ships;
        std::sort(sortedShips.begin(), sortedShips.end(),
            [](const Ship& a, const Ship& b) {
                return a.getSize() > b.getSize();
            });
    }

    void analyzeShotPattern(Player& enemy) {
        const std::vector<Ship>& enemyShips = enemy.getShips();
        if (!enemyShips.empty()) {
            auto maxShip = std::max_element(enemyShips.begin(), enemyShips.end(),
                [](const Ship& a, const Ship& b) {
                    return a.getSize() < b.getSize();
                });

            if (maxShip != enemyShips.end()) {
                std::cout << "Самый большой корабль противника: "
                    << maxShip->getSize() << " палуб" << std::endl;
            }
        }
    }

    bool containsInList(const std::list<Coordinate>& list, const Coordinate& coord) const {
        return std::find(list.begin(), list.end(), coord) != list.end();
    }

    int getHeatMapValue() const {
        int sum = 0;
        for (const auto& coord : heatMap) {
            sum += coord.x + coord.y;
        }
        return sum;
    }
};
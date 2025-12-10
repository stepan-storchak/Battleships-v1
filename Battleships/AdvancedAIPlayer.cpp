#include "AdvancedAIPlayer.hpp"
#include "Color.hpp"
#include <thread>


AdvancedAIPlayer::AdvancedAIPlayer(const std::string& name)
    : AIPlayer(name), isTrackingShip(false), currentTargetShip(nullptr) {
}

AdvancedAIPlayer::~AdvancedAIPlayer() {
    currentTargetShip = nullptr;
}

AdvancedAIPlayer& AdvancedAIPlayer::operator=(const AdvancedAIPlayer& other) {
    if (this != &other) {
        AIPlayer::operator=(other);
        this->priorityTargets = other.priorityTargets;
        this->isTrackingShip = other.isTrackingShip;
        this->firstHit = other.firstHit;
        this->shipDirectionCandidates = other.shipDirectionCandidates;
        this->currentTargetShip = other.currentTargetShip;
    }
    return *this;
}

void AdvancedAIPlayer::placeShips() {
    std::cin.clear();
    AIPlayer::placeShips();
}

bool AdvancedAIPlayer::makeMoveWithResult(Player& enemy) {
    std::cout << "\n=== Ход продвинутого ИИ ===" << std::endl;

    Color::setColor(Color::YELLOW);
    std::cout << "Продвинутый ИИ анализирует поле...";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << std::endl;
    Color::resetColor();

    Coordinate target;

    if (!priorityTargets.empty()) {
        target = priorityTargets.front();
        priorityTargets.pop_front();
    }
    else {
        target = calculateOptimalShot();
    }

    Color::setColor(Color::BLUE);
    std::cout << "Продвинутый ИИ стреляет в " << static_cast<char>('A' + target.x) << target.y + 1 << std::endl;
    Color::resetColor();

    ShotResult result = enemy.getShotResult(target);

    if (result == ShotResult::Hit || result == ShotResult::Sunk) {
        enemyBoard.setCellState(target, CellState::Hit);

        if (!isTrackingShip) {
            isTrackingShip = true;
            firstHit = target;
            findAndSetCurrentShip(enemy, target);

            if (currentTargetShip) {
                generateShipCompletionTargets();
            }
        }
        else if (currentTargetShip) {
            generateShipCompletionTargets();
        }

        if (result == ShotResult::Sunk) {
            Color::setColor(Color::RED);
            std::cout << "Уничтожен корабль!" << std::endl;
            Color::resetColor();
            markAreaAroundDestroyedShip(enemy, target);
            clearTrackingData();
            return true;
        }
        else {
            Color::setColor(Color::YELLOW);
            std::cout << "Попадание!" << std::endl;
            Color::resetColor();
            return true;
        }
    }
    else {
        enemyBoard.setCellState(target, CellState::Miss);
        Color::setColor(Color::BLUE);
        std::cout << "Промах!" << std::endl;
        Color::resetColor();

        if (isTrackingShip && currentTargetShip) {
            generateShipCompletionTargets();
        }

        return false;
    }
}

void AdvancedAIPlayer::markAreaAroundDestroyedShip(Player& enemy, const Coordinate& hitCoord) {
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            Coordinate around(hitCoord.x + dx, hitCoord.y + dy);
            if (around.x >= 0 && around.x < 10 && around.y >= 0 && around.y < 10) {
                if (enemyBoard.getCellState(around) == CellState::Empty) {
                    enemyBoard.setCellState(around, CellState::Miss);
                }
            }
        }
    }

    priorityTargets.clear();
}

std::string AdvancedAIPlayer::getPlayerType() const {
    return "Advanced AI Player";
}

Coordinate AdvancedAIPlayer::calculateOptimalShot() {
    Coordinate target;
    do {
        target.x = std::rand() % 10;
        target.y = std::rand() % 10;
    } while (enemyBoard.getCellState(target) != CellState::Empty);

    return target;
}

void AdvancedAIPlayer::findAndSetCurrentShip(Player& enemy, const Coordinate& hitCoord) {
    const std::vector<Ship>& enemyShips = enemy.getShips();

    for (const Ship& ship : enemyShips) {
        const std::vector<Coordinate>& shipCoords = ship.getCoordinates();

        for (const Coordinate& coord : shipCoords) {
            if (coord == hitCoord && !ship.isSunk()) {
                currentTargetShip = const_cast<Ship*>(&ship);
                return;
            }
        }
    }

    currentTargetShip = nullptr;
}

void AdvancedAIPlayer::generateShipCompletionTargets() {
    if (!currentTargetShip || currentTargetShip->isSunk()) {
        return;
    }

    const std::vector<Coordinate>& shipCoords = currentTargetShip->getCoordinates();

    for (const Coordinate& coord : shipCoords) {
        if (enemyBoard.getCellState(coord) == CellState::Empty) {
            bool alreadyInList = false;
            for (const Coordinate& existing : priorityTargets) {
                if (existing == coord) {
                    alreadyInList = true;
                    break;
                }
            }

            if (!alreadyInList) {
                priorityTargets.push_back(coord);
            }
        }
    }
}

void AdvancedAIPlayer::clearTrackingData() {
    isTrackingShip = false;
    currentTargetShip = nullptr;
    priorityTargets.clear();
}


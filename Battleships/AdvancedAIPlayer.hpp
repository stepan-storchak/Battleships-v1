#pragma once

#include "AIPlayer.hpp"
#include <vector>
#include <list>

class AdvancedAIPlayer : public AIPlayer {
protected:
    std::list<Coordinate> priorityTargets;
    bool isTrackingShip;
    Coordinate firstHit;
    std::vector<Coordinate> shipDirectionCandidates;
    Ship* currentTargetShip;

public:
    AdvancedAIPlayer(const AdvancedAIPlayer&) = delete;
    bool isAdvancedAI() const override { return true; }
    AdvancedAIPlayer(const std::string& name = "Advanced Computer");
    ~AdvancedAIPlayer();

    AdvancedAIPlayer& operator=(const AdvancedAIPlayer& other);
    void placeShips() override;
    bool makeMoveWithResult(Player& enemy) override;
    void markAreaAroundDestroyedShip(Player& enemy, const Coordinate& hitCoord) override;
    std::string getPlayerType() const override;

protected:
    Coordinate calculateOptimalShot();
    void findAndSetCurrentShip(Player& enemy, const Coordinate& hitCoord);
    void generateShipCompletionTargets();
    void clearTrackingData();
};
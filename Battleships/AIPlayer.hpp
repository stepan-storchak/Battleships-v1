#pragma once

#include "Player.hpp"
#include <vector>

class AIPlayer : public Player {
protected:
    Coordinate lastHit;
    bool huntMode;
    std::vector<Coordinate> possibleTargets;

public:
    AIPlayer(const std::string& name = "Computer");
    bool isHuman() const override { return false; }
    void placeShips() override;
    void makeMove(Player& enemy) override;
    bool makeMoveWithResult(Player& enemy) override;
    void markAreaAroundDestroyedShip(Player& enemy, const Coordinate& hitCoord) override;
    std::string getPlayerType() const override { return "AI Player"; }
    AIPlayer& operator=(const AIPlayer& other);

protected:
    Coordinate generateSmartMove();
    void updateStrategy(const ShotResult& result, const Coordinate& coord);
    void generatePossibleTargets(const Coordinate& hitCoord);
};
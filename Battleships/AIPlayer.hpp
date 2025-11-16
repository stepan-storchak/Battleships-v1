#ifndef AIPLAYER_HPP
#define AIPLAYER_HPP

#include "Player.hpp"
#include <vector>

class AIPlayer : public Player {
private:
    Coordinate lastHit;
    bool huntMode;
    std::vector<Coordinate> possibleTargets;

public:
    AIPlayer(const std::string& name = "Computer");
    void placeShips() override;
    void makeMove(Player& enemy) override;

private:
    Coordinate generateSmartMove();
    void updateStrategy(const ShotResult& result, const Coordinate& coord);
    void generatePossibleTargets(const Coordinate& hitCoord);
};

#endif
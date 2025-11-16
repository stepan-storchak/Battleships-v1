#ifndef AIPLAYER_HPP
#define AIPLAYER_HPP

#include "Player.hpp"

class AIPlayer : public Player {
public:
    AIPlayer(const std::string& name = "Computer");
    void placeShips() override;
    void makeMove(Player& enemy) override;
};

#endif
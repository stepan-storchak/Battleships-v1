#ifndef HUMANPLAYER_HPP
#define HUMANPLAYER_HPP

#include "Player.hpp"

class HumanPlayer : public Player {
public:
    HumanPlayer(const std::string& name);
    void placeShips() override;
    void makeMove(Player& enemy) override;

private:
    void manualPlacement();
};

#endif
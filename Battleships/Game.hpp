#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include "HumanPlayer.hpp"
#include "AIPlayer.hpp"

class Game {
private:
    Player* player1;
    Player* player2;
    Player* currentPlayer;

public:
    Game();
    ~Game();
    void run();
    void startNewGame();
};

#endif
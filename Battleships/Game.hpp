#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include "HumanPlayer.hpp"
#include "AIPlayer.hpp"
#include "Leaderboard.hpp"

class Game {
private:
    Player* player1;
    Player* player2;
    Player* currentPlayer;
    Leaderboard leaderboard;

public:
    Game();
    ~Game();
    void run();
    void showMainMenu();
    void startNewGame();
    void showLeaderboard();
};

#endif
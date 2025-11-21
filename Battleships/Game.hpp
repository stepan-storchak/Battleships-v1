#pragma once

#include "Player.hpp"
#include "HumanPlayer.hpp"
#include "AIPlayer.hpp"
#include "Leaderboard.hpp"
#include <memory>

enum class GameState { Menu, Placement, Battle, GameOver, AfterGame };

class Game {
private:
    std::unique_ptr<Player> player1; 
    std::unique_ptr<Player> player2; 
    Player* currentPlayer;          
    GameState gameState;           
    Leaderboard leaderboard;          
    std::string winnerName;          

public:
    Game();
    ~Game() = default;  

    Player* getOpponent() const;
    void run();
    void showMainMenu();
    void switchTurn();
    bool checkWinCondition();
    void processMenuInput(int choice);
    void startNewGame();
    void showLeaderboard();
    void showAfterGameMenu();
    void processAfterGameInput(int choice);
};
#include "Game.hpp"
#include <iostream>

Game::Game() : player1(nullptr), player2(nullptr), currentPlayer(nullptr) {}

Game::~Game() {
    delete player1;
    delete player2;
}

void Game::run() {
    std::cout << "=== МОРСКОЙ БОЙ ===" << std::endl;
    startNewGame();
}

void Game::startNewGame() {
    // Создание игроков
    std::string playerName;
    std::cout << "Введите ваше имя: ";
    std::cin >> playerName;

    player1 = new HumanPlayer(playerName);
    player2 = new AIPlayer();

    currentPlayer = player1;

    // Фаза расстановки
    std::cout << "\n=== ФАЗА РАССТАНОВКИ ===" << std::endl;
    player1->placeShips();
    player2->placeShips();

    std::cout << "\n=== ИГРА НАЧИНАЕТСЯ ===" << std::endl;

    // Простой игровой цикл
    for (int turn = 0; turn < 5; ++turn) { // Ограничим для теста
        if (currentPlayer == player1) {
            player1->makeMove(*player2);
            currentPlayer = player2;
        }
        else {
            player2->makeMove(*player1);
            currentPlayer = player1;
        }
    }

    std::cout << "Демонстрация завершена!" << std::endl;
}
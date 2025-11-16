#include "Game.hpp"
#include <iostream>

Game::Game() : player1(nullptr), player2(nullptr), currentPlayer(nullptr) {}

Game::~Game() {
    delete player1;
    delete player2;
}

void Game::run() {
    std::cout << "=== МОРСКОЙ БОЙ ===" << std::endl;

    while (true) {
        std::cout << "\n1. Новая игра" << std::endl;
        std::cout << "2. Выход" << std::endl;
        std::cout << "Выберите: ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            startNewGame();
        }
        else {
            break;
        }
    }
}

void Game::startNewGame() {
    // Очистка предыдущих игроков
    delete player1;
    delete player2;

    // Создание новых игроков
    std::string playerName;
    std::cout << "Введите ваше имя: ";
    std::cin >> playerName;

    player1 = new HumanPlayer(playerName);
    player2 = new AIPlayer();
    currentPlayer = player1;

    // Фаза расстановки
    std::cout << "\n=== РАССТАНОВКА КОРАБЛЕЙ ===" << std::endl;
    player1->placeShips();
    player2->placeShips();

    // Боевая фаза
    std::cout << "\n=== НАЧАЛО БОЯ ===" << std::endl;

    while (!player1->allShipsSunk() && !player2->allShipsSunk()) {
        if (currentPlayer == player1) {
            player1->makeMove(*player2);
            if (!player2->allShipsSunk()) {
                currentPlayer = player2;
            }
        }
        else {
            player2->makeMove(*player1);
            if (!player1->allShipsSunk()) {
                currentPlayer = player1;
            }
        }
    }

    // Определение победителя
    if (player1->allShipsSunk()) {
        std::cout << "\nПобедил компьютер!" << std::endl;
    }
    else {
        std::cout << "\nПобедил " << player1->getName() << "!" << std::endl;
    }
}
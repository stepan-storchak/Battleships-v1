#include "Game.hpp"
#include <iostream>

Game::Game() : player1(nullptr), player2(nullptr), currentPlayer(nullptr) {}

Game::~Game() {
    delete player1;
    delete player2;
}

void Game::run() {
    while (true) {
        showMainMenu();
    }
}

void Game::showMainMenu() {
    std::cout << "\n=== МОРСКОЙ БОЙ ===" << std::endl;
    std::cout << "1. Новая игра" << std::endl;
    std::cout << "2. Таблица лидеров" << std::endl;
    std::cout << "3. Выход" << std::endl;
    std::cout << "Выберите: ";

    int choice;
    std::cin >> choice;

    switch (choice) {
    case 1:
        startNewGame();
        break;
    case 2:
        showLeaderboard();
        break;
    case 3:
        std::cout << "Выход из игры..." << std::endl;
        exit(0);
    default:
        std::cout << "Неверный выбор!" << std::endl;
    }
}

void Game::startNewGame() {
    delete player1;
    delete player2;

    std::string playerName;
    std::cout << "Введите ваше имя: ";
    std::cin >> playerName;

    player1 = new HumanPlayer(playerName);
    player2 = new AIPlayer();
    currentPlayer = player1;

    // Фаза расстановки
    player1->placeShips();
    player2->placeShips();

    // Боевая фаза
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

    // Определение победителя и сохранение результата
    if (player1->allShipsSunk()) {
        std::cout << "\nПобедил компьютер!" << std::endl;
        leaderboard.addWin("Computer");
    }
    else {
        std::cout << "\nПобедил " << player1->getName() << "!" << std::endl;
        leaderboard.addWin(player1->getName());
    }
}

void Game::showLeaderboard() {
    leaderboard.display();
    std::cout << "\nНажмите Enter для продолжения...";
    std::cin.ignore();
    std::cin.get();
}
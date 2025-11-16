#include "HumanPlayer.hpp"
#include <iostream>

HumanPlayer::HumanPlayer(const std::string& name) : Player(name) {}

void HumanPlayer::placeShips() {
    std::cout << "Игрок " << name << " расставляет корабли..." << std::endl;
    manualPlacement();
}

void HumanPlayer::makeMove(Player& enemy) {
    std::cout << "Ход игрока " << name << std::endl;
    // Заглушка для хода
}

void HumanPlayer::manualPlacement() {
    std::cout << "Ручная расстановка кораблей:" << std::endl;

    // Простая расстановка для тестирования
    int shipSizes[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

    for (int size : shipSizes) {
        // Временная заглушка - просто создаем корабли
        Ship ship(size, Coordinate(0, 0), Orientation::Horizontal);
        ships.push_back(ship);
    }

    std::cout << "Расстановка завершена!" << std::endl;
}
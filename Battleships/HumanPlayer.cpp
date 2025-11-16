#include "HumanPlayer.hpp"
#include <iostream>

HumanPlayer::HumanPlayer(const std::string& name) : Player(name) {}

void HumanPlayer::placeShips() {
    std::cout << "Игрок " << name << " расставляет корабли..." << std::endl;
    manualPlacement();
}

void HumanPlayer::makeMove(Player& enemy) {
    std::cout << "Ход игрока " << name << std::endl;

    // Простой ввод координат
    char letter;
    int number;
    std::cout << "Введите координаты (например A5): ";
    std::cin >> letter >> number;

    int x = toupper(letter) - 'A';
    int y = number - 1;

    Coordinate target(x, y);
    ShotResult result = enemy.getShotResult(target);

    switch (result) {
    case ShotResult::Miss: std::cout << "Промах!" << std::endl; break;
    case ShotResult::Hit: std::cout << "Попадание!" << std::endl; break;
    case ShotResult::Sunk: std::cout << "Уничтожен корабль!" << std::endl; break;
    }
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
#include "Game.hpp"
#include <iostream>
#include <Windows.h>
#include <memory>
#include <stdexcept>


int main() {
    try {
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);

        auto game = std::make_unique<Game>();
        game->run();

        std::cout << "\nСтатистика игры:" << std::endl;
        std::cout << "Количество игроков: " << Player::getPlayerCount() << std::endl;
        std::cout << "Использование цветов: " << Color::getUsageCount() << " раз" << std::endl;

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Неизвестная критическая ошибка" << std::endl;
        return 1;
    }
}
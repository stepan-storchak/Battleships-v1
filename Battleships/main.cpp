#include "Game.hpp"
#include <iostream>
#include <Windows.h>
#include <memory>
#include <stdexcept>

/**
 * @brief Точка входа в приложение "Морской бой"
 * @return Код завершения программы
 *
 * Создает экземпляр главного класса Game и запускает игровой цикл.
 * Реализует принцип минимализма - вся логика инкапсулирована в Game.
 * Демонстрирует обработку исключений на уровне приложения.
 */
int main() {
    try {
        // Настройка консоли для поддержки кириллицы
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);

        // Использование умного указателя для Game (требование лабораторной)
        auto game = std::make_unique<Game>();
        game->run();

        // Вывод статистики использования статических полей
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
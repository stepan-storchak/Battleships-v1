#include "Game.hpp"
#include "GameUtils.hpp"
#include "ShipContainer.hpp"
#include <iostream>
#include <Windows.h>
#include <memory>
#include <stdexcept>

void demonstrateSTLFeatures() {
    std::cout << "\n=== Демонстрация STL возможностей ===" << std::endl;

    std::vector<int> numbers = { 5, 2, 8, 1, 9, 3, 7, 4, 6 };

    int max = findMax(numbers);
    int min = findMin(numbers);
    int avg = calculateAverage(numbers);

    std::cout << "Числа: ";
    for (auto n : numbers) std::cout << n << " ";
    std::cout << "\nМаксимум: " << max << std::endl;
    std::cout << "Минимум: " << min << std::endl;
    std::cout << "Среднее: " << avg << std::endl;

    std::sort(numbers.begin(), numbers.end());
    std::cout << "Отсортированные числа: ";
    for (auto n : numbers) std::cout << n << " ";
    std::cout << std::endl;

    int target = 7;
    bool found = contains(numbers, target);
    std::cout << "Число " << target << (found ? " найдено" : " не найдено") << std::endl;

    std::cout << "Пример альтернативы variant: использование union или класса-обертки" << std::endl;
}

int main() {
    try {
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);

        demonstrateSTLFeatures();

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
#include <iostream>
#include "HumanPlayer.hpp"
#include "AIPlayer.hpp"
#include <Windows.h>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::cout << "Морской бой - добавлена иерархия игроков" << std::endl;

    // Демонстрация создания объектов
    HumanPlayer human("Игрок");
    AIPlayer computer;

    std::cout << "Созданы игроки: " << human.getName() << " и " << computer.getName() << std::endl;

    return 0;
}
#include <iostream>
#include "GameBoard.hpp"
#include "Ship.hpp"
#include <Windows.h>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::cout << "Морской бой - начало разработки" << std::endl;

    // Тест базовых структур
    GameBoard board;
    board.display(true);

    return 0;
}
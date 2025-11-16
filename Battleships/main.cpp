#include "Game.hpp"
#include <iostream>
#include <Windows.h>

/**
 * @brief Точка входа в приложение "Морской бой"
 * @return Код завершения программы
 *
 * Создает экземпляр главного класса Game и запускает игровой цикл.
 * Реализует принцип минимализма - вся логика инкапсулирована в Game.
 */

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Game game;
    game.run();

    return 0;
}
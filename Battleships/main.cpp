#include "Game.hpp"
#include <Windows.h>



int main() {
    try {
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);

        auto game = std::make_unique<Game>();
        game->run();

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
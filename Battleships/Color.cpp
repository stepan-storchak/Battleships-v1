#include "Color.hpp"
#include <iostream>
#include <stdexcept>

// Инициализация статического поля (требование лабораторной)
int Color::usageCount = 0;

/**
 * @brief Устанавливает цвет текста в консоли
 * @param color Код цвета для установки
 *
 * Обрабатывает исключения для надежности (требование лабораторной)
 */
void Color::setColor(int color) {
    try {
#ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
#else
        // ANSI коды для Linux/Mac
        switch (color) {
        case RED: std::cout << "\033[31m"; break;
        case GREEN: std::cout << "\033[32m"; break;
        case BLUE: std::cout << "\033[34m"; break;
        case YELLOW: std::cout << "\033[33m"; break;
        case WHITE: std::cout << "\033[37m"; break;
        case GRAY: std::cout << "\033[90m"; break;
        }
#endif
        usageCount++;  // Увеличиваем счетчик использований
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при установке цвета: " << e.what() << std::endl;
    }
}

/**
 * @brief Сбрасывает цвет текста к значениям по умолчанию
 *
 * Обрабатывает исключения для надежности (требование лабораторной)
 */
void Color::resetColor() {
    try {
#ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 7);  // Белый по умолчанию
#else
        std::cout << "\033[0m";
#endif
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при сбросе цвета: " << e.what() << std::endl;
    }
}

/**
 * @brief Конвертирует код цвета в читаемое имя
 * @param color Код цвета
 * @return Строковое представление цвета
 *
 * Демонстрирует работу с std::string (требование лабораторной)
 */
std::string Color::getColorName(int color) {
    switch (color) {
    case RED: return "Красный";
    case GREEN: return "Зеленый";
    case BLUE: return "Синий";
    case YELLOW: return "Желтый";
    case WHITE: return "Белый";
    case GRAY: return "Серый";
    default: return "Неизвестный";
    }
}
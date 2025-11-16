#pragma once
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

/**
 * @class Color
 * @brief Класс для управления цветовым оформлением консоли
 *
 * Реализует паттерн Utility Class - предоставляет статические методы
 * для кроссплатформенного управления цветом текста в консоли.
 * Демонстрирует статические поля и методы (требование лабораторной).
 */
class Color {
private:
    static int usageCount;  // Статическое поле для подсчета использований

public:
    static void setColor(int color);
    static void resetColor();

    // Статический метод (требование лабораторной)
    static int getUsageCount() { return usageCount; }
    static std::string getColorName(int color);

    // Цветовые константы для Windows
    static const int RED = 12;     ///< Красный цвет
    static const int GREEN = 10;   ///< Зеленый цвет  
    static const int BLUE = 9;     ///< Синий цвет
    static const int YELLOW = 14;  ///< Желтый цвет
    static const int WHITE = 15;   ///< Белый цвет
    static const int GRAY = 8;     ///< Серый цвет
};
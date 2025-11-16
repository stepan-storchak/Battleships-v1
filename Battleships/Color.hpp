#pragma once

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

/**
 * @class Color
 * @brief Класс для управления цветовым оформлением консоли
 *
 * Реализует паттерн Utility Class - предоставляет статические методы
 * для кроссплатформенного управления цветом текста в консоли.
 * Инкапсулирует платформенно-зависимые детали реализации.
 */
class Color {
public:
    
    static void setColor(int color);
    static void resetColor();

    // Цветовые константы для Windows
    static const int RED = 12;    ///< Красный цвет
    static const int GREEN = 10;  ///< Зеленый цвет
    static const int BLUE = 9;    ///< Синий цвет
    static const int YELLOW = 14; ///< Желтый цвет
    static const int WHITE = 15;  ///< Белый цвет
    static const int GRAY = 8;    ///< Серый цвет
};
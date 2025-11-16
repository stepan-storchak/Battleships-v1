#ifndef COLOR_HPP
#define COLOR_HPP

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

class Color {
public:
    static void setColor(int color) {
#ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
#else
        std::cout << "\033[1;" << color << "m";
#endif
    }

    static void resetColor() {
#ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 7);
#else
        std::cout << "\033[0m";
#endif
    }

    static const int RED = 12;
    static const int GREEN = 10;
    static const int BLUE = 9;
    static const int YELLOW = 14;
    static const int WHITE = 15;
};

#endif
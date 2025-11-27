#include "Color.hpp"
#include <iostream>
#include <stdexcept>

int Color::usageCount = 0;

void Color::setColor(int color) {
    try {
#ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
#else
        switch (color) {
        case RED: std::cout << "\033[31m"; break;
        case GREEN: std::cout << "\033[32m"; break;
        case BLUE: std::cout << "\033[34m"; break;
        case YELLOW: std::cout << "\033[33m"; break;
        case WHITE: std::cout << "\033[37m"; break;
        case GRAY: std::cout << "\033[90m"; break;
        }
#endif
        usageCount++;
    }
    catch (const std::exception& e) {
        std::cerr << "Îøèáêà ïðè óñòàíîâêå öâåòà: " << e.what() << std::endl;
    }
}


void Color::resetColor() {
    try {
#ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 7);  
#else
        std::cout << "\033[0m";
#endif
    }
    catch (const std::exception& e) {
        std::cerr << "Îøèáêà ïðè ñáðîñå öâåòà: " << e.what() << std::endl;
    }
}


std::string Color::getColorName(int color) {
    switch (color) {
    case RED: return "Êðàñíûé";
    case GREEN: return "Çåëåíûé";
    case BLUE: return "Ñèíèé";
    case YELLOW: return "Æåëòûé";
    case WHITE: return "Áåëûé";
    case GRAY: return "Ñåðûé";
    default: return "Íåèçâåñòíûé";
    }
}
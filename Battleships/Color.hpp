#pragma once
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif


class Color {
private:
    static int usageCount; 

public:
    static void setColor(int color);
    static void resetColor();

    static int getUsageCount() { return usageCount; }


    static const int RED = 12;     
    static const int GREEN = 10;   
    static const int BLUE = 9;    
    static const int YELLOW = 14; 
    static const int WHITE = 15;  
    static const int GRAY = 8;    
};
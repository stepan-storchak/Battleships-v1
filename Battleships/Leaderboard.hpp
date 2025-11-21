#pragma once
#include <string>
#include <map>
#include <vector>

class Leaderboard {
private:
    std::map<std::string, int> records;  
    const std::string filename = "leaderboard.txt";  

public:
    Leaderboard();
    void loadFromFile();
    void saveToFile();
    void addWin(const std::string& playerName);
    void display() const;
};
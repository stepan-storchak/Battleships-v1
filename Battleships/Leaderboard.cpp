#include "Leaderboard.hpp"
#include <iostream>
#include <fstream>

Leaderboard::Leaderboard() {
    loadFromFile();
}

void Leaderboard::loadFromFile() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return;
    }

    std::string name;
    int wins;
    while (file >> name >> wins) {
        records[name] = wins;
    }
    file.close();
}

void Leaderboard::saveToFile() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return;
    }

    for (const auto& record : records) {
        file << record.first << " " << record.second << std::endl;
    }
    file.close();
}

void Leaderboard::addWin(const std::string& playerName) {
    records[playerName]++;
    saveToFile();
}

void Leaderboard::display() const {
    std::cout << "\n=== ÒÀÁËÈÖÀ ËÈÄÅÐÎÂ ===" << std::endl;

    if (records.empty()) {
        std::cout << "Ïîêà íåò çàïèñåé." << std::endl;
        return;
    }

    for (const auto& record : records) {
        std::cout << record.first << ": " << record.second << " ïîáåä" << std::endl;
    }
}
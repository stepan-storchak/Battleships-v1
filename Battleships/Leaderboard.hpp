#pragma once
#include <string>
#include <map>
#include <vector>

/**
 * @class Leaderboard
 * @brief Класс для управления таблицей рекордов
 *
 * Инкапсулирует логику работы с таблицей лидеров: загрузка из файла,
 * сохранение, добавление записей и отображение. Реализует паттерн
 * Repository для работы с постоянным хранилищем данных.
 */
class Leaderboard {
private:
    std::map<std::string, int> records;  ///< Ассоциативный массив "имя-победы"
    const std::string filename = "leaderboard.txt";  ///< Имя файла для сохранения

public:
    Leaderboard();
    void loadFromFile();
    void saveToFile();
    void addWin(const std::string& playerName);
    void display() const;
};
#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

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
    std::map<std::string, int> records; ///< Ассоциативный массив "имя-победы"
    const std::string filename = "leaderboard.txt"; ///< Имя файла для сохранения

public:
    Leaderboard();

    /**
     * @brief Загружает таблицу лидеров из файла
     */
    void loadFromFile();

    /**
     * @brief Сохраняет текущую таблицу в файл
     */
    void saveToFile();

    /**
     * @brief Добавляет победу игрока в таблицу
     * @param playerName Имя игрока-победителя
     */
    void addWin(const std::string& playerName);

    /**
     * @brief Отображает таблицу лидеров в отсортированном виде
     */
    void display() const;
};

#endif
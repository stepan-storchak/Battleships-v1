#include "Leaderboard.hpp"
#include "Color.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

/**
 * @brief Конструктор - автоматически загружает данные из файла
 */
Leaderboard::Leaderboard() {
    loadFromFile();
}

/**
 * @brief Загрузка данных из файлового хранилища
 *
 * Реализует устойчивость к ошибкам - если файл не существует,
 * создается пустая таблица лидеров.
 */
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

/**
 * @brief Сохранение данных в файловое хранилище
 *
 * Обеспечивает сохранение состояния между сеансами игры.
 * Реализует принцип инкапсуляции - скрывает формат хранения.
 */
void Leaderboard::saveToFile() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка сохранения таблицы лидеров!" << std::endl;
        return;
    }

    for (const auto& record : records) {
        file << record.first << " " << record.second << std::endl;
    }
    file.close();
}

/**
 * @brief Добавление записи о победе с автоматическим обновлением файла
 * @param playerName Имя победителя для обновления статистики
 *
 * Реализует инкрементальное обновление статистики и
 * обеспечивает согласованность данных в памяти и в хранилище.
 */
void Leaderboard::addWin(const std::string& playerName) {
    records[playerName]++;
    saveToFile();

    Color::setColor(Color::GREEN);
    std::cout << "Победа игрока " << playerName << " сохранена в таблице лидеров!" << std::endl;
    Color::resetColor();

    // Показываем обновленную таблицу
    display();
}

/**
 * @brief Отображение таблицы лидеров с цветовым оформлением
 *
 * Сортирует игроков по количеству побед и применяет цветовое
 * кодирование для первых трех мест (золото, серебро, бронза).
 */
void Leaderboard::display() const {
    std::cout << "\n";
    Color::setColor(Color::YELLOW);
    std::cout << "=== ТАБЛИЦА ЛИДЕРОВ ===" << std::endl;
    Color::resetColor();

    if (records.empty()) {
        std::cout << "Пока нет записей." << std::endl;
        return;
    }

    // Создаем вектор для сортировки
    std::vector<std::pair<std::string, int>> sortedRecords;
    for (const auto& record : records) {
        sortedRecords.push_back(record);
    }

    // Сортируем по количеству побед (по убыванию)
    for (size_t i = 0; i < sortedRecords.size() - 1; ++i) {
        for (size_t j = i + 1; j < sortedRecords.size(); ++j) {
            if (sortedRecords[i].second < sortedRecords[j].second) {
                std::swap(sortedRecords[i], sortedRecords[j]);
            }
        }
    }

    std::cout << "-------------------------" << std::endl;
    std::cout << "Игрок\t\tПобеды" << std::endl;
    std::cout << "-------------------------" << std::endl;

    for (size_t i = 0; i < sortedRecords.size(); ++i) {
        const auto& record = sortedRecords[i];

        // Разные цвета для первых трех мест
        if (i == 0) {
            Color::setColor(Color::YELLOW); // Золото
        }
        else if (i == 1) {
            Color::setColor(Color::GRAY);   // Серебро  
        }
        else if (i == 2) {
            Color::setColor(Color::RED);    // Бронза
        }
        else {
            Color::setColor(Color::WHITE);  // Обычный
        }

        std::cout << record.first;
        if (record.first.length() < 8) {
            std::cout << "\t\t";
        }
        else {
            std::cout << "\t";
        }
        std::cout << record.second << std::endl;
        Color::resetColor();
    }
    std::cout << "-------------------------" << std::endl;
}
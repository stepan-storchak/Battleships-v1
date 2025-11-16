#pragma once

#include "Player.hpp"
#include "HumanPlayer.hpp"
#include "AIPlayer.hpp"
#include "Leaderboard.hpp"
#include <memory>

/**
 * @enum GameState
 * @brief Перечисление состояний игрового процесса
 *
 * Определяет конечный автомат игры с состояниями:
 * - Menu: главное меню
 * - Placement: фаза расстановки кораблей
 * - Battle: фаза боя
 * - AfterGame: экран завершения игры
 * - GameOver: завершение приложения
 */
enum class GameState { Menu, Placement, Battle, GameOver, AfterGame };

/**
 * @class Game
 * @brief Главный класс приложения, управляющий игровым процессом
 *
 * Реализует паттерн Controller в архитектуре MVC - управляет
 * взаимодействием между моделью (игроки) и представлением (консоль).
 * Координирует переходы между состояниями игры.
 * Демонстрирует использование умных указателей (требование лабораторной).
 */
class Game {
private:
    std::unique_ptr<Player> player1;  ///< Умный указатель на первого игрока (человек)
    std::unique_ptr<Player> player2;  ///< Умный указатель на второго игрока (компьютер)
    Player* currentPlayer;            ///< Указатель на текущего активного игрока
    GameState gameState;              ///< Текущее состояние игрового процесса
    Leaderboard leaderboard;          ///< Объект для управления таблицей лидеров
    std::string winnerName;           ///< Имя победителя текущей игры

public:
    Game();
    ~Game() = default;  // Деструктор не нужен благодаря умным указателям

    Player* getOpponent() const;
    void run();
    void showMainMenu();
    void switchTurn();
    bool checkWinCondition();
    void processMenuInput(int choice);
    void startNewGame();
    void showLeaderboard();
    void showAfterGameMenu();
    void processAfterGameInput(int choice);
};
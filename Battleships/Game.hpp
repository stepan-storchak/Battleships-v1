#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include "HumanPlayer.hpp"
#include "AIPlayer.hpp"
#include "Leaderboard.hpp"

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
 */
class Game {
private:
    Player* player1;            ///< Указатель на первого игрока (человек)
    Player* player2;            ///< Указатель на второго игрока (компьютер)
    Player* currentPlayer;      ///< Указатель на текущего активного игрока
    GameState gameState;        ///< Текущее состояние игрового процесса
    Leaderboard leaderboard;    ///< Объект для управления таблицей лидеров
    std::string winnerName;     ///< Имя победителя текущей игры

    /**
     * @brief Возвращает указатель на противника текущего игрока
     * @return Указатель на игрока-оппонента
     */
    Player* getOpponent() const;

public:
    Game();
    ~Game();

    /**
     * @brief Главный игровой цикл приложения
     */
    void run();

    /**
     * @brief Отображает главное меню игры
     */
    void showMainMenu();

    /**
     * @brief Переключает активного игрока
     */
    void switchTurn();

    /**
     * @brief Проверяет условия завершения игры
     * @return true если один из игроков победил
     */
    bool checkWinCondition();

    /**
     * @brief Обрабатывает выбор пользователя в главном меню
     * @param choice Числовой выбор пользователя
     */
    void processMenuInput(int choice);

    /**
     * @brief Инициализирует новую игровую сессию
     */
    void startNewGame();

    /**
     * @brief Отображает таблицу лидеров
     */
    void showLeaderboard();

    /**
     * @brief Отображает меню после завершения игры
     */
    void showAfterGameMenu();

    /**
     * @brief Обрабатывает выбор пользователя после игры
     * @param choice Числовой выбор пользователя
     */
    void processAfterGameInput(int choice);
};

#endif
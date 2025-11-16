#include "Game.hpp"
#include "Color.hpp"
#include <iostream>
#include <limits>
#include <thread>
#include <chrono>

/**
 * @brief Конструктор игры - инициализирует состояние и зависимости
 */
Game::Game() : player1(nullptr), player2(nullptr), currentPlayer(nullptr),
gameState(GameState::Menu), winnerName("") {
}

/**
 * @brief Деструктор игры - освобождает ресурсы игроков
 *
 * Реализует принцип RAII - автоматическое освобождение
 * динамически выделенной памяти.
 */
Game::~Game() {
    delete player1;
    delete player2;
}

/**
 * @brief Возвращает оппонента текущего активного игрока
 * @return Указатель на игрока-противника
 */
Player* Game::getOpponent() const {
    return (currentPlayer == player1) ? player2 : player1;
}

/**
 * @brief Главный игровой цикл, реализующий конечный автомат
 *
 * Управляет переходами между состояниями игры и координирует
 * взаимодействие между всеми компонентами системы.
 */
void Game::run() {
    while (gameState != GameState::GameOver) {
        switch (gameState) {
        case GameState::Menu:
            showMainMenu();
            break;
        case GameState::Placement:
            player1->placeShips();
            player2->placeShips();
            gameState = GameState::Battle;
            currentPlayer = player1;
            break;
        case GameState::Battle:
            if (checkWinCondition()) {
                gameState = GameState::AfterGame;
            }
            else {
                Player* opponent = getOpponent();

                // Сохраняем результат выстрела
                bool wasHit = false;

                // Для человеческого игрока показываем поля и делаем ход
                if (dynamic_cast<HumanPlayer*>(currentPlayer)) {
                    HumanPlayer* human = dynamic_cast<HumanPlayer*>(currentPlayer);
                    wasHit = human->makeMoveWithResult(*opponent);
                }
                else {
                    // Для ИИ
                    AIPlayer* ai = dynamic_cast<AIPlayer*>(currentPlayer);
                    wasHit = ai->makeMoveWithResult(*opponent);
                }

                // Передаем ход только если не было попадания
                if (!wasHit) {
                    switchTurn();
                }
                else {
                    std::cout << "Попадание! " << currentPlayer->getName() << " продолжает ход." << std::endl;
                }
            }
            break;
        case GameState::AfterGame:
            showAfterGameMenu();
            break;
        case GameState::GameOver:
            break;
        }
    }
}

/**
 * @brief Отображает главное меню с навигацией по приложению
 */
void Game::showMainMenu() {
    Color::setColor(Color::GREEN);
    std::cout << "\n=== МОРСКОЙ БОЙ ===" << std::endl;
    Color::resetColor();

    std::cout << "1. Начать новую игру" << std::endl;
    std::cout << "2. Показать таблицу лидеров" << std::endl;
    std::cout << "3. Выход" << std::endl;
    std::cout << "Выберите опцию: ";

    int choice;
    std::cin >> choice;
    processMenuInput(choice);
}

/**
 * @brief Отображает меню после завершения игровой сессии
 *
 * Предоставляет пользователю выбор действий: сохранение результата,
 * продолжение игры или выход из приложения.
 */
void Game::showAfterGameMenu() {
    std::cout << "\n";
    Color::setColor(Color::GREEN);
    std::cout << "=== ИГРА ЗАВЕРШЕНА ===" << std::endl;
    Color::resetColor();

    Color::setColor(Color::YELLOW);
    std::cout << "Победитель: " << winnerName << "!" << std::endl;
    Color::resetColor();

    std::cout << "\nВыберите действие:" << std::endl;
    Color::setColor(Color::GREEN);
    std::cout << "1. Сохранить результат и выйти в меню" << std::endl;
    Color::setColor(Color::BLUE);
    std::cout << "2. Продолжить играть (новая игра)" << std::endl;
    Color::setColor(Color::RED);
    std::cout << "3. Выйти из игры" << std::endl;
    Color::resetColor();
    std::cout << "Ваш выбор: ";

    int choice;
    std::cin >> choice;
    processAfterGameInput(choice);
}

/**
 * @brief Обрабатывает пользовательский ввод в меню завершения игры
 * @param choice Числовой выбор пользователя
 *
 * Реализует бизнес-логику переходов между состояниями
 * на основе выбора пользователя.
 */
void Game::processAfterGameInput(int choice) {
    switch (choice) {
    case 1:
        // Сохраняем результат и выходим в меню
        leaderboard.addWin(winnerName);
        Color::setColor(Color::GREEN);
        std::cout << "Результат сохранен в таблице лидеров!" << std::endl;
        Color::resetColor();

        // Очищаем игроков
        delete player1;
        delete player2;
        player1 = nullptr;
        player2 = nullptr;
        currentPlayer = nullptr;
        winnerName = "";

        gameState = GameState::Menu;
        break;

    case 2:
        // Начинаем новую игру
        Color::setColor(Color::BLUE);
        std::cout << "Начинаем новую игру!" << std::endl;
        Color::resetColor();

        // Очищаем предыдущих игроков
        delete player1;
        delete player2;

        startNewGame();
        break;

    case 3:
        // Выход из игры
        gameState = GameState::GameOver;
        Color::setColor(Color::RED);
        std::cout << "Выход из игры..." << std::endl;
        Color::resetColor();
        break;

    default:
        Color::setColor(Color::RED);
        std::cout << "Неверный выбор! Попробуйте снова." << std::endl;
        Color::resetColor();
        break;
    }
}

/**
 * @brief Переключает активного игрока в пошаговом режиме
 */
void Game::switchTurn() {
    currentPlayer = (currentPlayer == player1) ? player2 : player1;
}

/**
 * @brief Проверяет условия победы и определяет победителя
 * @return true если игра достигла завершающего состояния
 *
 * Анализирует состояние флотов обоих игроков и объявляет
 * победителя при уничтожении всех кораблей оппонента.
 */
bool Game::checkWinCondition() {
    if (player1->allShipsSunk()) {
        std::cout << "\n";
        Color::setColor(Color::GREEN);
        std::cout << "=== ИГРА ОКОНЧЕНА ===" << std::endl;
        Color::resetColor();
        Color::setColor(Color::RED);
        std::cout << "Победил компьютер!" << std::endl;
        Color::resetColor();
        winnerName = "Computer";

        // Показываем финальные поля
        std::cout << "\nФинальное состояние полей:" << std::endl;
        Color::setColor(Color::GREEN);
        std::cout << "Ваше поле:" << std::endl;
        Color::resetColor();
        player1->getMyBoard().display(true);

        Color::setColor(Color::BLUE);
        std::cout << "\nПоле противника:" << std::endl;
        Color::resetColor();
        player1->getEnemyBoard().display(false);

        return true;
    }
    else if (player2->allShipsSunk()) {
        std::cout << "\n";
        Color::setColor(Color::GREEN);
        std::cout << "=== ИГРА ОКОНЧЕНА ===" << std::endl;
        Color::resetColor();
        Color::setColor(Color::GREEN);
        std::cout << "Победил " << player1->getName() << "!" << std::endl;
        Color::resetColor();
        winnerName = player1->getName();

        // Показываем финальные поля
        std::cout << "\nФинальное состояние полей:" << std::endl;
        Color::setColor(Color::GREEN);
        std::cout << "Ваше поле:" << std::endl;
        Color::resetColor();
        player1->getMyBoard().display(true);

        Color::setColor(Color::BLUE);
        std::cout << "\nПоле противника:" << std::endl;
        Color::resetColor();
        player1->getEnemyBoard().display(false);

        return true;
    }
    return false;
}

/**
 * @brief Обрабатывает пользовательский ввод в главном меню
 * @param choice Числовой выбор пользователя
 */
void Game::processMenuInput(int choice) {
    switch (choice) {
    case 1:
        startNewGame();
        break;
    case 2:
        showLeaderboard();
        break;
    case 3:
        gameState = GameState::GameOver;
        std::cout << "Выход из игры..." << std::endl;
        break;
    default:
        Color::setColor(Color::RED);
        std::cout << "Неверный выбор!" << std::endl;
        Color::resetColor();
        break;
    }
}

/**
 * @brief Инициализирует новую игровую сессию с пользовательскими настройками
 *
 * Создает экземпляры игроков, запрашивает имя пользователя
 * и переводит игру в состояние расстановки кораблей.
 */
void Game::startNewGame() {
    // Очищаем предыдущих игроков
    delete player1;
    delete player2;

    // Создаем новых игроков
    std::string playerName;
    std::cout << "Введите ваше имя: ";
    std::cin >> playerName;

    player1 = new HumanPlayer(playerName);
    player2 = new AIPlayer();

    currentPlayer = player1;
    gameState = GameState::Placement;
    winnerName = "";

    Color::setColor(Color::GREEN);
    std::cout << "\nНовая игра началась! Удачи, " << playerName << "!" << std::endl;
    Color::resetColor();
}

/**
 * @brief Отображает таблицу лидеров с паузой для просмотра
 */
void Game::showLeaderboard() {
    leaderboard.display();
    std::cout << "\nНажмите Enter для продолжения...";
    std::cin.ignore();
    std::cin.get();
}
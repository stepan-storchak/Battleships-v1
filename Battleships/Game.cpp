#include "Game.hpp"
#include "Color.hpp"
#include "IShip.hpp"
#include "ShipContainer.hpp"
#include "GameUtils.hpp"
#include <iostream>
#include <limits>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <memory>
#include <vector>
#include <stdexcept>

Game::Game() : player1(nullptr), player2(nullptr), currentPlayer(nullptr),
gameState(GameState::Menu), winnerName("") {
}

Player* Game::getOpponent() const {
    if (currentPlayer == player1.get()) {
        return player2.get();
    }
    else {
        return player1.get();
    }
}

void Game::run() {
    try {
        
        demonstrateSTLFeatures();
        while (gameState != GameState::GameOver) {
            switch (gameState) {
            case GameState::Menu:
                showMainMenu();
                break;
            case GameState::Placement:
                player1->placeShips();
                player2->placeShips();
                gameState = GameState::Battle;
                currentPlayer = player1.get();
                break;
            case GameState::Battle:
                if (checkWinCondition()) {
                    gameState = GameState::AfterGame;
                }
                else {
                    Player* opponent = getOpponent();
                    bool wasHit = false;

                    if (dynamic_cast<HumanPlayer*>(currentPlayer)) {
                        HumanPlayer* human = dynamic_cast<HumanPlayer*>(currentPlayer);
                        wasHit = human->makeMoveWithResult(*opponent);
                    }
                    else if (dynamic_cast<AdvancedAIPlayer*>(currentPlayer)) {
                        AdvancedAIPlayer* advancedAI = dynamic_cast<AdvancedAIPlayer*>(currentPlayer);
                        wasHit = advancedAI->makeMoveWithResult(*opponent);
                    }
                    else {
                        AIPlayer* ai = dynamic_cast<AIPlayer*>(currentPlayer);
                        wasHit = ai->makeMoveWithResult(*opponent);
                    }

                    if (!wasHit) {
                        switchTurn();
                    }
                    else {
                        std::cout << "Попадание! " << currentPlayer->getName()
                            << " продолжает ход." << std::endl;
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
    catch (const std::exception& e) {
        std::cerr << "Критическая ошибка в игровом цикле: " << e.what() << std::endl;
    }
}

void Game::showMainMenu() {
    Color::setColor(Color::GREEN);
    std::cout << "\n=== МОРСКОЙ БОЙ ===" << std::endl;
    Color::resetColor();
    std::cout << "1. Начать новую игру" << std::endl;
    std::cout << "2. Показать таблицу лидеров" << std::endl;
    std::cout << "3. Демонстрация ООП функций" << std::endl;
    std::cout << "4. Выход" << std::endl;
    std::cout << "Выберите опцию: ";
    int choice;
    std::cin >> choice;
    processMenuInput(choice);
}

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

void Game::processAfterGameInput(int choice) {
    switch (choice) {
    case 1:
        leaderboard.addWin(winnerName);
        Color::setColor(Color::GREEN);
        std::cout << "Результат сохранен в таблице лидеров!" << std::endl;
        Color::resetColor();
        player1.reset();
        player2.reset();
        currentPlayer = nullptr;
        winnerName = "";
        gameState = GameState::Menu;
        break;
    case 2:
        Color::setColor(Color::BLUE);
        std::cout << "Начинаем новую игру!" << std::endl;
        Color::resetColor();
        player1.reset();
        player2.reset();
        startNewGame();
        break;
    case 3:
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

void Game::switchTurn() {
    currentPlayer = (currentPlayer == player1.get()) ? player2.get() : player1.get();
}

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
        std::cout << "=== ИГРА ОКОНЧЕНA ===" << std::endl;
        Color::resetColor();
        Color::setColor(Color::GREEN);
        std::cout << "Победил " << player1->getName() << "!" << std::endl;
        Color::resetColor();
        winnerName = player1->getName();
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

void Game::processMenuInput(int choice) {
    switch (choice) {
    case 1:
        startNewGame();
        break;
    case 2:
        showLeaderboard();
        break;
    case 3:
        demonstrateOOPFeatures();
        break;
    case 4:
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

void Game::startNewGame() {
    try {
        player1.reset();
        player2.reset();

        std::string playerName;
        std::cout << "Введите ваше имя: ";
        std::cin >> playerName;

        int gameType;
        std::cout << "Выберите тип игры:\n1. Против обычного ИИ\n2. Против продвинутого ИИ\nВаш выбор: ";
        std::cin >> gameType;

        player1 = std::make_unique<HumanPlayer>(playerName);

        if (gameType == 2) {
            player2 = std::make_unique<AdvancedAIPlayer>("Advanced AI", 3);
        }
        else {
            player2 = std::make_unique<AIPlayer>();
        }

        currentPlayer = player1.get();
        gameState = GameState::Placement;
        winnerName = "";

        Color::setColor(Color::GREEN);
        std::cout << "\nНовая игра началась! Удачи, " << playerName << "!" << std::endl;
        Color::resetColor();
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при создании новой игры: " << e.what() << std::endl;
        gameState = GameState::Menu;
    }
}

void Game::showLeaderboard() {
    leaderboard.display();
    std::cout << "\nНажмите Enter для продолжения...";
    std::cin.ignore();
    std::cin.get();
}

void Game::demonstrateOOPFeatures() {
    std::cout << "\n=== ДЕМОНСТРАЦИЯ ООП ВОЗМОЖНОСТЕЙ ===" << std::endl;

    demonstrateVirtualFunctions();
    demonstrateCloning();
    demonstrateAbstractClass();

    std::cout << "\nНажмите Enter для продолжения...";
    std::cin.ignore();
    std::cin.get();
}

void Game::demonstrateVirtualFunctions() {
    std::cout << "\n--- Демонстрация виртуальных функций ---" << std::endl;

    HumanPlayer human("Тестовый Игрок");
    AIPlayer ai("Тестовый ИИ");
    AdvancedAIPlayer advancedAI("Продвинутый ИИ", 2);

    std::vector<Player*> players = { &human, &ai, &advancedAI };

    for (Player* player : players) {
        player->displayInfo();
        std::cout << "---" << std::endl;
    }

    std::cout << "Демонстрация присваивания указателей:" << std::endl;
    Player* basePtr = &advancedAI;
    std::cout << "Тип через базовый указатель: " << basePtr->getPlayerType() << std::endl;
}

void Game::demonstrateCloning() {
    Ship originalShip(3, Coordinate(0, 0), Orientation::Horizontal, "Тестовый корабль");
    originalShip.setShipId(100);

    Ship* shallowCopy = originalShip.shallowClone();
    Ship* deepCopy = originalShip.deepClone();

    shallowCopy->setShipId(200);
    deepCopy->setShipId(300);

    delete shallowCopy;
    delete deepCopy;
}

void Game::demonstrateAbstractClass() {
    std::cout << "\n--- Демонстрация абстрактного класса ---" << std::endl;

    Ship concreteShip(2, Coordinate(1, 1), Orientation::Vertical, "Интерфейсный корабль");
    IShip* shipPtr = &concreteShip;

    std::cout << "Использование через абстрактный интерфейс:" << std::endl;
    std::cout << "Имя: " << shipPtr->getName() << std::endl;
    std::cout << "Размер: " << shipPtr->getSize() << std::endl;
    std::cout << "Тип: " << shipPtr->getType() << std::endl;
    std::cout << "Полная информация: " << shipPtr->getFullInfo() << std::endl;
}

void Game:: demonstrateSTLFeatures() {
    std::cout << "\n=== Демонстрация STL возможностей ===" << std::endl;

    std::vector<int> numbers = { 5, 2, 8, 1, 9, 3, 7, 4, 6 };

    int max = findMax(numbers);
    int min = findMin(numbers);
    int avg = calculateAverage(numbers);

    std::cout << "Числа: ";
    for (auto n : numbers) std::cout << n << " ";
    std::cout << "\nМаксимум: " << max << std::endl;
    std::cout << "Минимум: " << min << std::endl;
    std::cout << "Среднее: " << avg << std::endl;

    std::sort(numbers.begin(), numbers.end());
    std::cout << "Отсортированные числа: ";
    for (auto n : numbers) std::cout << n << " ";
    std::cout << std::endl;

    int target = 7;
    bool found = contains(numbers, target);
    std::cout << "Число " << target << (found ? " найдено" : " не найдено") << std::endl;

    std::cout << "Пример альтернативы variant: использование union или класса-обертки" << std::endl;
}

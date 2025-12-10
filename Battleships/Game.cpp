#include "Game.hpp"
#include "Color.hpp"


std::string getSafeInputGame() {
    std::string input;
    std::getline(std::cin, input);

    if (!input.empty()) {
        input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
        input.erase(std::remove(input.begin(), input.end(), '\r'), input.end());
    }

    return input;
}

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}

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
                clearInputBuffer();
                break;
            case GameState::Battle:
                if (checkWinCondition()) {
                    gameState = GameState::AfterGame;
                }
                else {
                    Player* opponent = getOpponent();
                    bool wasHit = false;

                    wasHit = currentPlayer->makeMoveWithResult(*opponent);

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
    std::cout << "3. Выход" << std::endl;

    int choice = 0;
    bool validChoice = false;

    while (!validChoice) {
        std::cout << "Выберите опцию: ";
        std::string input = getSafeInputGame();

        try {
            choice = std::stoi(input);
            if (choice >= 1 && choice <= 3) {
                validChoice = true;
            }
            else {
                Color::setColor(Color::RED);
                std::cout << "Неверный выбор! Введите 1, 2 или 3." << std::endl;
                Color::resetColor();
            }
        }
        catch (const std::exception&) {
            Color::setColor(Color::RED);
            std::cout << "Неверный ввод! Введите число от 1 до 3." << std::endl;
            Color::resetColor();
        }
    }

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

    int choice = 0;
    bool validChoice = false;

    while (!validChoice) {
        std::cout << "Ваш выбор: ";
        std::string input = getSafeInputGame();

        try {
            choice = std::stoi(input);
            if (choice >= 1 && choice <= 3) {
                validChoice = true;
            }
            else {
                Color::setColor(Color::RED);
                std::cout << "Неверный выбор! Введите 1, 2 или 3." << std::endl;
                Color::resetColor();
            }
        }
        catch (const std::exception&) {
            Color::setColor(Color::RED);
            std::cout << "Неверный ввод! Введите число от 1 до 3." << std::endl;
            Color::resetColor();
        }
    }

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
        std::string playerName;
        while (true) {
            std::cout << "Введите ваше имя: ";
            playerName = getSafeInputGame();

            if (!playerName.empty()) {
                break;
            }
            Color::setColor(Color::RED);
            std::cout << "Имя не может быть пустым!" << std::endl;
            Color::resetColor();
        }

        int gameType = 0;
        bool validGameType = false;

        while (!validGameType) {
            std::cout << "Выберите тип игры:\n1. Против обычного ИИ\n2. Против продвинутого ИИ\nВаш выбор: ";
            std::string input = getSafeInputGame();

            try {
                gameType = std::stoi(input);
                if (gameType == 1 || gameType == 2) {
                    validGameType = true;
                }
                else {
                    Color::setColor(Color::RED);
                    std::cout << "Введите 1 или 2!" << std::endl;
                    Color::resetColor();
                }
            }
            catch (const std::exception&) {
                Color::setColor(Color::RED);
                std::cout << "Неверный ввод! Введите 1 или 2." << std::endl;
                Color::resetColor();
            }
        }

        player1 = std::make_unique<HumanPlayer>(playerName);

        if (gameType == 2) {
            player2 = std::make_unique<AdvancedAIPlayer>("Advanced AI");
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
    clearInputBuffer();
    std::cin.get();
}
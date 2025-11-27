#include "Game.hpp"
#include "Color.hpp"
#include "IShip.hpp"
#include <iostream>
#include <limits>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <memory>
#include <vector>

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
        demonstrateOOPFeatures();

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
                        std::cout << "Ďîďŕäŕíčĺ! " << currentPlayer->getName()
                            << " ďđîäîëćŕĺň őîä." << std::endl;
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
        std::cerr << "Ęđčňč÷ĺńęŕ˙ îřčáęŕ â čăđîâîě öčęëĺ: " << e.what() << std::endl;
    }
}

void Game::showMainMenu() {
    Color::setColor(Color::GREEN);
    std::cout << "\n=== ĚÎĐŃĘÎÉ ÁÎÉ ===" << std::endl;
    Color::resetColor();
    std::cout << "1. Íŕ÷ŕňü íîâóţ čăđó" << std::endl;
    std::cout << "2. Ďîęŕçŕňü ňŕáëčöó ëčäĺđîâ" << std::endl;
    std::cout << "3. Äĺěîíńňđŕöč˙ ÎÎĎ ôóíęöčé" << std::endl;
    std::cout << "4. Âűőîä" << std::endl;
    std::cout << "Âűáĺđčňĺ îďöčţ: ";
    int choice;
    std::cin >> choice;
    processMenuInput(choice);
}

void Game::showAfterGameMenu() {
    std::cout << "\n";
    Color::setColor(Color::GREEN);
    std::cout << "=== ČĂĐŔ ÇŔÂĹĐŘĹÍŔ ===" << std::endl;
    Color::resetColor();
    Color::setColor(Color::YELLOW);
    std::cout << "Ďîáĺäčňĺëü: " << winnerName << "!" << std::endl;
    Color::resetColor();
    std::cout << "\nÂűáĺđčňĺ äĺéńňâčĺ:" << std::endl;
    Color::setColor(Color::GREEN);
    std::cout << "1. Ńîőđŕíčňü đĺçóëüňŕň č âűéňč â ěĺíţ" << std::endl;
    Color::setColor(Color::BLUE);
    std::cout << "2. Ďđîäîëćčňü čăđŕňü (íîâŕ˙ čăđŕ)" << std::endl;
    Color::setColor(Color::RED);
    std::cout << "3. Âűéňč čç čăđű" << std::endl;
    Color::resetColor();
    std::cout << "Âŕř âűáîđ: ";
    int choice;
    std::cin >> choice;
    processAfterGameInput(choice);
}

void Game::processAfterGameInput(int choice) {
    switch (choice) {
    case 1:
        leaderboard.addWin(winnerName);
        Color::setColor(Color::GREEN);
        std::cout << "Đĺçóëüňŕň ńîőđŕíĺí â ňŕáëčöĺ ëčäĺđîâ!" << std::endl;
        Color::resetColor();
        player1.reset();
        player2.reset();
        currentPlayer = nullptr;
        winnerName = "";
        gameState = GameState::Menu;
        break;
    case 2:
        Color::setColor(Color::BLUE);
        std::cout << "Íŕ÷číŕĺě íîâóţ čăđó!" << std::endl;
        Color::resetColor();
        player1.reset();
        player2.reset();
        startNewGame();
        break;
    case 3:
        gameState = GameState::GameOver;
        Color::setColor(Color::RED);
        std::cout << "Âűőîä čç čăđű..." << std::endl;
        Color::resetColor();
        break;
    default:
        Color::setColor(Color::RED);
        std::cout << "Íĺâĺđíűé âűáîđ! Ďîďđîáóéňĺ ńíîâŕ." << std::endl;
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
        std::cout << "=== ČĂĐŔ ÎĘÎÍ×ĹÍŔ ===" << std::endl;
        Color::resetColor();
        Color::setColor(Color::RED);
        std::cout << "Ďîáĺäčë ęîěďüţňĺđ!" << std::endl;
        Color::resetColor();
        winnerName = "Computer";
        std::cout << "\nÔčíŕëüíîĺ ńîńňî˙íčĺ ďîëĺé:" << std::endl;
        Color::setColor(Color::GREEN);
        std::cout << "Âŕřĺ ďîëĺ:" << std::endl;
        Color::resetColor();
        player1->getMyBoard().display(true);
        Color::setColor(Color::BLUE);
        std::cout << "\nĎîëĺ ďđîňčâíčęŕ:" << std::endl;
        Color::resetColor();
        player1->getEnemyBoard().display(false);
        return true;
    }
    else if (player2->allShipsSunk()) {
        std::cout << "\n";
        Color::setColor(Color::GREEN);
        std::cout << "=== ČĂĐŔ ÎĘÎÍ×ĹÍA ===" << std::endl;
        Color::resetColor();
        Color::setColor(Color::GREEN);
        std::cout << "Ďîáĺäčë " << player1->getName() << "!" << std::endl;
        Color::resetColor();
        winnerName = player1->getName();
        std::cout << "\nÔčíŕëüíîĺ ńîńňî˙íčĺ ďîëĺé:" << std::endl;
        Color::setColor(Color::GREEN);
        std::cout << "Âŕřĺ ďîëĺ:" << std::endl;
        Color::resetColor();
        player1->getMyBoard().display(true);
        Color::setColor(Color::BLUE);
        std::cout << "\nĎîëĺ ďđîňčâíčęŕ:" << std::endl;
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
        std::cout << "Âűőîä čç čăđű..." << std::endl;
        break;
    default:
        Color::setColor(Color::RED);
        std::cout << "Íĺâĺđíűé âűáîđ!" << std::endl;
        Color::resetColor();
        break;
    }
}

void Game::startNewGame() {
    try {
        player1.reset();
        player2.reset();

        std::string playerName;
        std::cout << "Ââĺäčňĺ âŕřĺ čě˙: ";
        std::cin >> playerName;

        int gameType;
        std::cout << "Âűáĺđčňĺ ňčď čăđű:\n1. Ďđîňčâ îáű÷íîăî ČČ\n2. Ďđîňčâ ďđîäâčíóňîăî ČČ\nÂŕř âűáîđ: ";
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
        std::cout << "\nÍîâŕ˙ čăđŕ íŕ÷ŕëŕńü! Óäŕ÷č, " << playerName << "!" << std::endl;
        Color::resetColor();
    }
    catch (const std::exception& e) {
        std::cerr << "Îřčáęŕ ďđč ńîçäŕíčč íîâîé čăđű: " << e.what() << std::endl;
        gameState = GameState::Menu;
    }
}

void Game::showLeaderboard() {
    leaderboard.display();
    std::cout << "\nÍŕćěčňĺ Enter äë˙ ďđîäîëćĺíč˙...";
    std::cin.ignore();
    std::cin.get();
}

void Game::demonstrateOOPFeatures() {
    std::cout << "\n=== ÄĹĚÎÍŃŇĐŔÖČß ÎÎĎ ÂÎÇĚÎĆÍÎŃŇĹÉ ===" << std::endl;

    demonstrateVirtualFunctions();
    demonstrateCloning();
    demonstrateAbstractClass();

    std::cout << "\nÍŕćěčňĺ Enter äë˙ ďđîäîëćĺíč˙...";
    std::cin.ignore();
    std::cin.get();
}

void Game::demonstrateVirtualFunctions() {
    std::cout << "\n--- Äĺěîíńňđŕöč˙ âčđňóŕëüíűő ôóíęöčé ---" << std::endl;

    HumanPlayer human("Ňĺńňîâűé Čăđîę");
    AIPlayer ai("Ňĺńňîâűé ČČ");
    AdvancedAIPlayer advancedAI("Ďđîäâčíóňűé ČČ", 2);

    std::vector<Player*> players = { &human, &ai, &advancedAI };

    for (Player* player : players) {
        player->displayInfo();
        std::cout << "---" << std::endl;
    }

    std::cout << "Äĺěîíńňđŕöč˙ ďđčńâŕčâŕíč˙ óęŕçŕňĺëĺé:" << std::endl;
    Player* basePtr = &advancedAI;
    std::cout << "Ňčď ÷ĺđĺç áŕçîâűé óęŕçŕňĺëü: " << basePtr->getPlayerType() << std::endl;
}

void Game::demonstrateCloning() {
    std::cout << "\n--- Äĺěîíńňđŕöč˙ ęëîíčđîâŕíč˙ ---" << std::endl;

    Ship originalShip(3, Coordinate(0, 0), Orientation::Horizontal, "Ňĺńňîâűé ęîđŕáëü");

    Ship* clonedShip = originalShip.clone();
    std::cout << "Ęëîíčđîâŕíčĺ: " << clonedShip->getDescription() << std::endl;

    delete clonedShip;
}

void Game::demonstrateAbstractClass() {
    std::cout << "\n--- Äĺěîíńňđŕöč˙ ŕáńňđŕęňíîăî ęëŕńńŕ ---" << std::endl;

    Ship concreteShip(2, Coordinate(1, 1), Orientation::Vertical, "Číňĺđôĺéńíűé ęîđŕáëü");
    IShip* shipPtr = &concreteShip;

    std::cout << "Čńďîëüçîâŕíčĺ ÷ĺđĺç ŕáńňđŕęňíűé číňĺđôĺéń:" << std::endl;
    std::cout << "Čě˙: " << shipPtr->getName() << std::endl;
    std::cout << "Đŕçěĺđ: " << shipPtr->getSize() << std::endl;
    std::cout << "Ňčď: " << shipPtr->getType() << std::endl;
    std::cout << "Ďîëíŕ˙ číôîđěŕöč˙: " << shipPtr->getFullInfo() << std::endl;
}
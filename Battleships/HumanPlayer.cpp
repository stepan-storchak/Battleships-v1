#include "HumanPlayer.hpp"
#include "Color.hpp"

namespace {
    const int shipSizes[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
    const int BOARD_SIZE = 10;
    const int MAX_SINGLE_PLACEMENT_TRIES = 100;
    const int AROUND_OFFSET = 1;
}

void HumanPlayer::placeShips() {
    std::cout << "Расстановка кораблей для игрока: " << this->name << std::endl;
    int choice;
    while (true) {
        std::cout << "Выберите способ расстановки:\n";
        Color::setColor(Color::GREEN);
        std::cout << "1 - Автоматическая расстановка\n";
        Color::setColor(Color::YELLOW);
        std::cout << "2 - Ручная расстановка\n";
        Color::resetColor();
        std::cout << "Ваш выбор: ";
        std::cin >> choice;
        if (choice == 1) {
            if (automaticPlacement()) {
                break;
            }
            else {
                Color::setColor(Color::RED);
                std::cout << "Автоматическая расстановка не удалась. Попробуйте ручную расстановку.\n";
                Color::resetColor();
            }
        }
        else if (choice == 2) {
            manualPlacement();
            break;
        }
        else {
            Color::setColor(Color::RED);
            std::cout << "Неверный выбор! Попробуйте снова.\n";
            Color::resetColor();
        }
    }
}

void HumanPlayer::makeMove(Player& enemy) {
    makeMoveWithResult(enemy);
}


bool HumanPlayer::makeMoveWithResult(Player& enemy) {
    std::cout << "\n=== Ход игрока. " << this->name << " ===" << std::endl;

    Color::setColor(Color::GREEN);
    std::cout << "Ваше поле:" << std::endl;
    Color::resetColor();
    this->myBoard.display(true);

    Color::setColor(Color::BLUE);
    std::cout << "\nПоле противника:" << std::endl;
    Color::resetColor();
    this->enemyBoard.display(false);

    Coordinate target = inputCoordinate();
    ShotResult result = enemy.getShotResult(target);
    bool wasHit = false;

    switch (result) {
    case ShotResult::Miss:
        this->enemyBoard.setCellState(target, CellState::Miss);
        Color::setColor(Color::BLUE);
        std::cout << "Промах!" << std::endl;
        Color::resetColor();
        wasHit = false;
        break;
    case ShotResult::Hit:
        this->enemyBoard.setCellState(target, CellState::Hit);
        Color::setColor(Color::YELLOW);
        std::cout << "Попадание!" << std::endl;
        Color::resetColor();
        wasHit = true;
        break;
    case ShotResult::Sunk:
        this->enemyBoard.setCellState(target, CellState::Hit);
        Color::setColor(Color::RED);
        std::cout << "Уничтожен корабль!" << std::endl;
        Color::resetColor();
        wasHit = true;
        markAreaAroundDestroyedShip(enemy, target);
        break;
    }
    return wasHit;
}

void HumanPlayer::markSurroundingCells(const Coordinate& center) {
    for (int dy = -AROUND_OFFSET; dy <= AROUND_OFFSET; ++dy) {
        for (int dx = -AROUND_OFFSET; dx <= AROUND_OFFSET; ++dx) {
            Coordinate around(center.x + dx, center.y + dy);
            if (around.x >= 0 && around.x < BOARD_SIZE && around.y >= 0 && around.y < BOARD_SIZE) {
                if (this->enemyBoard.getCellState(around) == CellState::Empty) {
                    this->enemyBoard.setCellState(around, CellState::Miss);
                }
            }
        }
    }
}

void HumanPlayer::markAreaAroundDestroyedShip(Player& enemy, const Coordinate& hitCoord) {
    markSurroundingCells(hitCoord);
    for (int radius = 1; radius <= 3; radius++) {
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                Coordinate around(hitCoord.x + dx, hitCoord.y + dy);
                if (around.x >= 0 && around.x < BOARD_SIZE && around.y >= 0 && around.y < BOARD_SIZE) {
                    if (this->enemyBoard.getCellState(around) == CellState::Hit) {
                        markSurroundingCells(around);
                    }
                }
            }
        }
    }
}

Coordinate HumanPlayer::inputCoordinate() const {
    Coordinate coord;
    std::string input;

    while (true) {
        try {
            std::cout << "Введите координаты для выстрела (например, A1): ";
            input = getSafeInput();

            if (parseCoordinate(input, coord)) {

                CellState state = this->enemyBoard.getCellState(coord);
                if (state == CellState::Empty) {
                    return coord;
                }
                else {
                    Color::setColor(Color::RED);
                    std::cout << "Вы уже стреляли в эту клетку!" << std::endl;
                    Color::resetColor();
                }
            }
            
        }
        catch (const std::exception& e) {
            Color::setColor(Color::RED);
            std::cout << "Ошибка ввода: " << e.what() << std::endl;
            Color::resetColor();
        }
    }
}


void HumanPlayer::manualPlacement() {
    const char* shipNames[] = { "четырехпалубный", "трехпалубный", "трехпалубный",
                              "двухпалубный", "двухпалубный", "двухпалубный",
                              "однопалубный", "однопалубный", "однопалубный", "однопалубный" };

    for (int i = 0; i < BOARD_SIZE; ++i) {
        int size = shipSizes[i];
        bool placed = false;

        while (!placed) {
            try {
                Color::setColor(Color::YELLOW);
                std::cout << "\nРазмещение " << shipNames[i] << " корабля (" << size << " палубы)" << std::endl;
                Color::resetColor();

                myBoard.display(true);

                Coordinate startCoord;
                bool validStart = false;

                while (!validStart) {
                    std::cout << "Введите начальную координату (например, A1): ";
                    std::string input = getSafeInput();

                    if (parseCoordinate(input, startCoord)) {
                        validStart = true;
                    }
                    else {
                        Color::setColor(Color::RED);
                        std::cout << "Неверный формат координат! Используйте формат A1-J10." << std::endl;
                        Color::resetColor();
                    }
                }

                if (size > 1) {
                    char orient;
                    bool validOrientation = false;

                    while (!validOrientation) {
                        std::cout << "Выберите ориентацию (H - горизонтально, V - вертикально): ";
                        std::string orientInput = getSafeInput();

                        if (!orientInput.empty()) {
                            orient = toupper(orientInput[0]);
                            if (orient == 'H' || orient == 'V') {
                                validOrientation = true;
                            }
                            else {
                                Color::setColor(Color::RED);
                                std::cout << "Введите H или V!" << std::endl;
                                Color::resetColor();
                            }
                        }
                    }

                    Orientation orientation = (orient == 'H') ? Orientation::Horizontal : Orientation::Vertical;
                    Ship ship(size, startCoord, orientation);

                    if (isValidShipPlacement(size, startCoord, orientation)) {
                        if (myBoard.placeShip(ship)) {
                            ships.push_back(ship);
                            placed = true;
                            Color::setColor(Color::GREEN);
                            std::cout << "Корабль размещен успешно!" << std::endl;
                            Color::resetColor();
                        }
                        else {
                            Color::setColor(Color::RED);
                            std::cout << "Ошибка при размещении корабля!" << std::endl;
                            Color::resetColor();
                        }
                    }
                    else {
                        Color::setColor(Color::RED);
                        std::cout << "Неверное размещение корабля! Попробуйте снова." << std::endl;
                        Color::resetColor();
                    }
                }
                else {
                    Ship ship(size, startCoord, Orientation::Horizontal);

                    if (isValidShipPlacement(size, startCoord, Orientation::Horizontal)) {
                        if (myBoard.placeShip(ship)) {
                            ships.push_back(ship);
                            placed = true;
                            Color::setColor(Color::GREEN);
                            std::cout << "Корабль размещен успешно!" << std::endl;
                            Color::resetColor();
                        }
                    }
                    else {
                        Color::setColor(Color::RED);
                        std::cout << "Неверное размещение корабля! Попробуйте снова." << std::endl;
                        Color::resetColor();
                    }
                }
            }
            catch (const std::exception& e) {
                Color::setColor(Color::RED);
                std::cout << "Ошибка: " << e.what() << std::endl;
                Color::resetColor();
            }
        }
    }
}

bool HumanPlayer::automaticPlacement() {
    int attempts = 0;
    const int MAX_ATTEMPTS = 5;

    while (attempts < MAX_ATTEMPTS) {
        myBoard.clearBoard();
        ships.clear();

        bool success = true;

        for (int size : shipSizes) {
            bool placed = false;
            int placementAttempts = 0;

            while (!placed && placementAttempts < MAX_SINGLE_PLACEMENT_TRIES) {
                int x = std::rand() % BOARD_SIZE;
                int y = std::rand() % BOARD_SIZE;
                Orientation orientation = (std::rand() % 2 == 0) ? Orientation::Horizontal : Orientation::Vertical;

                Ship ship(size, Coordinate(x, y), orientation);
                if (myBoard.isValidPlacement(ship)) {
                    if (myBoard.placeShip(ship)) {
                        ships.push_back(ship);
                        placed = true;
                    }
                }
                placementAttempts++;
            }

            if (!placed) {
                success = false;
                break;
            }
        }

        if (success) {
            Color::setColor(Color::GREEN);
            std::cout << "Автоматическая расстановка завершена!" << std::endl;
            Color::resetColor();
            myBoard.display(true);

            if (attempts > 0) {
                std::cout << "Потребовалось " << (attempts + 1) << " попыток для успешной расстановки." << std::endl;
            }

            return true;
        }

        attempts++;
    }

    Color::setColor(Color::RED);
    std::cout << "Не удалось автоматически расставить корабли после " << MAX_ATTEMPTS << " попыток." << std::endl;
    Color::resetColor();
    return false;
}

bool HumanPlayer::parseCoordinate(const std::string& input, Coordinate& coord) {
    coord = Coordinate(-1, -1);

    if (input.empty()) {
        return false;
    }

    std::string str = input;

    size_t start = str.find_first_not_of(" \t");
    if (start == std::string::npos) {
        return false;
    }
    size_t end = str.find_last_not_of(" \t");
    str = str.substr(start, end - start + 1);

    if (str.length() < 2) {
        return false;
    }

    std::transform(str.begin(), str.end(), str.begin(), ::toupper);

    std::string cleaned;
    for (char c : str) {
        if (c != ' ' && c != '-' && c != ':') {
            cleaned += c;
        }
    }

    if (cleaned.empty()) {
        return false;
    }

    char letter = cleaned[0];
    if (letter < 'A' || letter > 'J') {
        return false;
    }

    std::string numberStr = cleaned.substr(1);
    if (numberStr.empty()) {
        return false;
    }

    for (char c : numberStr) {
        if (!isdigit(c)) {
            return false;
        }
    }

    int number;
    try {
        number = std::stoi(numberStr);
    }
    catch (const std::exception&) {
        return false;
    }

    if (number < 1 || number > 10) {
        return false;
    }

    coord.x = letter - 'A';
    coord.y = number - 1;

    return true;
}

std::string HumanPlayer::getSafeInput() {
    std::string input;
    std::getline(std::cin, input);
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    }
    return input;
}

bool HumanPlayer::isValidShipPlacement(int size, const Coordinate& start, Orientation orientation) const {
    Ship tempShip(size, start, orientation);
    return myBoard.isValidPlacement(tempShip);
}
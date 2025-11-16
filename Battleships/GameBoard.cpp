#include "GameBoard.hpp"
#include "Color.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

/**
 * @brief Конструктор - инициализирует поле пустыми клетками
 */
GameBoard::GameBoard() : grid(BOARD_SIZE, std::vector<CellState>(BOARD_SIZE, CellState::Empty)) {}

/**
 * @brief Валидация размещения корабля с учетом правил игры
 */
bool GameBoard::isValidPlacement(const Ship& ship) const {
    const auto& coords = ship.getCoordinates();

    for (const auto& coord : coords) {
        // Проверка границ
        if (coord.x < 0 || coord.x >= BOARD_SIZE || coord.y < 0 || coord.y >= BOARD_SIZE) {
            return false;
        }

        // Проверка что клетка и соседние клетки пусты
        if (!isCellEmpty(coord)) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Размещение корабля на поле после успешной валидации
 */
bool GameBoard::placeShip(const Ship& ship) {
    if (!isValidPlacement(ship)) {
        return false;
    }

    CellState shipState;
    switch (ship.getSize()) {
    case 1: shipState = CellState::Ship1; break;
    case 2: shipState = CellState::Ship2; break;
    case 3: shipState = CellState::Ship3; break;
    case 4: shipState = CellState::Ship4; break;
    default: return false;
    }

    for (const auto& coord : ship.getCoordinates()) {
        grid[coord.y][coord.x] = shipState;
    }

    return true;
}

/**
 * @brief Обработка выстрела с обновлением состояния поля
 */
ShotResult GameBoard::receiveShot(const Coordinate& coord) {
    if (coord.x < 0 || coord.x >= BOARD_SIZE || coord.y < 0 || coord.y >= BOARD_SIZE) {
        return ShotResult::Miss;
    }

    CellState& cell = grid[coord.y][coord.x];

    switch (cell) {
    case CellState::Empty:
        cell = CellState::Miss;
        return ShotResult::Miss;

    case CellState::Ship1:
    case CellState::Ship2:
    case CellState::Ship3:
    case CellState::Ship4:
        cell = CellState::Hit;
        return ShotResult::Hit;

    default:
        return ShotResult::Miss;
    }
}

/**
 * @brief Проверка клетки и ее окружения на возможность размещения корабля
 */
bool GameBoard::isCellEmpty(const Coordinate& coord) const {
    // Проверка самой клетки и всех соседних (включая диагональные)
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int nx = coord.x + dx;
            int ny = coord.y + dy;

            if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
                CellState state = grid[ny][nx];
                if (state != CellState::Empty && state != CellState::Miss) {
                    return false;
                }
            }
        }
    }
    return true;
}

/**
 * @brief Маркировка области вокруг уничтоженного корабля как промахов
 */
void GameBoard::markAreaAroundSunkShip(const Ship& ship) {
    const auto& coords = ship.getCoordinates();

    for (const auto& coord : coords) {
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                int nx = coord.x + dx;
                int ny = coord.y + dy;

                if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
                    CellState currentState = grid[ny][nx];
                    // Помечаем только пустые клетки как промахи
                    if (currentState == CellState::Empty) {
                        grid[ny][nx] = CellState::Miss;
                    }
                }
            }
        }
    }
}

/**
 * @brief Визуализация игрового поля с цветовым оформлением
 */
void GameBoard::display(bool showShips) const {
    // Вывод заголовка с буквами
    std::cout << "  ";
    for (int i = 0; i < BOARD_SIZE; ++i) {
        Color::setColor(Color::YELLOW);
        std::cout << static_cast<char>('A' + i) << " ";
        Color::resetColor();
    }
    std::cout << std::endl;

    // Вывод поля с цифрами
    for (int y = 0; y < BOARD_SIZE; ++y) {
        Color::setColor(Color::YELLOW);
        std::cout << (y + 1);
        if (y < 9) std::cout << " ";
        Color::resetColor();
        std::cout << " ";

        for (int x = 0; x < BOARD_SIZE; ++x) {
            CellState state = grid[y][x];
            char symbol = '~';
            int color = Color::BLUE;

            switch (state) {
            case CellState::Empty:
                symbol = '~';
                color = Color::BLUE;
                break;
            case CellState::Miss:
                symbol = 'O';
                color = Color::BLUE;
                break;
            case CellState::Hit:
                symbol = 'X';
                color = Color::RED;
                break;
            case CellState::Ship1:
                symbol = showShips ? '1' : '~';
                color = showShips ? Color::GREEN : Color::BLUE;
                break;
            case CellState::Ship2:
                symbol = showShips ? '2' : '~';
                color = showShips ? Color::GREEN : Color::BLUE;
                break;
            case CellState::Ship3:
                symbol = showShips ? '3' : '~';
                color = showShips ? Color::GREEN : Color::BLUE;
                break;
            case CellState::Ship4:
                symbol = showShips ? '4' : '~';
                color = showShips ? Color::GREEN : Color::BLUE;
                break;
            }

            Color::setColor(color);
            std::cout << symbol << " ";
            Color::resetColor();
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Получение состояния конкретной клетки с проверкой границ
 */
CellState GameBoard::getCellState(const Coordinate& coord) const {
    if (coord.x < 0 || coord.x >= BOARD_SIZE || coord.y < 0 || coord.y >= BOARD_SIZE) {
        return CellState::Empty;
    }
    return grid[coord.y][coord.x];
}

/**
 * @brief Установка состояния клетки с проверкой границ
 */
void GameBoard::setCellState(const Coordinate& coord, CellState state) {
    if (coord.x >= 0 && coord.x < BOARD_SIZE && coord.y >= 0 && coord.y < BOARD_SIZE) {
        grid[coord.y][coord.x] = state;
    }
}

/**
 * @brief Сброс поля в начальное состояние
 */
void GameBoard::clearBoard() {
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            grid[y][x] = CellState::Empty;
        }
    }
}
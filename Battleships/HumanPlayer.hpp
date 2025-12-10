#pragma once
#include "Player.hpp"
#include "Color.hpp"


class HumanPlayer : public Player {
public:
    HumanPlayer(const std::string& name) : Player(name) {}
    bool isHuman() const override { return true; }
    void placeShips() override;
    void makeMove(Player& enemy) override;
    bool makeMoveWithResult(Player& enemy) override;
    void markAreaAroundDestroyedShip(Player& enemy, const Coordinate& hitCoord) override;
    std::string getPlayerType() const override { return "Human Player"; }

private:
    Coordinate inputCoordinate() const;
    void manualPlacement();
    bool automaticPlacement();
    void markSurroundingCells(const Coordinate& center);
    bool isValidShipPlacement(int size, const Coordinate& start, Orientation orientation) const;
    static bool parseCoordinate(const std::string& input, Coordinate& coord);
    static std::string getSafeInput();
};
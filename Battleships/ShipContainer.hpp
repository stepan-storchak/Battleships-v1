#pragma once
#include "IShip.hpp"
#include <vector>
#include <memory>
#include <algorithm>
#include <type_traits>

template<typename T>
class ShipContainer {
    static_assert(std::is_base_of<IShip, T>::value,
        "T must inherit from IShip");

private:
    std::vector<std::shared_ptr<T>> ships;

public:
    ShipContainer() = default;

    void addShip(std::shared_ptr<T> ship) {
        ships.push_back(ship);
    }

    void removeShip(const std::string& name) {
        auto it = std::remove_if(ships.begin(), ships.end(),
            [&name](const std::shared_ptr<T>& ship) {
                return ship->getName() == name;
            });
        ships.erase(it, ships.end());
    }

    std::shared_ptr<T> findShip(const std::string& name) const {
        auto it = std::find_if(ships.begin(), ships.end(),
            [&name](const std::shared_ptr<T>& ship) {
                return ship->getName() == name;
            });
        return (it != ships.end()) ? *it : nullptr;
    }

    std::vector<std::shared_ptr<T>> findShipsBySize(int size) const {
        std::vector<std::shared_ptr<T>> result;
        std::copy_if(ships.begin(), ships.end(), std::back_inserter(result),
            [size](const std::shared_ptr<T>& ship) {
                return ship->getSize() == size;
            });
        return result;
    }

    void sortBySize() {
        std::sort(ships.begin(), ships.end(),
            [](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
                return a->getSize() < b->getSize();
            });
    }

    void sortByName() {
        std::sort(ships.begin(), ships.end(),
            [](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
                return a->getName() < b->getName();
            });
    }

    int countSunk() const {
        return std::count_if(ships.begin(), ships.end(),
            [](const std::shared_ptr<T>& ship) {
                return ship->isSunk();
            });
    }

    int countActive() const {
        return std::count_if(ships.begin(), ships.end(),
            [](const std::shared_ptr<T>& ship) {
                return !ship->isSunk();
            });
    }

    std::shared_ptr<T> getLargestShip() const {
        if (ships.empty()) return nullptr;
        return *std::max_element(ships.begin(), ships.end(),
            [](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
                return a->getSize() < b->getSize();
            });
    }

    std::shared_ptr<T> getSmallestShip() const {
        if (ships.empty()) return nullptr;
        return *std::min_element(ships.begin(), ships.end(),
            [](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
                return a->getSize() < b->getSize();
            });
    }

    template<typename Predicate>
    std::vector<std::shared_ptr<T>> filter(Predicate pred) const {
        std::vector<std::shared_ptr<T>> result;
        std::copy_if(ships.begin(), ships.end(), std::back_inserter(result), pred);
        return result;
    }

    size_t size() const { return ships.size(); }
    bool empty() const { return ships.empty(); }

    void clear() { ships.clear(); }

    const std::vector<std::shared_ptr<T>>& getAllShips() const { return ships; }

    std::vector<std::string> getAllShipNames() const {
        std::vector<std::string> names;
        std::transform(ships.begin(), ships.end(), std::back_inserter(names),
            [](const std::shared_ptr<T>& ship) { return ship->getName(); });
        return names;
    }

    std::vector<int> getAllShipSizes() const {
        std::vector<int> sizes;
        std::transform(ships.begin(), ships.end(), std::back_inserter(sizes),
            [](const std::shared_ptr<T>& ship) { return ship->getSize(); });
        return sizes;
    }

    bool anySunk() const {
        return std::any_of(ships.begin(), ships.end(),
            [](const std::shared_ptr<T>& ship) { return ship->isSunk(); });
    }

    bool allSunk() const {
        return std::all_of(ships.begin(), ships.end(),
            [](const std::shared_ptr<T>& ship) { return ship->isSunk(); });
    }
};
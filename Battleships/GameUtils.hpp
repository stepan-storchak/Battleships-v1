#pragma once
#include <vector>
#include <algorithm>
#include <numeric>
#include <type_traits>

template<typename T>
T calculateAverage(const std::vector<T>& values) {
    static_assert(std::is_arithmetic<T>::value, "T must be arithmetic type");
    if (values.empty()) return T(0);
    T sum = std::accumulate(values.begin(), values.end(), T(0));
    return sum / static_cast<T>(values.size());
}

template<typename T>
T findMax(const std::vector<T>& values) {
    static_assert(std::is_arithmetic<T>::value, "T must be arithmetic type");
    if (values.empty()) return T(0);
    return *std::max_element(values.begin(), values.end());
}

template<typename T>
T findMin(const std::vector<T>& values) {
    static_assert(std::is_arithmetic<T>::value, "T must be arithmetic type");
    if (values.empty()) return T(0);
    return *std::min_element(values.begin(), values.end());
}

template<typename Container, typename T>
bool contains(const Container& container, const T& value) {
    return std::find(container.begin(), container.end(), value) != container.end();
}

template<typename Container, typename Predicate>
typename Container::value_type findIf(const Container& container, Predicate pred) {
    auto it = std::find_if(container.begin(), container.end(), pred);
    return (it != container.end()) ? *it : typename Container::value_type();
}

template<typename Container, typename T>
void removeAll(Container& container, const T& value) {
    container.erase(std::remove(container.begin(), container.end(), value),
        container.end());
}

template<typename Container, typename Predicate>
void removeIf(Container& container, Predicate pred) {
    container.erase(std::remove_if(container.begin(), container.end(), pred),
        container.end());
}

template<typename InputIt, typename OutputIt, typename Transformer>
void transformRange(InputIt first, InputIt last, OutputIt dest, Transformer trans) {
    std::transform(first, last, dest, trans);
}

template<typename Container, typename T>
int countOccurrences(const Container& container, const T& value) {
    return std::count(container.begin(), container.end(), value);
}

template<typename Container, typename Predicate>
int countIf(const Container& container, Predicate pred) {
    return std::count_if(container.begin(), container.end(), pred);
}
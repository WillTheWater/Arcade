#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/Graphics/Color.hpp>
#include <random>

class RandomManager
{
public:
    template<typename T>
    T Random(T Min, T Max);

    bool Random(double Probablity = 0.5);
    sf::Vector2f Random(sf::Vector2f Min, sf::Vector2f Max);
    sf::Angle Random(sf::Angle Min, sf::Angle Max);
    sf::Color Random(sf::Color Min, sf::Color Max, uint8_t Alpha = 255);

private:
    std::mt19937 Generator{ std::random_device{}() };
};

template<typename T>
T RandomManager::Random(T Min, T Max)
{
    if constexpr (std::is_floating_point_v<T>)
    {
        return std::uniform_real_distribution<T>(Min, Max)(Generator);
    }
    else if constexpr (sizeof(T) == 1)
    {
        return static_cast<T>(std::uniform_int_distribution<int>(Min, Max)(Generator));
    }
    else
    {
        return std::uniform_int_distribution<T>(Min, Max)(Generator);
    }
}
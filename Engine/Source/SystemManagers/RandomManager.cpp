#include "SystemManagers/RandomManager.h"

bool RandomManager::Random(double Probablity)
{
    double SafeProbablity = std::clamp(Probablity, 0.0, 1.0); // Ensure Probablity [0.0, 1.0]
    return std::bernoulli_distribution(SafeProbablity)(Generator);
}

sf::Vector2f RandomManager::Random(sf::Vector2f Min, sf::Vector2f Max)
{
    return { Random(Min.x, Max.x), Random(Min.y, Max.y) };
}

sf::Angle RandomManager::Random(sf::Angle Min, sf::Angle Max)
{
    return sf::degrees(Random(Min.asDegrees(), Max.asDegrees()));
}

sf::Color RandomManager::Random(sf::Color Min, sf::Color Max, std::uint8_t Alpha)
{
    return {
        (uint8_t)(Random(Min.r, Max.r)),
        (uint8_t)(Random(Min.g, Max.g)),
        (uint8_t)(Random(Min.b, Max.b)),
        Alpha
    };
}
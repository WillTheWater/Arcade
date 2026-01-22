#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <optional>
#include <ranges>
#include <vector>
#include <sfml/Audio/Sound.hpp>
#include <SFML/Graphics.hpp>

#include "Core/EngineConfig.h"
#include "Utilities/Assert.h"
#include "Utilities/Timer.h"
#include "Utilities/Log.h"

inline const sf::Font& GetDefaultFont()
{
	static const sf::Font Font("Content/Assets/Fonts/gamefont.ttf");
	return Font;
}

inline bool IsOutsideWindowLeft(const sf::Shape& Bounds)
{
	return Bounds.getPosition().x < Bounds.getGlobalBounds().size.x / 2.f;
}

inline bool IsOutsideWindowRight(const sf::Shape& Bounds)
{
	return Bounds.getPosition().x > EConfig.WindowSize.x - Bounds.getGlobalBounds().size.x / 2.f;
}

inline bool IsOutsideWindowTop(const sf::Shape& Bounds)
{
	return Bounds.getPosition().y < Bounds.getGlobalBounds().size.y / 2.f;
}

inline bool IsOutsideWindowBottom(const sf::Shape& Bounds)
{
	return Bounds.getPosition().y > EConfig.WindowSize.y - Bounds.getGlobalBounds().size.y / 2.f;
}

inline bool IsOutsideWindow(sf::Vector2f Point)
{
	return Point.x < 0 || Point.y < 0 ||
		   Point.x >= EConfig.WindowSize.x ||
		   Point.y >= EConfig.WindowSize.y;
}

inline std::optional<sf::FloatRect> Intersects(const sf::Shape& ObjectA, const sf::Shape& ObjectB)
{
	return ObjectA.getGlobalBounds().findIntersection(ObjectB.getGlobalBounds());
}

inline float DistanceSquared(sf::Vector2f PointA, sf::Vector2f PointB)
{
	return (PointA - PointB).lengthSquared();
}

inline float Distance(sf::Vector2f PointA, sf::Vector2f PointB)
{
	return (PointA - PointB).length();
}

inline bool Contains(const sf::Shape& Shape, sf::Vector2f Point)
{
	return Shape.getGlobalBounds().contains(Point);
}

inline bool Contains(const sf::CircleShape& Circle, sf::Vector2f Point)
{
	return DistanceSquared(Circle.getPosition(), Point) <= std::pow(Circle.getRadius(), 2);
}

inline sf::Vector2f Lerp(sf::Vector2f Start, sf::Vector2f End, float Time)
{
	return { std::lerp(Start.x, End.x, Time), std::lerp(Start.y, End.y, Time) };
}
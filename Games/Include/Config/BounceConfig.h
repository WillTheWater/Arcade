#pragma once

#include "Scenes/Scene.h"

namespace Bounce
{
    const sf::Vector2f PADDLE_SIZE(200, 40);
    const int PADDLE_LIVES = 10;
    const float PADDLE_SPEED = 750;

    const float BALL_RADIUS = 25;
    const float BALL_SPEED = 250;
    const float BALL_SPAWN_COOLDOWN_DURATION = 2;

    const sf::Color STATS_TEXT_COLOR(sf::Color::White);
    const sf::Color OBJECT_OUTLINE_COLOR(sf::Color::White);
    const float OBJECT_OUTLINE_THICKNESS(-2.f);
    const int STATS_TEXT_SIZE = 48;
    const sf::Color OUTLINE_COLOR({ 65,96,157 });
    const float OUTLINE_THICKNESS(6.f);

    const float EXTRA_LIFE_SPAWN_INTERVAL = 30.f;
    const float EXTRA_LIFE_FALL_SPEED = 150.f;
    const float EXTRA_LIFE_SIZE = 40.f;
    const float EXTRA_LIFE_ROTATION_SPEED = 180.f; // degrees/sec
    const sf::Color EXTRA_LIFE_COLOR({ 255, 215, 0 }); // gold-ish

    const sf::Color PADDLE_COLOR({ 65,96,157 });
    const std::array<sf::Color, 5> BALL_COLORS =
    {
        sf::Color(85, 114, 156),
        sf::Color(189, 125, 193),
        sf::Color(109, 1, 73),
        sf::Color(107, 26, 152),
        sf::Color(162, 9, 121)
    };

    const std::string_view STATS_HIGH_SCORE_KEY = "Bounce:High Score";
}
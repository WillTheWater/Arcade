#pragma once

#include "Scenes/Scene.h"

namespace Dino
{
    const sf::Vector2f DINO_SIZE(60, 160);          // 2x height
    const sf::Vector2f DINO_DUCK_SIZE(60, 80);      // half height
    const float DINO_RUN_SPEED = 600.f;
    const float DINO_JUMP_FORCE = 1000.f;
    const float GRAVITY = 2800.f;

    const sf::Vector2f OBSTACLE_SIZE(40, 60);
    const float OBSTACLE_SPEED = 600.f;
    const float OBSTACLE_SPAWN_INTERVAL = 1.5f;

    const sf::Vector2f GROUND_SIZE(1054, 50);
    const float GROUND_Y_RATIO = 0.75f;

    const sf::Color DINO_COLOR(sf::Color::White);
    const sf::Color OBSTACLE_COLOR(sf::Color::Red);
    const sf::Color GROUND_COLOR(sf::Color(80, 80, 80));

    const sf::Color STATS_TEXT_COLOR(sf::Color::White);
    const int STATS_TEXT_SIZE = 48;

    const std::string_view STATS_HIGH_SCORE_KEY = "Dino:HighScore";
}

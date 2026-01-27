#pragma once

#include "Scenes/Scene.h"

namespace Duck
{
    // Duck
    const sf::Vector2f DUCK_COLLISION_SIZE(60, 160);
    const sf::Vector2f DUCK_DUCK_COLLISION_SIZE(60, 80);
    const sf::Vector2f DUCK_SPRITE_SIZE(192, 196);
    const float DUCK_RUN_SPEED = 600.f;
    const float DUCK_JUMP_FORCE = 1100.f;
    const float GRAVITY = 2900.f;
    const float OFFSET = 24.f;

    // Obstacles
    const sf::Vector2f OBSTACLE_SIZE(50, 50);
    const float OBSTACLE_BASE_SPEED = 600.f;         // CHANGED
    const float OBSTACLE_MAX_SPEED = 2400.f;         // NEW
    const float OBSTACLE_SPEED_RAMP = 120.f;          // NEW

    const float OBSTACLE_SPAWN_INTERVAL = 1.5f;
    const float OBSTACLE_MIN_DISTANCE = 350.f;       // NEW
    const float OBSTACLE_MAX_DISTANCE = 650.f;       // NEW

    const float OBSTACLE_DUCK_HEIGHT = 160.f;        // NEW
    const float OBSTACLE_MID_HEIGHT = 110.f;         // NEW

    // Ground
    const sf::Vector2f GROUND_SIZE(1054, 50);
    const float GROUND_Y_RATIO = 0.75f;

    // Colors
    const sf::Color DUCK_COLOR(sf::Color::White);
    const sf::Color OBSTACLE_COLOR(189, 125, 193);
    const sf::Color GROUND_COLOR({ 65,96,157 });

    // UI
    const sf::Color STATS_TEXT_COLOR(sf::Color::White);
    const int STATS_TEXT_SIZE = 48;
    const std::string_view STATS_HIGH_SCORE_KEY = "Dino:HighScore";

    // Assets
    const std::string RUN_SPRITESHEET_FILENAME = "runsheet.png";
    const std::string DUCKING_RUN_SPRITESHEET_FILENAME = "duckedsheet.png";
}

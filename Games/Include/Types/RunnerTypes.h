#pragma once
#include "Config/RunnerConfig.h"

namespace Dino
{
    enum Action
    {
        Duck
    };

    struct Dino
    {
        sf::RectangleShape Shape;
        sf::Vector2f Velocity;
        bool IsOnGround = true;
        bool IsDucking = false;
    };

    struct Obstacle
    {
        sf::RectangleShape Shape;
    };

    struct Stats
    {
        int Score;
        int HighScore;
        sf::Text ScoreText{ GetDefaultFont() };
        sf::Text HighScoreText{ GetDefaultFont() };
    };
}

#pragma once
#include "Config/RunnerConfig.h"

namespace Duck
{
    enum Action
    {
        Ducked
    };

    struct Duck
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

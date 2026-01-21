#pragma once

#include "Config/BounceConfig.h"

namespace Bounce
{
    enum Action
    {
        MoveLeft, 
        MoveRight
    };

    struct Paddle
    {
        sf::RectangleShape Shape;
        float Speed;
    };

    struct Ball
    {
        sf::CircleShape Shape;
        sf::Vector2f Direction;
        float Speed;
    };

    struct Stats
    {
        int Score;
        int HighScore;
        int Lives;
        sf::Text ScoreText{ GetDefaultFont() };
        sf::Text HighScoreText{ GetDefaultFont() };
        sf::Text LivesText{ GetDefaultFont() };
    };
}
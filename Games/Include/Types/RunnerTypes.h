#pragma once
#include "Config/RunnerConfig.h"

namespace Duck
{
    enum Action
    {
        Ducked
    };

    enum class AnimState
    {
        Run,
        Jump,
        DuckRun,
        DuckJump
    };

    struct Duck
    {
        sf::RectangleShape Shape;
        sf::Vector2f Velocity;
        bool IsOnGround = true;
        bool IsDucking = false;
        AnimState State = AnimState::Run;
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

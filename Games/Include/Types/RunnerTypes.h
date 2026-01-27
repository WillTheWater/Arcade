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

    enum class ObstacleType            // NEW
    {
        JumpOnly,
        DuckOnly,
        Either
    };

    struct Duck
    {
        sf::RectangleShape Shape;
        sf::Vector2f Velocity;
        bool IsOnGround = true;
        bool IsDucking = false;
        AnimState State = AnimState::Run;
    };

    struct Obstacle                    // CHANGED
    {
        sf::RectangleShape Shape;
        ObstacleType Type;
    };

    struct Stats
    {
        int Score;
        int HighScore;
        sf::Text ScoreText{ GetDefaultFont() };
        sf::Text HighScoreText{ GetDefaultFont() };
    };
}

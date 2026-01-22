#pragma once

#include "Config/ClickerConfig.h"

namespace Clicker
{
    enum Action
    {
        Click
    };

    struct Target
    {
        sf::CircleShape Shape;
        Timer TeleportCooldown;
    };

    struct Stats
    {
        int Score;
        Timer FinalCooldown;
        float BestTime;
        sf::Text ScoreText{ GetDefaultFont() };
        sf::Text FinalCooldownText{ GetDefaultFont() };
        sf::Text BestTimeText{ GetDefaultFont() };
    };
}
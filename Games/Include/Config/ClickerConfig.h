#pragma once

#include "Scenes/Scene.h"

namespace Clicker
{
    const float CURSOR_SPEED = 1200;
    
    const float TARGET_RADIUS = 30;
    const float TARGET_TELEPORT_COOLDOWN_DURATION = 2;
    
    const int STATS_SCORE_GOAL = 20;
    const float STATS_FINAL_COOLDOWN_DURATION = 20;
    const int STATS_TEXT_SIZE = 50;
    
    const sf::Color TARGET_COLOR(sf::Color::White);
    const sf::Color STATS_SCORE_TEXT_COLOR(sf::Color::White);
    const sf::Color STATS_FINAL_COOLDOWN_TEXT_COLOR(sf::Color::White);
    const sf::Color STATS_BEST_TIME_TEXT_COLOR(sf::Color::White);
    const sf::Color OUTLINE_COLOR({ 65,96,157 });
    const float OUTLINE_THICKNESS(6.f);
    
    const std::string TARGET_TEXTURE_FILENAME = "Target.png";
    
    const std::string_view STATS_BEST_TIME_KEY = "Clicker:Best Time";
    
}
#pragma once

#pragma once

#include "Config/MainMenuConfig.h"

namespace Menu
{
    struct Button
    {
        sf::RectangleShape Shape;
        std::string Name;
        sf::Text Text{ GetDefaultFont() };
    };

    struct Controls
    {
        sf::RectangleShape Background;
        std::vector<sf::Text> Texts;
        std::optional<int> Current;
    };
}
#pragma once

#include <sfml/System/Vector2.hpp>
#include <sfml/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <String>

struct EngineConfig
{
	std::string WindowTitle;
	sf::Vector2f WindowSize;
	bool DisableSFMLLogging;
	sf::Time MaxDeltaTime;
	float GlobalVolume;
	sf::Color BackgroundColor;

	EngineConfig();
};

inline const EngineConfig EConfig;
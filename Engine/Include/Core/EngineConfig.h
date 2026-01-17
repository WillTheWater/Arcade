#pragma once

#include <sfml/System/Vector2.hpp>
#include <String>

struct EngineConfig
{
	std::string WindowTitle;
	sf::Vector2f WindowSize;

	EngineConfig();
};

inline const EngineConfig EngConfig;
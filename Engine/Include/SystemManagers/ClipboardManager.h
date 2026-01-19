#pragma once

#include <SFML/System/String.hpp>

class ClipboardManager
{
public:
	void SetString(const sf::String& Text);
	sf::String GetString() const;
};
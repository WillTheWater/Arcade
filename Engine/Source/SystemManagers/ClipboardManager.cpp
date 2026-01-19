#include <SFML/Window/Clipboard.hpp>

#include "SystemManagers/ClipboardManager.h"

void ClipboardManager::SetString(const sf::String& Text)
{
	sf::Clipboard::setString(Text);
}

sf::String ClipboardManager::GetString() const
{
	return sf::Clipboard::getString();
}

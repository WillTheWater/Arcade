#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

class ScreenshotManager
{
public:
	ScreenshotManager(const sf::RenderWindow& Window);

	void TakeScreenshot() const;

private:
	const sf::RenderWindow& Window;
};
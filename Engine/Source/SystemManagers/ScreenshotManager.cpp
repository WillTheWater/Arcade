#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <chrono>
#include <format>
#include <string>

#include <iostream>
#include <filesystem>

#include "SystemManagers/ScreenshotManager.h"
#include "Utilities/Log.h"

ScreenshotManager::ScreenshotManager(const sf::RenderWindow& Window)
	: Window{Window}
{
}

void ScreenshotManager::TakeScreenshot() const
{
	std::cout << std::filesystem::current_path() << '\n';

	std::string Filename = std::format("Screenshot_{:%Y%m%d_%H%M%S}.png",
		floor<std::chrono::microseconds>(std::chrono::system_clock::now()));

	//Filename.replace(Filename.find('.'), 1, "_");
	sf::Texture Screenshot(Window.getSize());
	Screenshot.update(Window);

	if (Screenshot.copyToImage().saveToFile("Content/Screenshots/" + Filename))
	{
		LOG("Successfully saved screenshot: {}", Filename);
	}
	else
	{
		LOG("Failed to save screenshot: {}", Filename);
	}
}
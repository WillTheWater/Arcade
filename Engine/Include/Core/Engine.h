#pragma once

#include <SFML/Graphics.hpp>

class Engine
{
public:
	Engine();

	void ProcessEvents();
	void Update();
	void Render();

	bool IsRunning() const;


private:
	sf::RenderWindow GameWindow;
};
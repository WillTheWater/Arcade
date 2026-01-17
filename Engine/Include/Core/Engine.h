#pragma once

#include <SFML/Graphics.hpp>

#include "Core/EngineVisitor.h"

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
	friend EngineVisitor;

private:
	void EventWindowClose();
};
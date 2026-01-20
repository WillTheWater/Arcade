#pragma once

#include <SFML/Graphics.hpp>

#include "Core/EngineVisitor.h"
#include "Core/Managers.h"

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
	Managers Manager;

	friend EngineVisitor;

private:
	void EventWindowClose();
	void EventWindowResized(sf::Vector2u Size);
	void EventWindowFocusLost();
	void EventWindowFocusGained();
	void EventScreenshot() const;
	void EventGamepadConnected(int GamepadID);
	void EventGamepadDisconnected(int GamepadID);
};
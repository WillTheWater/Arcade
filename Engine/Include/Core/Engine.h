#pragma once

#include <SFML/Graphics.hpp>

#include "Core/EngineVisitor.h"
#include "Core/Managers.h"
#include "Scenes/SceneFactory.h"
#include "Core/Overlay.h"

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
	
	SceneFactory::Scenes Scenes;
	Scene* CurrentScene;

	bool CursorVisible;
	bool Fullscreen;
	Overlay PauseMenu;

	sf::Vector2f ScreenSize;
	sf::Vector2f ScreenCenter;
	float ScaleFactor;

	friend EngineVisitor;

private:
	void Calculate();
	void EventWindowClose();
	void EventWindowResized(sf::Vector2u Size);
	void EventWindowFocusLost();
	void EventWindowFocusGained();
	void EventScreenshot() const;
	void EventGamepadConnected(int GamepadID);
	void EventGamepadDisconnected(int GamepadID);
	void EventChangeScene(const std::string& SceneName);
	void EventRestartScene();
	void EventReturnToMainMenu();
	void EventToggleFullscreen();
	void EventPauseMenuToggle();
	void EventPauseMenuSelection(OverlaySelection Selection);
};
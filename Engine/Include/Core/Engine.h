#pragma once

#include <SFML/Graphics.hpp>

#include "Core/Managers.h"
#include "Core/EngineVisitor.h"
#include "Core/WindowController.h"
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
	bool HasFocus() const;


private:
	friend EngineVisitor;

	sf::RenderWindow GameWindow;
	Managers Manager;
	
	SceneFactory::Scenes Scenes;
	Scene* CurrentScene;

	bool CursorVisible;
	Overlay PauseMenu;
	WindowControls WindowControl;

	bool Maximized;
	sf::Vector2u PrevSize;
	sf::Vector2i PrevPos;

private:
	void EventWindowClose();
	void EventWindowResized(sf::Vector2u Size);
	void EventWindowMaximize();
	void EventWindowMinimize();
	void EventWindowFocusLost();
	void EventWindowFocusGained();
	void EventScreenshot() const;
	void EventGamepadConnected(int GamepadID);
	void EventGamepadDisconnected(int GamepadID);
	void EventChangeScene(const std::string& SceneName);
	void EventRestartScene();
	void EventReturnToMainMenu();
	void EventPauseMenuToggle();
	void EventPauseMenuSelection(OverlaySelection Selection);
	void EventWindowAction();
};
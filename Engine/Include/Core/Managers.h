#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "SystemManagers/RandomManager.h"
#include "SystemManagers/TimeManager.h"
#include "SystemManagers/SaveManager.h"
#include "SystemManagers/ClipboardManager.h"
#include "SystemManagers/AssetManager.h"
#include "SystemManagers/AudioManager.h"
#include "SystemManagers/InputManager.h"
#include "SystemManagers/RenderManager.h"
#include "SystemManagers/ScreenshotManager.h"
#include "Systemmanagers/CursorManager.h"
#include "SystemManagers/GUIManager.h"

struct Managers
{
	RandomManager Randomizer;
	TimeManager Timer;
	SaveManager Save;
	ClipboardManager Clipboard;
	AssetManager Assets;
	AudioManager Audio;
	InputManager Input;
	RenderManager Renderer;
	ScreenshotManager ScreenShot;
	CursorManager Cursor;
	GUIManager GUI;

	Managers(sf::RenderWindow& Window) 
		: ScreenShot{ Window } 
		, Cursor{Window}
		, GUI{Window}
	{}
};
#include "Core/Engine.h"
#include "Core/EngineConfig.h"
#include "Utilities/Log.h"

Engine::Engine()
			   // sf::VideoMode::getDesktopMode()
	: GameWindow{ sf::VideoMode(sf::Vector2u(EConfig.WindowSize)), EConfig.WindowTitle, sf::Style::Default, sf::State::Windowed}
	, Manager{GameWindow}
{
	GameWindow.setIcon(sf::Image("Content/Assets/Textures/icon.png"));
	GameWindow.setMinimumSize(GameWindow.getSize() / 2u);
	if (EConfig.DisableSFMLLogging) { sf::err().rdbuf(nullptr); }
	Manager.Audio.SetGlobalVolume(EConfig.GlobalVolume);
	GameWindow.setKeyRepeatEnabled(false);
	
	// For Debug ONLY
	GameWindow.setPosition(sf::Vector2i((int)(EConfig.WindowSize.x / 2), (int)(EConfig.WindowSize.y / 2)));
	Manager.Save.Set<int>("Score", 100);
	int Score = Manager.Save.Get<int>("Score");
	LOG("Score: {}", Score);
	LOG("Global Volume: {}", EConfig.GlobalVolume);
}

void Engine::ProcessEvents()
{
	while (std::optional<sf::Event> Event = GameWindow.pollEvent())
	{
		Event->visit(EngineVisitor{ *this });
	}
}

void Engine::Update()
{
	Manager.Timer.Tick();
}

void Engine::Render()
{
	GameWindow.clear(sf::Color::Black);
	Manager.Renderer.BeginDrawing();
	GameWindow.draw(sf::Sprite(Manager.Renderer.FinishDrawing()));
	GameWindow.display();
}

bool Engine::IsRunning() const
{
	return GameWindow.isOpen();
}

void Engine::EventWindowClose()
{
	GameWindow.close();
	LOG("Window Closed After: {:.2f} seconds", Manager.Timer.GetElapsedTime());
}

void Engine::EventWindowResized(sf::Vector2u Size)
{
	LOG("Num: {}", Manager.Randomizer.Random(1, 10));
	LOG("Num: {}", Manager.Randomizer.Random(1.f, (float)Size.y));
	LOG("Bool: {}", Manager.Randomizer.Random());
}

void Engine::EventWindowFocusLost()
{
	LOG("Window Focus Lost!");
}

void Engine::EventWindowFocusGained()
{
	LOG("Window Focus Gained!");
}

void Engine::EventScreenshot() const
{
	Manager.ScreenShot.TakeScreenshot();
}

void Engine::EventGamepadConnected(int GamepadID)
{
	LOG("Gamepad Connected {}", GamepadID);
}

void Engine::EventGamepadDisconnected(int GamepadID)
{
	LOG("Gamepad Disconnected {}", GamepadID);
}

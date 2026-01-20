#include "Core/Engine.h"
#include "Core/EngineConfig.h"
#include "Utilities/Log.h"

Engine::Engine()
			   // sf::VideoMode::getDesktopMode()
	: GameWindow{ sf::VideoMode(sf::Vector2u(EConfig.WindowSize)), EConfig.WindowTitle, sf::Style::Default, sf::State::Windowed}
	, Manager{GameWindow}
	, Scenes{SceneFactory::CreateScenes(Manager)}
	, CurrentScene{nullptr}
	, PauseMenu{Manager.GUI}
	, CursorVisible{true}
{
	GameWindow.setIcon(sf::Image("Content/Assets/Textures/icon.png"));
	GameWindow.setMinimumSize(GameWindow.getSize() / 2u);
	if (EConfig.DisableSFMLLogging) { sf::err().rdbuf(nullptr); }
	Manager.Audio.SetGlobalVolume(EConfig.GlobalVolume);
	GameWindow.setKeyRepeatEnabled(false);
	GameWindow.setMouseCursorVisible(false);
	
	// For Debug ONLY
	GameWindow.setPosition(sf::Vector2i((int)(EConfig.WindowSize.x / 2), (int)(EConfig.WindowSize.y / 2)));
	Manager.Save.Set<int>("Score", 100);
	int Score = Manager.Save.Get<int>("Score");
	LOG("Score: {}", Score);
	LOG("Global Volume: {}", EConfig.GlobalVolume);
}

void Engine::ProcessEvents()
{
	if (const auto NextScene = Manager.Scene.GetNextScene())
	{
		EventChangeScene(*NextScene);
	}
	while (std::optional<sf::Event> Event = GameWindow.pollEvent())
	{
		Event->visit(EngineVisitor{ *this });
		Manager.GUI.HandleEvents(*Event);
		//if (!PauseMenu.IsVisible())
		//{
		//	//CurrentScene->OnEvent(*Event);
		//}
	}
	if (const auto Selection = PauseMenu.GetSelection())
	{
		EventPauseMenuSelection(*Selection);
	}
}

void Engine::Update()
{
	Manager.Timer.Tick();
	Manager.Cursor.Update(Manager.Timer.GetDeltaTime());
	if (!PauseMenu.IsVisible())
	{
		//CurrentScene->Update();
	}
}

void Engine::Render()
{
	GameWindow.clear(sf::Color::Black);
	Manager.Renderer.BeginDrawing();
	//CurrentScene->Render();
	GameWindow.draw(sf::Sprite(Manager.Renderer.FinishDrawing()));
	Manager.GUI.Render();
	Manager.Cursor.Render();
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
	//CurrentScene->OnPause(true);
	LOG("Window Focus Lost!");
}

void Engine::EventWindowFocusGained()
{
	//CurrentScene->OnPause(PauseMenu.IsVisible());
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
	//CurrentScene->OnPause(true);
	PauseMenu.SetVisible(true);
	LOG("Gamepad Disconnected {}", GamepadID);
}

void Engine::EventChangeScene(const std::string& SceneName)
{
	ASSERT(Scenes.contains(SceneName));
	Scene* NextScene = Scenes.at(SceneName).get();
	
	if (CurrentScene)
	{
		CurrentScene->OnCleanup();
	}

	Manager.Input.Clear();
	//CurrentScene = NextScene;
	//CurrentScene->Start();
}

void Engine::EventRestartScene()
{
	PauseMenu.SetVisible(false);
	Manager.Scene.ReloadScene();
}

void Engine::EventReturnToMainMenu()
{
	PauseMenu.SetVisible(false);
	Manager.Scene.ChangeScene("MainMenu");
	Manager.Cursor.SetVisibility(true);
	Manager.Cursor.SetCursorSpeed(EConfig.CursorSpeed);
}

void Engine::EventPauseMenuToggle()
{
	const bool Visibility = !PauseMenu.IsVisible();
	PauseMenu.SetVisible(Visibility);

	const bool CursorVisibility = Manager.Cursor.IsVisible();
	Manager.Cursor.SetVisibility(Visibility || CursorVisibility);
	CursorVisible = CursorVisibility;

	//CurrentScene->OnPause(Visibility);
	LOG(Visibility ? "Game Paused!" : "Game Unpaused!");
}

void Engine::EventPauseMenuSelection(OverlaySelection Selection)
{
	switch (Selection)
	{
	case OverlaySelection::Resume:
		EventPauseMenuToggle();
		break;
	case OverlaySelection::Restart:
		EventRestartScene();
		break;
	case OverlaySelection::MainMenu:
		EventReturnToMainMenu();
		break;
	case OverlaySelection::Quit:
		EventWindowClose();
		break;
	default:
		break;
	}
}

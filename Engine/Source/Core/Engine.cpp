#include "Core/Engine.h"
#include "Core/EngineConfig.h"
#include "Utilities/Log.h"

Engine::Engine()
	: GameWindow{ sf::VideoMode::getDesktopMode(), EConfig.WindowTitle, sf::Style::None, sf::State::Windowed}
	, Manager{GameWindow}
	, Scenes{SceneFactory::CreateScenes(Manager)}
	, CurrentScene{nullptr}
	, PauseMenu{Manager.GUI}
	, CursorVisible{true}
	, Fullscreen{true}
{
	GameWindow.setIcon(sf::Image("Content/Assets/Textures/icon.png"));
	GameWindow.setMinimumSize(sf::VideoMode::getDesktopMode().size / 4u);
	if (EConfig.DisableSFMLLogging) { sf::err().rdbuf(nullptr); }
	Manager.Audio.SetGlobalVolume(EConfig.GlobalVolume);
	GameWindow.setKeyRepeatEnabled(false);
	GameWindow.setMouseCursorVisible(false);
	Calculate();
	Manager.Scene.ChangeScene("Bounce");
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
		if (!PauseMenu.IsVisible())
		{
			CurrentScene->OnEvent(*Event);
		}
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
		CurrentScene->Update();
	}
}

void Engine::Render()
{
	GameWindow.clear(sf::Color::Black);
	Manager.Renderer.BeginDrawing();
	CurrentScene->Render();

	auto RenderedScene = sf::Sprite(Manager.Renderer.FinishDrawing());
	RenderedScene.setScale({ ScaleFactor, ScaleFactor });
	RenderedScene.setOrigin({ RenderedScene.getTexture().getSize().x / 2.f, RenderedScene.getTexture().getSize().y / 2.f });
	RenderedScene.setPosition(ScreenCenter);

	GameWindow.draw(RenderedScene);
	Manager.GUI.Render();
	Manager.Cursor.Render();
	GameWindow.display();
}

bool Engine::IsRunning() const
{
	return GameWindow.isOpen();
}

bool Engine::HasFocus() const
{
	return GameWindow.hasFocus();
}

void Engine::Calculate()
{
	ScreenSize = sf::Vector2f(sf::VideoMode::getDesktopMode().size);
	ScreenCenter = sf::Vector2f(ScreenSize.x / 2.f, ScreenSize.y / 2.f);
	ScaleFactor = 1080 / EConfig.WindowSize.x;
}

void Engine::EventWindowClose()
{
	GameWindow.close();
	LOG("Window Closed After: {:.2f} seconds", Manager.Timer.GetElapsedTime());
}

void Engine::EventWindowResized(sf::Vector2u Size)
{
}

void Engine::EventWindowFocusLost()
{
	CurrentScene->OnPause(true);
}

void Engine::EventWindowFocusGained()
{
	CurrentScene->OnPause(PauseMenu.IsVisible());
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
	CurrentScene->OnPause(true);
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
	CurrentScene = NextScene;
	CurrentScene->Start();
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

void Engine::EventToggleFullscreen()
{
	auto Desktop = sf::VideoMode::getDesktopMode();
	sf::Vector2u ScreenSize = Desktop.size;
	if (Fullscreen)
	{
		GameWindow.setSize(ScreenSize / 2u);
		GameWindow.setPosition((sf::Vector2i)ScreenSize / 4);
	}
	else
	{
		GameWindow.setSize(ScreenSize);
		GameWindow.setPosition({ 0,0 });
	}
	Fullscreen = !Fullscreen;
}

void Engine::EventPauseMenuToggle()
{
	const bool Visibility = !PauseMenu.IsVisible();
	PauseMenu.SetVisible(Visibility);

	const bool CursorVisibility = Manager.Cursor.IsVisible();
	Manager.Cursor.SetVisibility(Visibility || CursorVisibility);
	CursorVisible = CursorVisibility;

	CurrentScene->OnPause(Visibility);
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
	case OverlaySelection::Main_Menu:
		EventReturnToMainMenu();
		break;
	case OverlaySelection::Toggle_Fullscreen:
		EventToggleFullscreen();
		break;
	case OverlaySelection::Quit:
		EventWindowClose();
		break;
	default:
		break;
	}
}

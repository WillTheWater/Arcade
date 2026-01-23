#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include "Core/Engine.h"
#include "Core/EngineConfig.h"
#include "Utilities/Log.h"

Engine::Engine()
	: GameWindow{ sf::VideoMode(sf::Vector2u(EConfig.WindowSize)), EConfig.WindowTitle, sf::Style::None, sf::State::Windowed}
	, Manager{GameWindow}
	, Scenes{SceneFactory::CreateScenes(Manager)}
	, CurrentScene{nullptr}
	, PauseMenu{Manager.GUI}
	, WindowControl{ Manager }
	, CursorVisible{true}
	, Maximized{false}
{
	GameWindow.setVerticalSyncEnabled(true);
	GameWindow.setIcon(sf::Image("Content/Assets/Textures/icon.png"));
	GameWindow.setMinimumSize(GameWindow.getSize() / 2u);
	if (EConfig.DisableSFMLLogging) { sf::err().rdbuf(nullptr); }
	Manager.Audio.SetGlobalVolume(EConfig.GlobalVolume);
	GameWindow.setKeyRepeatEnabled(false);
	GameWindow.setMouseCursorVisible(false);
	//GameWindow.setMouseCursorGrabbed(true);
	EventWindowAction();

	// =================== GAME =============================
	Manager.Scene.ChangeScene("MainMenu");
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
		WindowControl.OnEvent(*Event);
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
	WindowControl.Update();
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
	WindowControl.Render();
	GameWindow.draw(sf::Sprite(Manager.Renderer.FinishDrawing()));
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

void Engine::EventWindowClose()
{
	GameWindow.close();
}

void Engine::EventWindowResized(sf::Vector2u Size)
{
}

void Engine::EventWindowMaximize()
{
	auto desktop = sf::VideoMode::getDesktopMode();

	LOG("Mouse Needs Fixed");
	/*if (!Maximized)
	{
		PrevSize = GameWindow.getSize();
		PrevPos = GameWindow.getPosition();

		sf::Vector2u newSize = { std::min(desktop.size.x, desktop.size.y),
								 std::min(desktop.size.x, desktop.size.y) };
		GameWindow.setSize(newSize);
		GameWindow.setPosition(
			{
				sf::Vector2i((desktop.size.x - newSize.x) / 2,
				(desktop.size.y - newSize.y) / 2 )
			});

		Maximized = true;
	}
	else
	{
		GameWindow.setSize(PrevSize);
		GameWindow.setPosition(PrevPos);

		Maximized = false;
	}*/
}


void Engine::EventWindowMinimize()
{
#ifdef _WIN32
	HWND hwnd = static_cast<HWND>(GameWindow.getNativeHandle());
	ShowWindow(hwnd, SW_MINIMIZE);
#endif
}

void Engine::EventWindowFocusLost()
{
	LOG("LOST FOCUS");
	CurrentScene->OnPause(true);
}

void Engine::EventWindowFocusGained()
{
	LOG("GAINED FOCUS");
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

void Engine::EventPauseMenuToggle()
{
	const bool Visibility = !PauseMenu.IsVisible();
	PauseMenu.SetVisible(Visibility);

	const bool CursorVisibility = Manager.Cursor.IsVisible();
	Manager.Cursor.SetVisibility(Visibility || CursorVisible);
	CursorVisible = CursorVisibility;

	CurrentScene->OnPause(Visibility);
	//LOG(Visibility ? "Game Paused!" : "Game Unpaused!");
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
	case OverlaySelection::Quit:
		EventWindowClose();
		break;
	default:
		break;
	}
}

void Engine::EventWindowAction()
{
	WindowControl.SetActionCallback
	(
		[this](WindowAction action)
		{
			switch (action)
			{
			case WindowAction::Close: EventWindowClose(); break;
			case WindowAction::Maximize: EventWindowMaximize(); break;
			case WindowAction::Minimize: EventWindowMinimize(); break;
			}
		}
	);
}
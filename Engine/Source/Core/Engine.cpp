#include "Core/Engine.h"

Engine::Engine()
	: GameWindow{ sf::VideoMode::getDesktopMode(), "Arcade", sf::Style::Default, sf::State::Fullscreen }
{
	GameWindow.setIcon(sf::Image("Assets/icon.png"));
	GameWindow.setMinimumSize(GameWindow.getSize() / 2u);
}

void Engine::ProcessEvents()
{
	while (std::optional<sf::Event> Event = GameWindow.pollEvent())
	{
		if (const auto* KeyEvent = Event->getIf<sf::Event::KeyPressed>())
		{
			if (KeyEvent->code == sf::Keyboard::Key::Escape || Event->is<sf::Event::Closed>())
			{
				GameWindow.close();
			}
		}
	}
}

void Engine::Update()
{

}

void Engine::Render()
{
	GameWindow.clear(sf::Color::Black);

	GameWindow.display();
}

bool Engine::IsRunning() const
{
	return GameWindow.isOpen();
}
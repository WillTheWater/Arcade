#include "Core/Engine.h"

Engine::Engine()
	: GameWindow{ sf::VideoMode::getDesktopMode(), "Arcade", sf::Style::Default, sf::State::Windowed }
{
	GameWindow.setIcon(sf::Image("Assets/icon.png"));
	GameWindow.setMinimumSize(GameWindow.getSize() / 2u);
	
	// For Debug ONLY
	GameWindow.setSize({ 1920u, 1080u });
	GameWindow.setPosition({ 1920/2, 1080/2 });
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

void Engine::EventWindowClose()
{
	GameWindow.close();
}

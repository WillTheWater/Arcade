#include "Core/Engine.h"
#include "Core/EngineConfig.h"

Engine::Engine()
	: GameWindow{ sf::VideoMode::getDesktopMode(), EngConfig.WindowTitle, sf::Style::Default, sf::State::Windowed }
{
	GameWindow.setIcon(sf::Image("Assets/icon.png"));
	GameWindow.setMinimumSize(GameWindow.getSize() / 2u);
	
	// For Debug ONLY
	GameWindow.setSize(sf::Vector2u(EngConfig.WindowSize));
	GameWindow.setPosition(sf::Vector2i(EngConfig.WindowSize.x / 2, EngConfig.WindowSize.y / 2));
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

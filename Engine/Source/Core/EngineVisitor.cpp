#include "Core/EngineVisitor.h"
#include "Core/Engine.h"

void EngineVisitor::operator()(const sf::Event::Closed&)
{
	GameEngine.EventWindowClose();
}

void EngineVisitor::operator()(const sf::Event::Resized& Resize)
{
	GameEngine.EventWindowResized(Resize.size);
}

void EngineVisitor::operator()(const sf::Event::FocusLost&)
{
	GameEngine.EventWindowFocusLost();
}

void EngineVisitor::operator()(const sf::Event::FocusGained&)
{
	GameEngine.EventWindowFocusGained();
}

#include "Core/EngineVisitor.h"
#include "Core/Engine.h"

void EngineVisitor::operator()(const sf::Event::Closed&)
{
	GameEngine.EventWindowClose();
}

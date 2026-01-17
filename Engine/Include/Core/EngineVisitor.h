#pragma once

#include <sfml/Window/Event.hpp>

struct EngineVisitor
{
	class Engine& GameEngine;

	void operator()(const sf::Event::Closed&);
	void operator()(const auto&) {}
};
#pragma once

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <tgui/TGUI.hpp>

class GUIManager
{
public:
	GUIManager(sf::RenderWindow& Window);

	void AddWidget(const tgui::Widget::Ptr& Widget);
	void RemoveWidget(const tgui::Widget::Ptr& Widget);

private:
	sf::RenderWindow& Window;
	tgui::Gui GUI;

private:
	friend class Engine;
	void HandleEvents(const sf::Event&);
	void Render();

private:
	void HandleEvent(const sf::Event::Resized&);
	void HandleEvent(const sf::Event::JoystickButtonPressed&);
	void HandleEvent(const sf::Event::JoystickButtonReleased&);
	void HandleEvent(const auto&) {}
};

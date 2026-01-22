#include "SystemManagers/GUIManager.h"
#include "Core/InputBindings.h"

GUIManager::GUIManager(sf::RenderWindow& Window)
	: Window{Window}
	, GUI{Window}
{
	tgui::Theme::setDefault("Content/Themes/Dark.txt");
	GUI.setTabKeyUsageEnabled(true);
	GUI.setFont("Content/Assets/Fonts/base.ttf");
}

void GUIManager::HandleEvents(const sf::Event& Event)
{
	Event.visit([this](const auto& Type)
		{
			this->HandleEvent(Type);
			this->GUI.handleEvent(Type);
		});
}

void GUIManager::Render()
{
	GUI.draw();
}

void GUIManager::AddWidget(const tgui::Widget::Ptr& Widget)
{
	GUI.add(Widget);
}

void GUIManager::RemoveWidget(const tgui::Widget::Ptr& Widget)
{
	GUI.remove(Widget);
}

void GUIManager::HandleEvent(const sf::Event::Resized& Resize)
{
	const sf::Vector2f DefualtSize = Window.getDefaultView().getSize();
	const sf::Vector2f Scale = sf::Vector2f(Resize.size).componentWiseDiv(DefualtSize);
	GUI.setRelativeView({ 0, 0, 1 / Scale.x, 1 / Scale.y });
}

void GUIManager::HandleEvent(const sf::Event::JoystickButtonPressed& Gamepad)
{
	if (Input::HardwareToLogic(Gamepad.button, Gamepad.joystickId) == GamepadButton::South)
	{
		GUI.handleEvent(sf::Event::MouseButtonPressed{ {}, sf::Mouse::getPosition(Window) });
	}
}

void GUIManager::HandleEvent(const sf::Event::JoystickButtonReleased& Gamepad)
{
	if (Input::HardwareToLogic(Gamepad.button, Gamepad.joystickId) == GamepadButton::South)
	{
		GUI.handleEvent(sf::Event::MouseButtonReleased{ {}, sf::Mouse::getPosition(Window) });
	}
}
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

void EngineVisitor::operator()(const sf::Event::JoystickConnected& Gamepad)
{
	GameEngine.EventGamepadConnected(Gamepad.joystickId);
}

void EngineVisitor::operator()(const sf::Event::JoystickDisconnected& Gamepad)
{
	GameEngine.EventGamepadDisconnected(Gamepad.joystickId);
}

void EngineVisitor::operator()(const sf::Event::KeyPressed& Key)
{
	if (Key.control && Key.shift && Key.scancode == sf::Keyboard::Scan::S)
	{
		GameEngine.EventScreenshot();
	}
	else if (Key.scancode == sf::Keyboard::Scan::R)
	{
		GameEngine.EventRestartScene();
	}
	else if (Key.scancode == sf::Keyboard::Scan::M)
	{
		GameEngine.EventReturnToMainMenu();
	}
}

void EngineVisitor::operator()(const sf::Event::JoystickButtonPressed& Gamepad)
{
	if (const auto Button = Input::HardwareToLogic(Gamepad.button, Gamepad.joystickId))
	{
		if (*Button == GamepadButton::Select)
		{
			GameEngine.EventReturnToMainMenu();
		}
	}
}

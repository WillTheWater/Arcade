#pragma once

#include <SFML/Window.hpp>

#include <optional>
#include <variant>

enum class GamepadButton
{
	West, South, East, North, // Face Buttons
	L1, R1, L2, R2, // Sholder Buttons
	Select, Start,
	L3, R3 // Joy-Stick Click
};

namespace Input
{
	struct Keyboard
	{
		sf::Keyboard::Scan Key;
	};

	struct Mouse
	{
		sf::Mouse::Button Button;
	};

	struct Gamepad
	{
		GamepadButton Button;
		int ID = 0;
	};

	struct Axis
	{
		sf::Joystick::Axis Axis;
		float Threshold = 0.2f;
		int ID = 0;
	};

	using Binding = std::variant<Keyboard, Mouse, Gamepad, Axis>;

	std::optional<int> LogicToHardware(GamepadButton InButton, int ButtonID);
	std::optional<GamepadButton> HardwareToLogic(int InButton, int ButtonID);
}
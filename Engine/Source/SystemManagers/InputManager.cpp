#include <algorithm>

#include "SystemManagers/InputManager.h"

void InputManager::Bind(int InputAction, const Input::Binding& Binding)
{
	Bindings.emplace(InputAction, Binding);
}

void InputManager::Clear()
{
	Bindings.clear();
}

bool InputManager::Pressed(int InputAction) const
{
	const auto [First, Last] = Bindings.equal_range(InputAction);

	return std::any_of(First, Last, [this](const auto& Entry)
		{
			return std::visit([this](const auto& Binding)
				{
					return this->Pressed(Binding);
				}, Entry.second);
		});
}

bool InputManager::Pressed(const Input::Keyboard& Binding) const
{
	return sf::Keyboard::isKeyPressed(Binding.Key);
}

bool InputManager::Pressed(const Input::Mouse& Binding) const
{
	return sf::Mouse::isButtonPressed(Binding.Button);
}

bool InputManager::Pressed(const Input::Gamepad& Binding) const
{
	const auto Button = Input::LogicToHardware(Binding.Button, Binding.ID);
	return Button ? sf::Joystick::isButtonPressed(Binding.ID, *Button) : false;
}

bool InputManager::Pressed(const Input::Axis& Binding) const
{
	const float Axis = sf::Joystick::getAxisPosition(Binding.ID, Binding.Axis) / 100;
	return (Binding.Threshold > 0) ? Axis > Binding.Threshold : Axis < Binding.Threshold;
}
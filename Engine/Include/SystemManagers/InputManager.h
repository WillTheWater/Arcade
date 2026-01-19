#pragma once

#include <unordered_map>

#include "Core/InputBindings.h"

class InputManager
{
public:
	void Bind(int InputAction, const Input::Binding& Binding);
	void Clear();
	bool Pressed(int InputAction) const;

private:
	std::unordered_multimap<int, Input::Binding> Bindings;

private:
	bool Pressed(const Input::Keyboard& Binding) const;
	bool Pressed(const Input::Mouse& Binding) const;
	bool Pressed(const Input::Gamepad& Binding) const;
	bool Pressed(const Input::Axis& Binding) const;
};
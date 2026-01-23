#pragma once

#include <unordered_map>

#include "Core/InputBindings.h"

class InputEventManager
{
public:
	void Bind(int InputAction, const Input::Binding& Binding);
	void Clear();
	bool OnPressed(int InputAction) const;


private:
	std::unordered_multimap<int, Input::Binding> Bindings;

private:
	bool OnPressed(const Input::Keyboard& Binding) const;
	bool OnPressed(const Input::Mouse& Binding) const;
	bool OnPressed(const Input::Gamepad& Binding) const;
	bool OnPressed(const Input::Axis& Binding) const;
};
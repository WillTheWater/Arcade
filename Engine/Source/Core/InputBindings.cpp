#include <array>

#include "Core/InputBindings.h"

namespace MappingContext
{
	const std::array<int, 12> PlayStation =
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
	};

	const std::array<int, 12> Xbox =
	{
		2, 0, 1, 3, 4, 5, -1, -1, 6, 7, 8, 9
	};

	const int MicrosoftVID = 0x045E;

	const auto& GetMappingContext(int MappingID)
	{
		const int VendorID = sf::Joystick::getIdentification(MappingID).vendorId;
		return (VendorID == MicrosoftVID) ? Xbox : PlayStation;
	}
}

std::optional<int> Input::LogicToHardware(GamepadButton InButton, int ButtonID)
{
	const auto& Mapping = MappingContext::GetMappingContext(ButtonID);
	const int Value = Mapping[(int)InButton];
	return (Value != -1) ? std::optional(Value) : std::nullopt;
}

std::optional<GamepadButton> Input::HardwareToLogic(int InButton, int ButtonID)
{
	const auto& Mapping = MappingContext::GetMappingContext(ButtonID);
	for (std::size_t i = 0; i < Mapping.size(); i++)
	{
		if (Mapping[i] == InButton)
		{
			return GamepadButton(i);
		}
	}
	return std::nullopt;
}

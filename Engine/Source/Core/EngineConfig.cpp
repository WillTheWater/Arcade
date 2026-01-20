#include <nlohmann/json.hpp>
#include <fstream>
#include <cassert>

#include "Core/EngineConfig.h"

EngineConfig::EngineConfig()
{
	std::ifstream Config("Content/Config.json");
	assert(Config);
	nlohmann::json json = nlohmann::json::parse(Config);
	WindowTitle = json["WindowTitle"];
	WindowSize = { json["WindowSize"][0], json["WindowSize"][1] };
	DisableSFMLLogging = { json["DisableSFMLLogging"] };
	MaxDeltaTime = sf::seconds(json["MaxDeltaTime"]);
	GlobalVolume = json["GlobalVolume"];
	BackgroundColor = { json["BackgroundColor"][0],	json["BackgroundColor"][1],	json["BackgroundColor"][2] };
	CursorRadius = json["CursorRadius"];
	CursorSpeed = json["CursorSpeed"];
	CursorColor = { json["CursorColor"][0],	json["CursorColor"][1],	json["CursorColor"][2] };
	JoystickDeadZone = json["JoystickDeadZone"];
}

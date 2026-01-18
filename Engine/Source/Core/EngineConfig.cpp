#include <nlohmann/json.hpp>
#include <fstream>
#include <cassert>

#include "Core/EngineConfig.h"

EngineConfig::EngineConfig()
{
	std::ifstream Config("Engine/Config.json");
	assert(Config);
	nlohmann::json json = nlohmann::json::parse(Config);
	WindowTitle = json["WindowTitle"];
	WindowSize = { json["WindowSize"][0], json["WindowSize"][1] };
	DisableSFMLLogging = { json["DisableSFMLLogging"] };
	MaxDeltaTime = sf::seconds(json["MaxDeltaTime"]);
}

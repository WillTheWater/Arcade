#include <nlohmann/json.hpp>
#include <fstream>
#include <cassert>

#include "Core/EngineConfig.h"

EngineConfig::EngineConfig()
{
	std::ifstream Config("Engine/Config.json");
	assert(Config);
	nlohmann::json Json = nlohmann::json::parse(Config);
	WindowTitle = Json["WindowTitle"];
	WindowSize = { Json["WindowSize"][0], Json["WindowSize"][1] };
	DisableSFMLLogging = { Json["DisableSFMLLogging"] };
}

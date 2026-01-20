#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Core/Managers.h"
#include "Scenes/Scene.h"

namespace SceneFactory
{
	using Scenes = std::unordered_map<std::string, std::unique_ptr<Scene>>;

	Scenes CreateScenes(Managers& Manager);
}
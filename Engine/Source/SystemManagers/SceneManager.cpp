#include "SystemManagers/SceneManager.h"
#include "Utilities/Log.h"

std::optional<std::string> SceneManager::GetNextScene()
{
	if (NextScene)
	{
		if (CurrentScene == *NextScene)
		{
			LOG("Restarting Scene: {}", CurrentScene);
		}
		else
		{
			LOG("Loading Scene: {}", *NextScene);
		}

		CurrentScene = *NextScene;
		return std::exchange(NextScene, std::nullopt);
	}
	return std::nullopt;
}

void SceneManager::ChangeScene(std::string_view Name)
{
	if (!NextScene)
	{
		NextScene = Name;
	}
}

void SceneManager::ReloadScene()
{
	ChangeScene(CurrentScene);
}
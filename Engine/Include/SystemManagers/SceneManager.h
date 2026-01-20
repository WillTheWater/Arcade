#pragma once

#include <optional>
#include <string>
#include <string_view>

class SceneManager
{
public:
	void ChangeScene(std::string_view Name);
	void ReloadScene();

private:
	std::string CurrentScene;
	std::optional<std::string> NextScene;

private:
	friend class Engine;
	std::optional<std::string> GetNextScene();
};
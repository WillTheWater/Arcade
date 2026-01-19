#include "SystemManagers/AssetManager.h"
#include "Utilities/Log.h"

sf::Texture* AssetManager::GetTexture(const std::string& FileName)
{
	if (!Textures.contains(FileName))
	{
		if (!Textures[FileName].loadFromFile("Content/Assets/Textures/" + FileName))
		{
			LOG("Failed to load texture: {}", FileName);
			return nullptr;
		}
	}
	return &Textures.at(FileName);
}

sf::Font* AssetManager::GetFont(const std::string& FileName)
{
	if (!Fonts.contains(FileName))
	{
		if (!Fonts[FileName].openFromFile("Content/Assets/Fonts/" + FileName))
		{
			LOG("Failed to load font: {}", FileName);
			return nullptr;
		}
	}
	return &Fonts.at(FileName);
}

sf::SoundBuffer* AssetManager::GetSound(const std::string& FileName)
{
	if (!Sounds.contains(FileName))
	{
		if (!Sounds[FileName].loadFromFile("Content/Assets/Sounds/" + FileName))
		{
			LOG("Failed to load sound: {}", FileName);
			return nullptr;
		}
	}
	return &Sounds.at(FileName);
}

std::optional<sf::Music> AssetManager::GetMusic(const std::string& FileName) const
{
	sf::Music MusicFile;
	if (!MusicFile.openFromFile("Content/Assets/Music/" + FileName))
	{
		LOG("Failed to load music: {}", FileName);
		return std::nullopt;
	}
	return MusicFile;
}

#pragma once

#include <optional>
#include <unordered_map>
#include <string>

#include <sfml/Graphics/Texture.hpp>
#include <sfml/Graphics/Font.hpp>
#include <sfml/Audio/SoundBuffer.hpp>
#include <sfml/Audio/Music.hpp>

class AssetManager
{
public:
	sf::Texture* GetTexture(const std::string& FileName);
	sf::Font* GetFont(const std::string& FileName);
	sf::SoundBuffer* GetSound(const std::string& FileName);
	std::optional<sf::Music> GetMusic(const std::string& FileName) const;

private:
	std::unordered_map<std::string, sf::Texture> Textures;
	std::unordered_map<std::string, sf::Font> Fonts;
	std::unordered_map<std::string, sf::SoundBuffer> Sounds;
};
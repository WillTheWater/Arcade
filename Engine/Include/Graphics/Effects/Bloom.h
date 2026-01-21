#pragma once

#include <sfml/Graphics/Shader.hpp>
#include <sfml/Graphics/RenderTexture.hpp>
#include <array>

#include "Graphics/PostProcess.h"

class Bloom : public PostProccess
{
public:
	Bloom();

	void Apply(const sf::Texture& Input, sf::RenderTarget& Output) override;

private:
	sf::Shader DownSampleShader;
	sf::Shader BlurShader;
	sf::Shader AdditiveShader;
	std::array<sf::RenderTexture, 2> Textures;

private:
	void Render(const sf::Shader& Shader, sf::RenderTexture& Output);
};
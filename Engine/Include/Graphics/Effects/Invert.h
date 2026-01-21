#pragma once

#include <sfml/Graphics/Shader.hpp>

#include "Graphics/PostProcess.h"

class Invert : public PostProccess
{
public:
	Invert();

	void Apply(const sf::Texture& Input, sf::RenderTarget& Output) override;

private:
	sf::Shader Shader;
};
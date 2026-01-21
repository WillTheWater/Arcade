#pragma once

#include <sfml/Graphics/Shader.hpp>
#include <sfml/System/Clock.hpp>

#include "Graphics/PostProcess.h"

class Glitch : public PostProccess
{
public:
	Glitch();

	void Apply(const sf::Texture& Input, sf::RenderTarget& Output) override;

private:
	sf::Shader Shader;
	sf::Clock Clock;
};
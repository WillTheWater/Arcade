#pragma once

#include <sfml/Graphics/Shader.hpp>
#include <sfml/System/Clock.hpp>

#include "Graphics/PostProcess.h"

class CRT : public PostProccess
{
public:
	CRT();

	void Apply(const sf::Texture& Input, sf::RenderTarget& Output) override;

private:
	sf::Shader Shader;
	sf::Clock Clock;
};
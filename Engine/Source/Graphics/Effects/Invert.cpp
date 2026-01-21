#include <sfml/Graphics/Sprite.hpp>

#include "Graphics/Effects/Invert.h"
#include "Utilities/Assert.h"

Invert::Invert()
{
	ASSERT(Shader.loadFromFile("Content/Shaders/Invert.frag", sf::Shader::Type::Fragment));
}

void Invert::Apply(const sf::Texture& Input, sf::RenderTarget& Output)
{
	Shader.setUniform("sourceTexture", sf::Shader::CurrentTexture);
	Output.draw(sf::Sprite(Input), &Shader);
}

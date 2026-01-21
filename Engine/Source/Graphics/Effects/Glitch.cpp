#include <sfml/Graphics/Sprite.hpp>
#include <sfml/System/Time.hpp>

#include "Graphics/Effects/Glitch.h"
#include "Utilities/Assert.h"

Glitch::Glitch()
{
	ASSERT(Shader.loadFromFile("Content/Shaders/Glitch.frag", sf::Shader::Type::Fragment));
}

void Glitch::Apply(const sf::Texture& Input, sf::RenderTarget& Output)
{
	Shader.setUniform("sourceTexture", sf::Shader::CurrentTexture);
	Shader.setUniform("time", Clock.getElapsedTime().asSeconds());
	Output.draw(sf::Sprite(Input), &Shader);
}

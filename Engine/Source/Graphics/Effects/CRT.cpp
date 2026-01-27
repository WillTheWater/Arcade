#include <sfml/Graphics/Sprite.hpp>
#include <sfml/System/Time.hpp>

#include "Graphics/Effects/CRT.h"
#include "Utilities/Assert.h"

CRT::CRT()
{
	ASSERT(Shader.loadFromFile("Content/Shaders/Monitor.frag", sf::Shader::Type::Fragment));
}

void CRT::Apply(const sf::Texture& Input, sf::RenderTarget& Output)
{
	Shader.setUniform("sourceTexture", sf::Shader::CurrentTexture);
	Shader.setUniform("resolution", sf::Vector2f(Output.getSize()));
	Shader.setUniform("time", Clock.getElapsedTime().asSeconds());
	Output.draw(sf::Sprite(Input), &Shader);
}
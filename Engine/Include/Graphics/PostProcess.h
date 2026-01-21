#pragma once

#include <sfml/Graphics/RenderTarget.hpp>
#include <sfml/Graphics/Texture.hpp>

class PostProccess
{
public:
	virtual ~PostProccess() = default;

	virtual void Apply(const sf::Texture& Input, sf::RenderTarget& Output) = 0;
};
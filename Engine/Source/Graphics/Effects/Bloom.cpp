#include <sfml/Graphics/Sprite.hpp>
#include <string>

#include "Graphics/Effects/Bloom.h"
#include "Core/EngineConfig.h"
#include "Utilities/Assert.h"

Bloom::Bloom()
{
	const std::string ShaderPath = "Content/Shaders/";
	const std::string VertShader = ShaderPath + "Default.vert";

	ASSERT(DownSampleShader.loadFromFile(VertShader, ShaderPath + "Downsample.frag"));
	ASSERT(BlurShader.loadFromFile(VertShader, ShaderPath + "Blur.frag"));
	ASSERT(AdditiveShader.loadFromFile(VertShader, ShaderPath + "Additive.frag"));

	for (auto& T : Textures)
	{
		ASSERT(T.resize(sf::Vector2u(EConfig.WindowSize / 2.f)));
	}
}

void Bloom::Apply(const sf::Texture& Input, sf::RenderTarget& Output)
{
    // 1. Downsample to reduce input resolution before blur
    DownSampleShader.setUniform("sourceTexture", Input);
    DownSampleShader.setUniform("texelSize", sf::Vector2f(1.f / Input.getSize().x, 1.f / Input.getSize().y));
    Render(DownSampleShader, Textures[0]);

    // 2. Vertical gaussian blur to spread brightness downward
    BlurShader.setUniform("sourceTexture", Textures[0].getTexture());
    BlurShader.setUniform("texelSize", sf::Vector2f(0, 1.f / Textures[0].getSize().y));
    Render(BlurShader, Textures[1]);

    // 3. Horizontal gaussian blur to spread brightness sideways
    BlurShader.setUniform("sourceTexture", Textures[1].getTexture());
    BlurShader.setUniform("texelSize", sf::Vector2f(1.f / Textures[1].getSize().x, 0));
    Render(BlurShader, Textures[0]);

    // 4. Add bloom on top of original image
    AdditiveShader.setUniform("sourceTexture", sf::Shader::CurrentTexture);
    AdditiveShader.setUniform("bloomTexture", Textures[0].getTexture());
    Output.draw(sf::Sprite(Input), &AdditiveShader);
}

void Bloom::Render(const sf::Shader& Shader, sf::RenderTexture& Output)
{
    const sf::Vector2f OutSize(Output.getSize());

    const sf::Vertex Quad[] =
    {
        {{0,0}, sf::Color::White, {0,1}},
        {{OutSize.x,0}, sf::Color::White, {1,1}},
        {{0,OutSize.y}, sf::Color::White, {0,1}},
        {{OutSize}, sf::Color::White, {1,0}}
    };

    sf::RenderStates States(&Shader);
    States.blendMode = sf::BlendNone;
    Output.draw(Quad, 4, sf::PrimitiveType::TriangleStrip, States);
    Output.display();
}

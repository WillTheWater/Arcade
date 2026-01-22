#include <utility>

#include "SystemManagers/RenderManager.h"
#include "Core/EngineConfig.h"
#include "Utilities/Log.h"
#include "Utilities/Assert.h"

//#include "Graphics/Effects/Invert.h"
//#include "Graphics/Effects/Glitch.h"
//#include "Graphics/Effects/Bloom.h"
//#include "Graphics/Effects/CRT.h"

RenderManager::RenderManager()
	: RenderTarget{ sf::Vector2u(EConfig.WindowSize) }
	, BackgroundTexture{"Content/Assets/Textures/gamewindow.png"}
	, Background{ EConfig.WindowSize }
{
	RenderTarget.setSmooth(true);
	Background.setTexture(&BackgroundTexture);
	Background.setFillColor(EConfig.BackgroundColor);

	if (sf::Shader::isAvailable())
	{
		ASSERT(PPETarget.resize(sf::Vector2u(EConfig.WindowSize)));

		//PostProcessEffects.emplace_back(std::make_unique<CRT>());
		//PostProcessEffects.emplace_back(std::make_unique<Bloom>());
	}
}

void RenderManager::Draw(const sf::Drawable& Drawable)
{
	RenderTarget.draw(Drawable);
}

void RenderManager::Draw(std::span<sf::Vertex> Vertices, sf::PrimitiveType Type)
{
	RenderTarget.draw(Vertices.data(), Vertices.size(), Type);
}

void RenderManager::SetView(const sf::View& View)
{
	RenderTarget.setView(View);
}

void RenderManager::ResetView()
{
	SetView(RenderTarget.getDefaultView());
}

void RenderManager::BeginDrawing()
{
	RenderTarget.clear();
	Draw(Background);
}

const sf::Texture& RenderManager::FinishDrawing()
{
	RenderTarget.display();

	sf::RenderTexture* Input = &RenderTarget;
	sf::RenderTexture* Output = &PPETarget;

	for (auto& PPE : PostProcessEffects)
	{
		Output->clear();
		PPE->Apply(Input->getTexture(), *Output);
		Output->display();
		std::swap(Input, Output);
	}

	return Input->getTexture();
}

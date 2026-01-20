#include "SystemManagers/RenderManager.h"
#include "Core/EngineConfig.h"
#include "Utilities/Log.h"

RenderManager::RenderManager()
	: RenderTarget{sf::Vector2u(EConfig.WindowSize)}
	, BackgroundTexture{"Content/Assets/Textures/Background.png"}
	, Background{EConfig.WindowSize}
{
	RenderTarget.setSmooth(true);
	Background.setTexture(&BackgroundTexture);
	Background.setFillColor(EConfig.BackgroundColor);
}

void RenderManager::Draw(const sf::Drawable& Drawable)
{
	RenderTarget.draw(Drawable);
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
	return RenderTarget.getTexture();
}

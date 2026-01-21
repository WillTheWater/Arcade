#pragma once 

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "Graphics/PostProcess.h"

class RenderManager
{
public:
	RenderManager();

	void Draw(const sf::Drawable& Drawable);
	void SetView(const sf::View& View);
	void ResetView();

private:
	sf::RenderTexture RenderTarget;
	sf::Texture BackgroundTexture;
	sf::RectangleShape Background;

	std::vector<std::unique_ptr<PostProccess>> PostProcessEffects;
	sf::RenderTexture PPETarget;

private:
	friend class Engine;
	void BeginDrawing();
	const sf::Texture& FinishDrawing();
};
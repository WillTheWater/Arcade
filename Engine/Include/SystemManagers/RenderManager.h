#pragma once 

#include <SFML/Graphics.hpp>

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

private:
	friend class Engine;
	void BeginDrawing();
	const sf::Texture& FinishDrawing();
};
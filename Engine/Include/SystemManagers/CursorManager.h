#pragma once

#include <sfml/Graphics/CircleShape.hpp>
#include <sfml/Graphics/RectangleShape.hpp>
#include <sfml/Graphics/Texture.hpp>
#include <sfml/Graphics/RenderWindow.hpp>
#include <sfml/Graphics/View.hpp>

class CursorManager
{
public:
	CursorManager(sf::RenderWindow& Window);

	void SetCursorSpeed(float Speed);
	float GetCursorSpeed() const;

	void SetVisibility(bool Visiblity);
	bool IsVisible() const;

	void SetPosition(sf::Vector2f Position);
	sf::Vector2f GetPosition() const;
	sf::Vector2f GetPosition(const sf::View& View) const;

private:
	sf::RenderWindow& Window;
	//sf::CircleShape CursorShape;
	sf::Texture CursorTexture;
	sf::RectangleShape Cursorbase;
	float CursorSpeed;
	bool Visible;

private:
	friend class Engine;
	void Update(float DeltaTime);
	void Render() const;
};

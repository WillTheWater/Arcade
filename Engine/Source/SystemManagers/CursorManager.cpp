#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Mouse.hpp>
#include <algorithm>

#include "SystemManagers/CursorManager.h"
#include "Core/EngineConfig.h"

CursorManager::CursorManager(sf::RenderWindow& Window)
	: Window{ Window }
	, Cursorbase{EConfig.CursorSize}
	, CursorSpeed{ EConfig.CursorSpeed }
	, Visible{ true }
{
	CursorTexture.loadFromFile("Content/Assets/Textures/cursor.png");
	Cursorbase.setTexture(&CursorTexture);
	Cursorbase.setPosition(EConfig.WindowSize / 2.f);
}

void CursorManager::Update(float DeltaTime)
{
	const sf::Vector2f JoystickDirection(
		sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) / 100,
		sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) / 100);

	if (JoystickDirection.length() > EConfig.JoystickDeadZone)
	{
		Cursorbase.move(JoystickDirection * CursorSpeed * DeltaTime);
		Cursorbase.setPosition(
			{
				std::clamp(Cursorbase.getPosition().x, 0.f,
				EConfig.WindowSize.x - 1),
				std::clamp(Cursorbase.getPosition().y, 0.f,
				EConfig.WindowSize.y - 1)
			});
		SetPosition(Cursorbase.getPosition());
	}
	else
	{
		Cursorbase.setPosition(GetPosition());
	}
}

void CursorManager::Render() const
{
	if (Visible)
	{
		Window.draw(Cursorbase);
	}
}

void CursorManager::SetCursorSpeed(float Speed)
{
	CursorSpeed = Speed;
}

float CursorManager::GetCursorSpeed() const
{
	return CursorSpeed;
}

void CursorManager::SetVisibility(bool Visiblity)
{
	Visible = Visiblity;
}

bool CursorManager::IsVisible() const
{
	return Visible;
}

void CursorManager::SetPosition(sf::Vector2f Position)
{
	sf::Mouse::setPosition(Window.mapCoordsToPixel(Position, Window.getDefaultView()), Window);
}

sf::Vector2f CursorManager::GetPosition() const
{
	return GetPosition(Window.getDefaultView());
}

sf::Vector2f CursorManager::GetPosition(const sf::View& View) const
{
	return Window.mapPixelToCoords(sf::Mouse::getPosition(Window), View);
}

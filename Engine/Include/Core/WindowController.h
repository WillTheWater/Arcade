#pragma once

#include <SFML/Graphics/Color.hpp>
#include <sfml/Window/Event.hpp>
#include <sfml/System/Vector2.hpp>
#include <vector>

#include "Core/Managers.h"
#include "Scenes/SceneUtilities.h"

struct Button
{
	sf::RectangleShape Shape;
    int ButtonIndex;
};

const sf::Vector2f BUTTON_SIZE{ 35.f, 35.f };
const sf::Vector2f BUTTON_SPACING{ 10,0 };
const float BUTTON_DISTANCE_FROM_SIDE{ 40.f };
const sf::Color BUTTON_COLOR(sf::Color::White);
const sf::Color BUTTON_HOVERED_COLOR({ 180,180,180 });
const std::array<std::string, 3> FILE_PATH = { "close.png" , "max.png" , "min.png" };


class WindowControls
{
public:
	WindowControls(Managers& Mgr);

    void OnEvent(const sf::Event&);
    void Update();
    void Render() const;

private:
    Managers& MGR;
    std::vector<Button> Buttons;

private:
    void InitButtons();
    void InitButton(Button& ButtonToInit, sf::FloatRect Bounds, std::string PATH, int INDEX);

    void HandleEvent(const sf::Event::MouseButtonPressed&);
    void HandleEvent(const sf::Event::JoystickButtonPressed&);
    void HandleEvent(const auto&) {}

    void UpdateButtons();
    void UpdateButton(Button& ButtonToUpdate);
    bool IsButtonHovered(const Button& ButtonHovered) const;
};
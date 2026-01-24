#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>

#include <vector>
#include <functional>
#include <array>

#include "Core/Managers.h"
#include "Scenes/SceneUtilities.h"

enum class WindowAction
{
    Close,
    Maximize,
    Minimize
};

/*
    Button state is now explicit.
    This mirrors the reference implementation behavior.
*/
struct Button
{
    sf::RectangleShape Shape;
    std::function<void()> OnClick;

    bool bPressed = false;   // True only while mouse is pressed down on THIS button
};

using ActionCallback = std::function<void(WindowAction)>;

constexpr sf::Vector2f BUTTON_SIZE{ 35.f, 35.f };
constexpr sf::Vector2f BUTTON_SPACING{ 10.f, 0.f };
constexpr float BUTTON_DISTANCE_FROM_SIDE{ 40.f };

const sf::Color BUTTON_COLOR(sf::Color::White);
const sf::Color BUTTON_HOVERED_COLOR({ 180,180,180 });
const sf::Color BUTTON_PRESSED_COLOR({ 100,100,100 });

const std::array<std::string, 3> FILE_PATH =
{
    "close.png",
    "max.png",
    "min.png"
};

class WindowControls
{
public:
    explicit WindowControls(Managers& Mgr, sf::RenderWindow& window);

    void OnEvent(const sf::Event& event);
    void Update();
    void Render() const;

    void SetActionCallback(ActionCallback cb);

private:
    Managers& MGR;
    std::vector<Button> Buttons;
    ActionCallback OnAction;

    // --- Maximize drag support ---
    sf::RenderWindow& GameWindow;
    static constexpr float MAX_DRAG_THRESHOLD = 6.f;

    bool bDraggingWindow = false;
    bool bMaximizeDragCandidate = false;

    sf::Vector2i DragStartMousePos;
    sf::Vector2i DragStartWindowPos;

private:
    void InitButtons();
    void InitButton(Button& button, sf::FloatRect bounds, const std::string& path);

    // --- Event handling ---
    void HandleEvent(const sf::Event::MouseMoved&);
    void HandleEvent(const sf::Event::MouseButtonPressed&);
    void HandleEvent(const sf::Event::MouseButtonReleased&);
    void HandleEvent(const sf::Event::JoystickButtonPressed&);
    void HandleEvent(const sf::Event::JoystickButtonReleased&);
    void HandleEvent(const auto&) {}

    // --- Button logic ---
    void UpdateButtons();
    void UpdateButton(Button& button);
    bool IsButtonHovered(const Button& button) const;
};

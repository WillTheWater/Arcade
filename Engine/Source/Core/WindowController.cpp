#include "Core/WindowController.h"

WindowControls::WindowControls(Managers& Mgr)
    : MGR {Mgr}
{
	InitButtons();
}

void WindowControls::InitButtons()
{
    Buttons.reserve(3);

    sf::Vector2f start
    { 
        EConfig.WindowSize.x - BUTTON_DISTANCE_FROM_SIDE, 
        BUTTON_DISTANCE_FROM_SIDE 
    };

    sf::Vector2f step { BUTTON_SPACING + BUTTON_SIZE };

    for (int i = 0; i < 3; i++)
    {
        sf::Vector2f position{ start.x + step.x * -i, start.y };
        auto& button = Buttons.emplace_back();
        InitButton(button, { position, BUTTON_SIZE }, FILE_PATH[i], i);
    }
}

void WindowControls::InitButton(Button& ButtonToInit, sf::FloatRect Bounds, std::string PATH, int INDEX)
{
    ButtonToInit.ButtonIndex = INDEX;
    ButtonToInit.Shape.setTexture(MGR.Assets.GetTexture(PATH));
    ButtonToInit.Shape.setFillColor(BUTTON_COLOR);
    ButtonToInit.Shape.setSize(Bounds.size);
    ButtonToInit.Shape.setOrigin(ButtonToInit.Shape.getGeometricCenter());
    ButtonToInit.Shape.setPosition(Bounds.position);
}

void WindowControls::HandleEvent(const sf::Event::MouseButtonPressed& mouse)
{
    if (mouse.button == sf::Mouse::Button::Left)
    {
        // TODO
        LOG("MOUSE PRESSED");
    }
}

void WindowControls::HandleEvent(const sf::Event::JoystickButtonPressed& joystick)
{
    if (auto button = Input::HardwareToLogic(joystick.button, joystick.joystickId))
    {
        if (*button == GamepadButton::South)
        {
            // TODO
            LOG("GAMEPAD PRESSED");
        }
    }
}

void WindowControls::UpdateButtons()
{
    for (auto& button : Buttons)
    {
        UpdateButton(button);
    }
}

void WindowControls::UpdateButton(Button& ButtonToUpdate)
{
    sf::Color color = IsButtonHovered(ButtonToUpdate) ? BUTTON_HOVERED_COLOR : BUTTON_COLOR;

    if (ButtonToUpdate.Shape.getFillColor() != color)
    {
        ButtonToUpdate.Shape.setFillColor(color);
    }
}

bool WindowControls::IsButtonHovered(const Button& ButtonHovered) const
{
    return Contains(ButtonHovered.Shape, MGR.Cursor.GetPosition());
}

void WindowControls::OnEvent(const sf::Event& event)
{
    event.visit([this](const auto& type) { this->HandleEvent(type); });
}

void WindowControls::Update()
{
    UpdateButtons();
}

void WindowControls::Render() const
{
    for (const auto& button : Buttons)
    {
        MGR.Renderer.Draw(button.Shape);
    }
}
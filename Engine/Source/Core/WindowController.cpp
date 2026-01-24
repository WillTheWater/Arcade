#include "Core/WindowController.h"

WindowControls::WindowControls(Managers& Mgr, sf::RenderWindow& window)
    : MGR{ Mgr }
    , GameWindow{window}
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

    sf::Vector2f step{ BUTTON_SPACING + BUTTON_SIZE };

    for (int i = 0; i < 3; i++)
    {
        sf::Vector2f position{ start.x - step.x * i, start.y };
        auto& button = Buttons.emplace_back();
        InitButton(button, { position, BUTTON_SIZE }, FILE_PATH[i]);
    }

    Buttons[0].OnClick = [this] { if (OnAction) OnAction(WindowAction::Close); };
    Buttons[1].OnClick = [this] { if (OnAction) OnAction(WindowAction::Maximize); };
    Buttons[2].OnClick = [this] { if (OnAction) OnAction(WindowAction::Minimize); };
}

void WindowControls::InitButton(Button& button, sf::FloatRect bounds, const std::string& path)
{
    button.Shape.setTexture(MGR.Assets.GetTexture(path));
    button.Shape.setFillColor(BUTTON_COLOR);
    button.Shape.setSize(bounds.size);
    button.Shape.setOrigin(button.Shape.getGeometricCenter());
    button.Shape.setPosition(bounds.position);
}

/*
    Mouse moved:
    - Only affects visuals
    - Pressed buttons do NOT change hover state
*/
void WindowControls::HandleEvent(const sf::Event::MouseMoved&)
{
    // Handle maximize-drag detection
    if (bMaximizeDragCandidate && !bDraggingWindow)
    {
        sf::Vector2i currentMousePos = sf::Mouse::getPosition();
        sf::Vector2i delta = currentMousePos - DragStartMousePos;

        float distance =
            std::sqrt(static_cast<float>(delta.x * delta.x + delta.y * delta.y));

        if (distance >= MAX_DRAG_THRESHOLD)
        {
            bDraggingWindow = true;

            // Cancel maximize click
            Buttons[1].bPressed = false;
        }
    }

    // Perform window dragging
    if (bDraggingWindow)
    {
        sf::Vector2i currentMousePos = sf::Mouse::getPosition();
        GameWindow.setPosition(
            DragStartWindowPos + (currentMousePos - DragStartMousePos)
        );
    }

    UpdateButtons();
}


/*
    Mouse pressed:
    - Only registers if mouse is inside button
    - Sets pressed state but does NOT trigger click
*/
void WindowControls::HandleEvent(const sf::Event::MouseButtonPressed& mouse)
{
    if (mouse.button != sf::Mouse::Button::Left)
        return;

    for (size_t i = 0; i < Buttons.size(); ++i)
    {
        auto& button = Buttons[i];

        if (!IsButtonHovered(button))
            continue;

        button.bPressed = true;
        button.Shape.setFillColor(BUTTON_PRESSED_COLOR);

        // Maximize button = index 1
        if (i == 1)
        {
            bMaximizeDragCandidate = true;
            DragStartMousePos = sf::Mouse::getPosition();
            DragStartWindowPos = GameWindow.getPosition();
        }

        return;
    }
}

/*
    Mouse released:
    - Click fires ONLY if:
        - Button was pressed
        - Mouse is still inside
*/
void WindowControls::HandleEvent(const sf::Event::MouseButtonReleased& mouse)
{
    if (mouse.button != sf::Mouse::Button::Left)
        return;

    // End drag if it was active
    if (bDraggingWindow)
    {
        bDraggingWindow = false;
        bMaximizeDragCandidate = false;
        return; // IMPORTANT: never trigger maximize after drag
    }

    for (auto& button : Buttons)
    {
        if (!button.bPressed)
            continue;

        bool inside = IsButtonHovered(button);

        if (inside && button.OnClick)
            button.OnClick();

        button.bPressed = false;
        bMaximizeDragCandidate = false;
        return;
    }

    bMaximizeDragCandidate = false;
}


/*
    Joystick support mirrors mouse logic:
    - Uses hover + press semantics
*/
void WindowControls::HandleEvent(const sf::Event::JoystickButtonPressed& joystick)
{
    if (auto button = Input::HardwareToLogic(joystick.button, joystick.joystickId))
    {
        if (*button != GamepadButton::South)
            return;

        for (auto& b : Buttons)
        {
            if (!IsButtonHovered(b))
                continue;

            b.bPressed = true;
            return;
        }
    }
}

void WindowControls::HandleEvent(const sf::Event::JoystickButtonReleased& joystick)
{
    if (auto button = Input::HardwareToLogic(joystick.button, joystick.joystickId))
    {
        if (*button != GamepadButton::South)
            return;

        for (auto& b : Buttons)
        {
            if (!b.bPressed)
                continue;

            bool inside = IsButtonHovered(b);

            if (inside && b.OnClick)
                b.OnClick();

            b.bPressed = false;
            return;
        }
    }
}


/*
    Updates button visuals based on state:
    Priority:
    1. Pressed
    2. Hovered
    3. Default
*/
void WindowControls::UpdateButtons()
{
    for (auto& button : Buttons)
        UpdateButton(button);
}

void WindowControls::UpdateButton(Button& button)
{
    sf::Color targetColor = BUTTON_COLOR;

    if (button.bPressed)
        targetColor = BUTTON_PRESSED_COLOR;
    else if (IsButtonHovered(button))
        targetColor = BUTTON_HOVERED_COLOR;

    if (button.Shape.getFillColor() != targetColor)
        button.Shape.setFillColor(targetColor);
}

bool WindowControls::IsButtonHovered(const Button& button) const
{
    return Contains(button.Shape, MGR.Cursor.GetPosition());
}

void WindowControls::SetActionCallback(ActionCallback cb)
{
    OnAction = std::move(cb);
}

void WindowControls::OnEvent(const sf::Event& event)
{
    event.visit([this](const auto& type)
        {
            HandleEvent(type);
        });
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

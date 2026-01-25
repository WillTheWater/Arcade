#include "MainMenu.h"

using namespace Menu;

Game::Game(Managers& Mgr) :
    Scene(Mgr),
    Music(*MGR.Assets.GetMusic(MUSIC_FILENAME))
{
    InitButtons();
    InitControls();
    InitMusic();
}

void Game::InitButtons()
{
    int count = (int)BUTTON_NAMES.size();

    int cols = std::min(BUTTONS_PER_ROW, count);
    int rows = (count + cols - 1) / cols;

    sf::Vector2f size(EConfig.WindowSize.x / (cols + BUTTON_PADDING_RATIO),
        90);

    sf::Vector2f spacing(size.x * BUTTON_PADDING_RATIO / (cols + 1),
        20);
    sf::Vector2f TopStart ={ 0,200 };
    sf::Vector2f start = spacing + size / 2.f + TopStart;
    sf::Vector2f step = spacing + size;

    Buttons.reserve(count);

    for (int i = 0; i < count; i++)
    {
        sf::Vector2f position(start.x + step.x * (i % cols),
            start.y + step.y * (i / cols));

        auto& button = Buttons.emplace_back();

        InitButton(button, { position, size }, BUTTON_NAMES[i]);
    }
}

void Game::InitButton(Button& button, sf::FloatRect bounds, std::string name)
{
    button.Shape.setTexture(MGR.Assets.GetTexture(BUTTON_TEXTURE_FILENAME));
    button.Shape.setFillColor(BUTTON_COLOR);
    button.Shape.setSize(bounds.size);
    button.Shape.setOrigin(button.Shape.getGeometricCenter());
    button.Shape.setPosition(bounds.position);

    button.Name = name;
    std::ranges::replace(name, ' ', ' ');

    button.Text.setFillColor(BUTTON_TEXT_COLOR);
    button.Text.setOutlineColor(TEXT_OUTLINE_COLOR);
    button.Text.setOutlineThickness(TEXT_OUTLINE_THICKNESS);
    button.Text.setString(name);
    button.Text.setCharacterSize(BUTTON_TEXT_SIZE);
    button.Text.setOrigin(button.Text.getLocalBounds().getCenter());
    button.Text.setPosition(bounds.position);
}

void Game::InitControls()
{
    Controls.Background.setFillColor(CONTROLS_BACKGROUND_COLOR);
    Controls.Background.setSize(EConfig.WindowSize);

    Controls.Texts.reserve(CONTROLS_TEXTS.size());

    for (const auto& info : CONTROLS_TEXTS)
    {
        auto& text = Controls.Texts.emplace_back(GetDefaultFont(), info, CONTROLS_TEXT_SIZE);

        text.setFillColor(CONTROLS_TEXT_COLOR);
        text.setOrigin(text.getLocalBounds().getCenter());
        text.setPosition(EConfig.WindowSize / 2.f);
    }
}

void Game::InitMusic()
{
    Music.setVolume(0);
    Music.setPitch(MUSIC_PITCH);
}

void Game::Start()
{
    StartControls();
    //StartMusic();
}

void Game::StartControls()
{
    Controls.Current.reset();
}

void Game::StartMusic()
{
    //Music.play();
}

void Game::OnEvent(const sf::Event& event)
{
    event.visit([this](const auto& type) { this->HandleEvent(type); });
}

void Game::HandleEvent(const sf::Event::MouseButtonPressed& mouse)
{
    if (mouse.button == sf::Mouse::Button::Left && !Controls.Current)
    {
        EventSceneLoad();
    }
    else if (mouse.button == sf::Mouse::Button::Right)
    {
        EventControlsShow();
    }
}

void Game::HandleEvent(const sf::Event::JoystickButtonPressed& joystick)
{
    if (auto button = Input::HardwareToLogic(joystick.button, joystick.joystickId))
    {
        if (*button == GamepadButton::South && !Controls.Current)
        {
            EventSceneLoad();
        }
        else if (*button == GamepadButton::West)
        {
            EventControlsShow();
        }
    }
}

void Game::Update()
{
    if (!Controls.Current)
    {
        UpdateButtons();
    }
}

void Game::UpdateButtons()
{
    for (auto& button : Buttons)
    {
        UpdateButton(button);
    }
}

void Game::UpdateButton(Button& button)
{
    sf::Color color = IsButtonHovered(button) ? BUTTON_HOVERED_COLOR : BUTTON_COLOR;

    if (button.Shape.getFillColor() != color)
    {
        button.Shape.setFillColor(color);
    }
}

void Game::EventSceneLoad()
{
    for (const auto& button : Buttons)
    {
        if (IsButtonHovered(button))
        {
            MGR.Scene.ChangeScene(button.Name);
            return;
        }
    }
}

void Game::EventControlsShow()
{
    if (Controls.Current)
    {
        Controls.Current.reset();
        return;
    }

    for (std::size_t i = 0; i < Buttons.size(); i++)
    {
        if (IsButtonHovered(Buttons[i]))
        {
            Controls.Current = (int)i;
            return;
        }
    }
}

bool Game::IsButtonHovered(const Button& button) const
{
    return Contains(button.Shape, MGR.Cursor.GetPosition());
}

void Game::Render() const
{
    if (Controls.Current)
    {
        MGR.Renderer.Draw(Controls.Background);
        MGR.Renderer.Draw(Controls.Texts[*Controls.Current]);
        return;
    }

    for (const auto& button : Buttons)
    {
        MGR.Renderer.Draw(button.Shape);
        MGR.Renderer.Draw(button.Text);
    }
}

void Game::OnPause(bool paused)
{
    if (paused)
    {
        Music.pause();
    }
    else
    {
        Music.play();
    }
}

void Game::OnCleanup()
{
    Music.stop();
}
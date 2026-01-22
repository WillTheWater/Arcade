#pragma once

#include "Types/MainMenuTypes.h"

namespace Menu
{
    class Game : public Scene
    {
    private:
        std::vector<Button> Buttons;
        Controls Controls;
        sf::Music Music;

    public:
        Game(Managers&);

        void Start();
        void OnEvent(const sf::Event&);
        void Update();
        void Render() const;
        void OnPause(bool);
        void OnCleanup();

    private:
        void InitButtons();
        void InitButton(Button& ButtonToInit, sf::FloatRect Bounds, std::string Name);
        void InitControls();
        void InitMusic();

        void StartControls();
        void StartMusic();

        void HandleEvent(const sf::Event::MouseButtonPressed&);
        void HandleEvent(const sf::Event::JoystickButtonPressed&);
        void HandleEvent(const auto&) {}

        void UpdateButtons();
        void UpdateButton(Button& ButtonToUpdate);

        void EventSceneLoad();
        void EventControlsShow();

        bool IsButtonHovered(const Button& ButtonHovered) const;
    };
}
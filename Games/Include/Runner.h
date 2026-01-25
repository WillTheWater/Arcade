#pragma once

#include "Types/RunnerTypes.h"

namespace Dino
{
    class Game : public Scene
    {
    private:
        Dino Dino;
        std::vector<Obstacle> Obstacles;
        sf::RectangleShape Ground;
        Timer ObstacleSpawnTimer;
        Stats Stats;

    public:
        Game(Managers&);

        void Start();
        void Update();
        void OnEvent(const sf::Event& event);
        void Render() const;

    private:
        float ScoreStartTime = 0.f;

    private:
        void InitDino();
        void InitGround();
        void InitStats();
        void BindInputs();

        bool CanJump();
        float GetGroundSnapY(float dinoHeight) const;

        void UpdateDino();
        void UpdateObstacles();

        void EventObstacleSpawn();
        void HandleCollisions();
        void EventDinoHit();
        void EventJump();

        void HandleEvent(const sf::Event::JoystickButtonPressed& Gamepad);
        void HandleEvent(const sf::Event::KeyPressed& Key);
        void HandleEvent(const auto&) {}
    };
}

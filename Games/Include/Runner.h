#pragma once

#include "Types/RunnerTypes.h"

namespace Duck
{
    class Game : public Scene
    {
    private:
        Duck Duck;
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
        void InitDuck();
        void InitGround();
        void InitStats();
        void BindInputs();

        bool CanJump();
        float GetGroundSnapY(float dinoHeight) const;

        void UpdateDuck();
        void UpdateObstacles();

        void EventObstacleSpawn();
        void HandleCollisions();
        void EventDuckHit();
        void EventJump();

        void HandleEvent(const sf::Event::JoystickButtonPressed& Gamepad);
        void HandleEvent(const sf::Event::KeyPressed& Key);
        void HandleEvent(const auto&) {}
    };
}

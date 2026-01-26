#pragma once

#include "Types/RunnerTypes.h"
#include "Animation/SpriteSheets.h"

namespace Duck
{
    class Game : public Scene
    {

    public:
        Game(Managers&);

        void Start();
        void Update();
        void OnEvent(const sf::Event& event);
        void Render() const;
    
    private:
        Duck Duck;
        std::vector<Obstacle> Obstacles;
        sf::RectangleShape Ground;
        Timer ObstacleSpawnTimer;
        Stats Stats;

    private:
        float ScoreStartTime = 0.f;
        Animation DuckAnim;
        AnimationConfig RunAnimConfig;
        AnimationConfig DuckRunAnimConfig;

    private:
        void InitDuck();
        void InitGround();
        void InitStats();
        void BindInputs();
        void InitAnimations();          // ADDED

        bool CanJump();
        float GetGroundSnapY() const;

        void UpdateDuck();
        void UpdateObstacles();
        void UpdateDuckAnimation();     // ADDED

        void EventObstacleSpawn();
        void HandleCollisions();
        void EventDuckHit();
        void EventJump();

        void HandleEvent(const sf::Event::JoystickButtonPressed& Gamepad);
        void HandleEvent(const sf::Event::KeyPressed& Key);
        void HandleEvent(const auto&) {}
    };
}

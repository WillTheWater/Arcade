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
        Timer GameTimer;
        Stats Stats;

        float ScoreStartTime = 0.f;

        // Animation
        Animation DuckAnim;
        AnimationConfig RunAnimConfig;
        AnimationConfig DuckRunAnimConfig;
        AnimState PrevState = AnimState::Run;

        // Difficulty
        float LastObstacleX = 0.f;                   // NEW

    private:
        void InitDuck();
        void InitGround();
        void InitStats();
        void BindInputs();
        void InitAnimations();

        bool CanJump();
        float GetGroundSnapY() const;
        float GetObstacleY(ObstacleType type) const; // NEW

        void UpdateDuck();
        void UpdateObstacles();
        void UpdateDuckAnimation();

        void EventObstacleSpawn();
        void HandleCollisions();
        void EventDuckHit();
        void EventJump();

        void HandleEvent(const sf::Event::JoystickButtonPressed& Gamepad);
        void HandleEvent(const sf::Event::KeyPressed& Key);
        void HandleEvent(const auto&) {}
    };
}

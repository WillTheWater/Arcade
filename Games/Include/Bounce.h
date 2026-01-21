#pragma once

#include "Types/BounceTypes.h"

namespace Bounce
{
    class Game : public Scene
    {
    private:
        Paddle Paddle;
        std::vector<Ball> Balls;
        Timer BallSpawnCooldown;
        Stats Stats;
        sf::RectangleShape Background;
        sf::Sound BounceSound;
        sf::Music Music;

    public:
        Game(Managers&);

        void Start();
        void Update();
        void Render() const;
        void OnPause(bool);
        void OnCleanup();

    private:
        void InitPaddle();
        void InitStats();
        void InitBackground();
        void InitBounceSound();
        void InitMusic();

        void BindInputs();

        void StartPaddle();
        void StartStats();
        void StartMusic();

        void UpdatePaddle();
        void UpdateBalls();
        void UpdateBall(Ball& BallToUpdate);

        void EventBallSpawn();
        void EventBallsMissed(int BallsMissed);

        void HandleCollisions();
        void HandleCollisionsPaddleBalls();
        void ResolveCollisionPaddleBall(Ball& CollidedBall);
        void HandleCollisionsBallsMap();
    };
}
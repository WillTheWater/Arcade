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

        Timer ExtraLifeSpawnTimer;
        std::optional<ExtraLife> ExtraLifePickup;

    public:
        Game(Managers&);

        void Start();
        void Update();
        void UpdateExtraLife();
        void Render() const;
        void OnPause(bool);

    private:
        void InitPaddle();
        void InitStats();

        void BindInputs();

        void StartPaddle();
        void StartStats();

        void UpdatePaddle();
        void UpdateBalls();
        void UpdateBall(Ball& BallToUpdate);

        void EventSpawnExtraLife();

        void EventBallSpawn();
        void EventBallsMissed(int BallsMissed);

        void HandleCollisions();
        void HandleCollisionsPaddleBalls();
        void ResolveCollisionPaddleBall(Ball& CollidedBall);
        void HandleCollisionsBallsMap();
    };
}
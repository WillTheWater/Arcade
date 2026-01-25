#include "Bounce.h"
	
Bounce::Game::Game(Managers& Mgr)
	: Scene{Mgr}
	, BallSpawnCooldown{BALL_SPAWN_COOLDOWN_DURATION}
	, ExtraLifeSpawnTimer{ EXTRA_LIFE_SPAWN_INTERVAL }
{
	InitPaddle();
	InitStats();
}

void Bounce::Game::InitPaddle()
{
	Paddle.Shape.setFillColor(PADDLE_COLOR);
	Paddle.Shape.setOutlineColor(OBJECT_OUTLINE_COLOR);
	Paddle.Shape.setOutlineThickness(OBJECT_OUTLINE_THICKNESS);
	Paddle.Shape.setSize(PADDLE_SIZE);
	Paddle.Shape.setOrigin(Paddle.Shape.getGeometricCenter());
	Paddle.Speed = PADDLE_SPEED;
}

void Bounce::Game::InitStats()
{
	Stats.ScoreText.setFillColor(STATS_TEXT_COLOR);
	Stats.ScoreText.setCharacterSize(STATS_TEXT_SIZE);
	Stats.ScoreText.setOutlineColor(OUTLINE_COLOR);
	Stats.ScoreText.setOutlineThickness(OUTLINE_THICKNESS);
	Stats.ScoreText.setPosition({ 35,20 });

	Stats.HighScoreText.setFillColor(STATS_TEXT_COLOR);
	Stats.HighScoreText.setCharacterSize(STATS_TEXT_SIZE);
	Stats.HighScoreText.setOutlineColor(OUTLINE_COLOR);
	Stats.HighScoreText.setOutlineThickness(OUTLINE_THICKNESS);
	Stats.HighScoreText.setPosition({ 35,80 });

	Stats.LivesText.setFillColor(STATS_TEXT_COLOR);
	Stats.LivesText.setCharacterSize(STATS_TEXT_SIZE);
	Stats.LivesText.setOutlineColor(OUTLINE_COLOR);
	Stats.LivesText.setOutlineThickness(OUTLINE_THICKNESS);
	Stats.LivesText.setPosition({ 35, 140 });
}

void Bounce::Game::Start()
{
	MGR.Cursor.SetVisibility(false);

	BindInputs();
	Balls.clear();
	StartPaddle();
	StartStats();
	BallSpawnCooldown.Restart();
}

void Bounce::Game::BindInputs()
{
	MGR.Input.Bind(MoveLeft, Input::Keyboard{ sf::Keyboard::Scan::Left});
	MGR.Input.Bind(MoveLeft, Input::Keyboard{ sf::Keyboard::Scan::A});
	MGR.Input.Bind(MoveLeft, Input::Axis{ sf::Joystick::Axis::X, -0.2f});

	MGR.Input.Bind(MoveRight, Input::Keyboard{ sf::Keyboard::Scan::Right });
	MGR.Input.Bind(MoveRight, Input::Keyboard{ sf::Keyboard::Scan::D });
	MGR.Input.Bind(MoveRight, Input::Axis{ sf::Joystick::Axis::X, 0.2f });
}

void Bounce::Game::StartPaddle()
{
	Paddle.Shape.setPosition(EConfig.WindowSize.componentWiseMul({ 0.50f, 0.90f }));
}

void Bounce::Game::StartStats()
{
	Stats.Score = 0;
	Stats.ScoreText.setString("Score: 0");

	Stats.HighScore = MGR.Save.Get<int>(STATS_HIGH_SCORE_KEY);
	Stats.HighScoreText.setString("High Score: " + std::to_string(Stats.HighScore));

	Stats.Lives = PADDLE_LIVES;
	Stats.LivesText.setString("Lives: " + std::to_string(Stats.Lives));
}


void Bounce::Game::UpdatePaddle()
{
	sf::Vector2f PreviousPosition = Paddle.Shape.getPosition();
	if (MGR.Input.Pressed(MoveLeft))
	{
		Paddle.Shape.move({ -Paddle.Speed * MGR.Timer.GetDeltaTime(), 0 });
	}
	if (MGR.Input.Pressed(MoveRight))
	{
		Paddle.Shape.move({ Paddle.Speed * MGR.Timer.GetDeltaTime(), 0 });
	}
	if (IsOutsideWindowLeft(Paddle.Shape) || IsOutsideWindowRight(Paddle.Shape))
	{
		Paddle.Shape.setPosition(PreviousPosition);
	}
}

void Bounce::Game::UpdateBalls()
{
	for (auto& B : Balls)
	{
		UpdateBall(B);
	}
}

void Bounce::Game::UpdateBall(Ball& BallToUpdate)
{
	sf::Vector2f lastPosition = BallToUpdate.Shape.getPosition();

	BallToUpdate.Shape.move(BallToUpdate.Direction * BallToUpdate.Speed * MGR.Timer.GetDeltaTime());
	
	if (IsOutsideWindowLeft(BallToUpdate.Shape) || IsOutsideWindowRight(BallToUpdate.Shape))
	{
		BallToUpdate.Direction.x *= -1;
		BallToUpdate.Shape.setPosition(lastPosition);
	}

	if (IsOutsideWindowTop(BallToUpdate.Shape))
	{
		BallToUpdate.Direction.y *= -1;
		BallToUpdate.Shape.setPosition(lastPosition);
	}
}

void Bounce::Game::EventSpawnExtraLife()
{
	ExtraLifePickup.emplace();
	auto& life = *ExtraLifePickup;

	life.Shape.setSize({ EXTRA_LIFE_SIZE, EXTRA_LIFE_SIZE });
	life.Shape.setOrigin(life.Shape.getGeometricCenter());
	life.Shape.setFillColor(EXTRA_LIFE_COLOR);
	life.Shape.setOutlineColor(OBJECT_OUTLINE_COLOR);
	life.Shape.setOutlineThickness(OBJECT_OUTLINE_THICKNESS);

	float minX = 40.f + EXTRA_LIFE_SIZE * 0.5f;
	float maxX = EConfig.WindowSize.x - minX;

	float x = MGR.Randomizer.Random(minX, maxX);

	life.Shape.setPosition({ x, -100.f });
	life.RotationSpeed = EXTRA_LIFE_ROTATION_SPEED;
}


void Bounce::Game::EventBallSpawn()
{
	auto& B = Balls.emplace_back();
	B.Shape.setFillColor(BALL_COLORS[MGR.Randomizer.Random(0, (int)BALL_COLORS.size() - 1)]);
	B.Shape.setOutlineColor(OBJECT_OUTLINE_COLOR);
	B.Shape.setOutlineThickness(OBJECT_OUTLINE_THICKNESS);
	B.Shape.setRadius(BALL_RADIUS);
	B.Shape.setOrigin(B.Shape.getGeometricCenter());
	B.Shape.setPosition(EConfig.WindowSize.componentWiseMul({ 0.50f, 0.25f }));
	B.Direction = { 1, MGR.Randomizer.Random(sf::Angle::Zero, sf::degrees(360)) };
	B.Speed = BALL_SPEED;
}

void Bounce::Game::EventBallsMissed(int BallsMissed)
{
	Stats.Lives -= BallsMissed;
	Stats.LivesText.setString("Lives: " + std::to_string(Stats.Lives));

	if (Stats.Lives < 0)
	{
		MGR.Save.Set(STATS_HIGH_SCORE_KEY, std::max(Stats.Score, Stats.HighScore));
		LOG("You Lose! Score: {}", Stats.Score);
		MGR.Scene.ReloadScene();
	}
}

void Bounce::Game::HandleCollisions()
{
	HandleCollisionsPaddleBalls();
	HandleCollisionsBallsMap();
}

void Bounce::Game::HandleCollisionsPaddleBalls()
{
	for (auto& B : Balls)
	{
		if (B.Direction.y > 0 && Intersects(B.Shape, Paddle.Shape))
		{
			ResolveCollisionPaddleBall(B);
		}
	}
}

void Bounce::Game::ResolveCollisionPaddleBall(Ball& CollidedBall)
{
	float paddleX = Paddle.Shape.getPosition().x;
	float ballX = CollidedBall.Shape.getPosition().x;
	float halfWidth = Paddle.Shape.getSize().x * 0.5f;

	float offset = (ballX - paddleX) / halfWidth;
	offset = std::clamp(offset, -1.f, 1.f);

	constexpr float MAX_ANGLE = 80.f;
	float angleDeg = offset * MAX_ANGLE;
	float angleRad = angleDeg * (std::numbers::pi_v<float> / 180.f);

	CollidedBall.Direction = {
		std::sin(angleRad),
		-std::cos(angleRad)
	};

	Stats.Score += 5;
	Stats.ScoreText.setString("Score: " + std::to_string(Stats.Score));
}


void Bounce::Game::HandleCollisionsBallsMap()
{
	int Missed = (int)std::erase_if(Balls, [](const Ball& B)
		{
			return IsOutsideWindowBottom(B.Shape);
		});

	if (Missed > 0)
	{
		EventBallsMissed(Missed);
	}
}

void Bounce::Game::Update()
{
	if (BallSpawnCooldown.IsOver())
	{
		EventBallSpawn();
		BallSpawnCooldown.Restart();
	}
	if (ExtraLifeSpawnTimer.IsOver() && !ExtraLifePickup)
	{
		EventSpawnExtraLife();
		ExtraLifeSpawnTimer.Restart();
	}
	UpdatePaddle();
	UpdateBalls();
	UpdateExtraLife();
	HandleCollisions();
}

void Bounce::Game::UpdateExtraLife()
{
	if (!ExtraLifePickup) return;

	auto& life = *ExtraLifePickup;

	life.Shape.move({ 0.f, EXTRA_LIFE_FALL_SPEED * MGR.Timer.GetDeltaTime() });
	life.Shape.rotate(sf::degrees(EXTRA_LIFE_ROTATION_SPEED * MGR.Timer.GetDeltaTime()));

	if (Intersects(life.Shape, Paddle.Shape))
	{
		Stats.Lives++;
		Stats.LivesText.setString("Lives: " + std::to_string(Stats.Lives));
		ExtraLifePickup.reset();
		return;
	}

	if (IsOutsideWindowBottom(life.Shape))
	{
		ExtraLifePickup.reset();
	}
}


void Bounce::Game::Render() const
{
	MGR.Renderer.Draw(Paddle.Shape);

	for (const auto& B : Balls)
	{
		MGR.Renderer.Draw(B.Shape);
	}
	if (ExtraLifePickup)
	{
		MGR.Renderer.Draw(ExtraLifePickup->Shape);
	}

	MGR.Renderer.Draw(Stats.ScoreText);
	MGR.Renderer.Draw(Stats.HighScoreText);
	MGR.Renderer.Draw(Stats.LivesText);
}

void Bounce::Game::OnPause(bool Paused)
{
	if (Paused)
	{
		BallSpawnCooldown.Stop();
	}
	else
	{
		BallSpawnCooldown.Start();
	}
}
#include "Bounce.h"
	
Bounce::Game::Game(Managers& Mgr)
	: Scene{Mgr}
	, BallSpawnCooldown{BALL_SPAWN_COOLDOWN_DURATION}
	, BounceSound{*MGR.Assets.GetSound(BOUNCE_SOUND_FILENAME)}
	, Music{*MGR.Assets.GetMusic(MUSIC_FILENAME)}
{
	InitPaddle();
	InitStats();
	InitBackground();
	InitBounceSound();
	InitMusic();
}

void Bounce::Game::InitPaddle()
{
	Paddle.Shape.setFillColor(PADDLE_COLOR);
	Paddle.Shape.setSize(PADDLE_SIZE);
	Paddle.Shape.setOrigin(Paddle.Shape.getGeometricCenter());
	Paddle.Speed = PADDLE_SPEED;
}

void Bounce::Game::InitStats()
{
	Stats.ScoreText.setFillColor(STATS_SCORE_TEXT_COLOR);
	Stats.ScoreText.setPosition({ 20,20 });

	Stats.HighScoreText.setFillColor(STATS_HIGH_SCORE_TEXT_COLOR);
	Stats.HighScoreText.setPosition({ 20,100 });

	Stats.LivesText.setFillColor(STATS_LIVES_TEXT_COLOR);
	Stats.LivesText.setPosition({ 10, 180 });
}

void Bounce::Game::InitBackground()
{
	auto X = (float)sf::VideoMode::getDesktopMode().size.x;
	auto Y = (float)sf::VideoMode::getDesktopMode().size.y;
	auto Pos = sf::Vector2f({ X,Y }).componentWiseMul({ 0.5f, 0.5f });
	Background.setTexture(MGR.Assets.GetTexture(BACKGROUND_TEXTURE_FILENAME));
	Background.setSize(EConfig.WindowSize);
	Background.setOrigin(Background.getGeometricCenter());
	Background.setPosition(Pos);
}

void Bounce::Game::InitBounceSound()
{
	BounceSound.setVolume(BOUNCE_SOUND_VOLUME);
}

void Bounce::Game::InitMusic()
{
	Music.setVolume(MUSIC_VOLUME);
	Music.setPitch(MUSIC_PITCH);
	Music.setLooping(true);
}

void Bounce::Game::Start()
{
	MGR.Cursor.SetVisibility(false);

	BindInputs();
	Balls.clear();
	StartPaddle();
	StartStats();
	StartMusic();
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

void Bounce::Game::StartMusic()
{
	Music.play();
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

void Bounce::Game::EventBallSpawn()
{
	auto& B = Balls.emplace_back();
	B.Shape.setFillColor(BALL_COLOR);
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
	CollidedBall.Direction.y *= -1;
	BounceSound.play();

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
	UpdatePaddle();
	UpdateBalls();
	HandleCollisions();
}

void Bounce::Game::Render() const
{
	MGR.Renderer.Draw(Background);
	MGR.Renderer.Draw(Paddle.Shape);

	for (const auto& B : Balls)
	{
		MGR.Renderer.Draw(B.Shape);
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
		Music.pause();
		BounceSound.stop();
	}
	else
	{
		BallSpawnCooldown.Start();
		Music.play();
	}
}

void Bounce::Game::OnCleanup()
{
	Music.stop();
	BounceSound.stop();
}
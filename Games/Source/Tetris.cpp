#include "Tetris.h"

namespace Tetris
{
	Game::Game(Managers& Mgr)
		: Scene{ Mgr }
		, ActionCooldown{ACTION_COOLDOWN_DURATION}
		, FallCooldown{PIECE_FALL_COOLDOWN_DURATION}
		, DropCooldown{SOFT_DROP_COOLDOWN_DURATION}
		, StartCountdown{ START_COUNTDOWN_DURATION } // ADDED
		, LevelTimer{ LEVEL_DURATION }               // ADDED
	{
		InitStats();
	}

	void Game::InitStats()
	{
		PlayerStats.ScoreText.setPosition({ 10,10 });
		PlayerStats.HighScoreText.setPosition({ 10,40 }); // ADDED
		PlayerStats.LevelText.setPosition({ 10,70 });     // ADDED
	}

	void Game::Start()
	{
		MGR.Cursor.SetVisibility(false);

		BindInput();
		StartGrid();
		StartNextPiece();
		StartStats();

		StartCountdown.Start(); // ADDED
		HasStarted = false;

		ActionCooldown.Stop();
		FallCooldown.Stop();
		DropCooldown.Stop();
	}

	void Game::BindInput()
	{
		MGR.Input.Bind(MoveRight, Input::Keyboard{ sf::Keyboard::Scan::Right });
		MGR.Input.Bind(MoveRight, Input::Keyboard{ sf::Keyboard::Scan::D });
		MGR.Input.Bind(MoveRight, Input::Axis{ sf::Joystick::Axis::X, 0.5f });

		MGR.Input.Bind(MoveLeft, Input::Keyboard{ sf::Keyboard::Scan::Left });
		MGR.Input.Bind(MoveLeft, Input::Keyboard{ sf::Keyboard::Scan::A });
		MGR.Input.Bind(MoveLeft, Input::Axis{ sf::Joystick::Axis::X, -0.5f });

		MGR.Input.Bind(Rotate, Input::Keyboard{ sf::Keyboard::Scan::Up });
		MGR.Input.Bind(Rotate, Input::Keyboard{ sf::Keyboard::Scan::W });
		MGR.Input.Bind(Rotate, Input::Gamepad{ GamepadButton::North });

		MGR.Input.Bind(SoftDrop, Input::Keyboard{ sf::Keyboard::Scan::Down });
		MGR.Input.Bind(SoftDrop, Input::Keyboard{ sf::Keyboard::Scan::S });
		MGR.Input.Bind(SoftDrop, Input::Axis{ sf::Joystick::Axis::Y, 0.5f });
	}

	void Game::Update()
	{
		// ADDED: start countdown
		if (!HasStarted)
		{
			if (StartCountdown.IsOver())
			{
				HasStarted = true;
				EventPieceSpawn();
				ActionCooldown.Restart();
				FallCooldown.Restart();
				DropCooldown.Restart();
				LevelTimer.Restart();
			}
			return;
		}

		UpdateScoreOverTime(); // ADDED

		if (LevelTimer.IsOver())
		{
			AdvanceLevel();
			LevelTimer.Restart();
		}

		if (MGR.Input.Pressed(Rotate) && ActionCooldown.IsOver())
		{
			EventPieceRotate();
			ActionCooldown.Restart();
		}
		if (MGR.Input.Pressed(MoveRight) && ActionCooldown.IsOver())
		{
			EventPieceMove({ 1,0 });
			ActionCooldown.Restart();
		}
		else if (MGR.Input.Pressed(MoveLeft) && ActionCooldown.IsOver())
		{
			EventPieceMove({ -1,0 });
			ActionCooldown.Restart();
		}
		if (MGR.Input.Pressed(SoftDrop) && DropCooldown.IsOver())
		{
			EventPieceFall();
			DropCooldown.Restart();
			FallCooldown.Restart();
		}
		else if (FallCooldown.IsOver())
		{
			EventPieceFall();
			FallCooldown.Restart();
		}
	}

	void Game::AdvanceLevel()
	{
		PlayerStats.Level++;
		PlayerStats.LevelText.setString("Level: " + std::to_string(PlayerStats.Level));

		FallCooldown.SetDuration(FallCooldown.GetDuration() * FALL_SPEED_MULTIPLIER);
	}

	void Game::UpdateScoreOverTime()
	{
		int currentSecond = static_cast<int>(LevelTimer.GetElapsedTime());

		if (currentSecond > LastScoreSecond)
		{
			int secondsPassed = currentSecond - LastScoreSecond;

			PlayerStats.Score += secondsPassed * 10 * PlayerStats.Level;

			LastScoreSecond = currentSecond;

			PlayerStats.ScoreText.setString(
				"Score: " + std::to_string(PlayerStats.Score)
			);
		}
	}


	void Game::Render() const
	{
		RenderBoard();

		if (HasStarted) // NEW
		{
			RenderPiece(CurrentPiece, GetBoardOrigin(), true);
			RenderPiece(NextPiece, { EConfig.WindowSize.x - 5.5f * BLOCK_SIZE, BLOCK_SIZE * 2.5f }, false);
		}
		MGR.Renderer.Draw(PlayerStats.ScoreText);
		MGR.Renderer.Draw(PlayerStats.HighScoreText);
		MGR.Renderer.Draw(PlayerStats.LevelText);

	}

	void Game::OnEvent(const sf::Event& Event)
	{
		if (auto Key = Event.getIf<sf::Event::KeyPressed>())
		{
			if (Key->scancode == sf::Keyboard::Scan::Space)
			{
				EventPieceDropHard();
			}
		}
		if (auto Gamepad = Event.getIf<sf::Event::JoystickButtonPressed>())
		{
			if (Input::HardwareToLogic(Gamepad->button, Gamepad->joystickId) == GamepadButton::South)
			{
				EventPieceDropHard();
			}
		}
	}

	void Game::OnPause(bool Paused)
	{
		if (Paused)
		{
			ActionCooldown.Stop();
			FallCooldown.Stop();
			DropCooldown.Stop();
			StartCountdown.Stop();
			LevelTimer.Stop();
		}
		else
		{
			ActionCooldown.Start();
			FallCooldown.Start();
			DropCooldown.Start();
			StartCountdown.Start();
			LevelTimer.Start();
		}
	}

	void Game::StartGrid()
	{
		TetrisBoard = {};
	}

	void Game::StartNextPiece()
	{
		NextPiece = GenerateRandomPiece();
	}

	void Game::StartStats()
	{
		PlayerStats.Score = 0;
		PlayerStats.Level = 1;

		PlayerStats.HighScore =	MGR.Save.Get<int>(STATS_HIGHSCORE_KEY, 0); // ADDED

		PlayerStats.ScoreText.setString("Score: 0");
		PlayerStats.HighScoreText.setString("High: " + std::to_string(PlayerStats.HighScore));
		PlayerStats.LevelText.setString("Level: 1");
	}

	void Game::EventPieceSpawn()
	{
		CurrentPiece = NextPiece;
		NextPiece = GenerateRandomPiece();
	}

	void Game::EventPieceRotate()
	{
		Piece Rotated = CurrentPiece;
		Rotated.Rotation = (Rotated.Rotation + 1) % 4;
		if (IsPieceValid(Rotated))
		{
			CurrentPiece = Rotated;
		}
	}

	void Game::EventPieceMove(sf::Vector2i Offset)
	{
		Piece Moved = CurrentPiece;
		Moved.Position += Offset;
		if (IsPieceValid(Moved))
		{
			CurrentPiece = Moved;
		}
	}
	void Game::EventPieceFall()
	{
		Piece Moved = CurrentPiece;
		Moved.Position.y++;
		if (IsPieceValid(Moved))
		{
			CurrentPiece = Moved;
		}
		else
		{
			EventPieceMerge();
		}
	}

	void Game::EventPieceDropHard()
	{
		while (IsPieceValid(CurrentPiece))
		{
			CurrentPiece.Position.y++;
		}
		CurrentPiece.Position.y--;
		EventPieceMerge();
	}

	void Game::EventPieceMerge()
	{
		for (int i = 0; i < PIECE_BLOCK_COUNT; i++)
		{
			sf::Vector2i Block = GetPointRotated(CurrentPiece.Type, i, CurrentPiece.Rotation) + CurrentPiece.Position;

			if (Block.y >= 0 && Block.y < GRID_HEIGHT &&
				Block.x >= 0 && Block.x < GRID_WIDTH)
			{
				TetrisBoard[Block.y][Block.x] = CurrentPiece.Color;
			}
		}
		EventLinesClear();
		EventPieceSpawn();
		if (!IsPieceValid(CurrentPiece))
		{
			// ADDED: save high score
			if (PlayerStats.Score > PlayerStats.HighScore)
			{
				MGR.Save.Set(STATS_HIGHSCORE_KEY, PlayerStats.Score);
				PlayerStats.HighScore = std::max(PlayerStats.HighScore, PlayerStats.Score);
				PlayerStats.HighScoreText.setString("High: " + std::to_string(PlayerStats.HighScore));

			}
			MGR.Scene.ReloadScene();
		}
	}

	void Game::EventLinesClear()
	{
		Board SimBoard = {};
		int Cleared = 0;
		int DestinationY = GRID_HEIGHT - 1;

		for (int y = GRID_HEIGHT - 1; y >= 0; y--)
		{
			if (std::ranges::all_of(TetrisBoard[y], [](auto Color) { return Color.has_value(); }))
			{
				Cleared++;
			}
			else
			{
				SimBoard[DestinationY] = TetrisBoard[y];
				DestinationY--;
			}
		}
		if (Cleared > 0)
		{
			TetrisBoard = SimBoard;
			PlayerStats.Score += Cleared * STATS_SCORE_PER_LINE;
			PlayerStats.ScoreText.setString("Score: " + std::to_string(PlayerStats.Score));
		}
	}

	Piece Game::GenerateRandomPiece() const
	{
		Piece NewPiece;
		NewPiece.Type = MGR.Randomizer.Random(0, PIECE_TYPE_COUNT - 1);
		NewPiece.Rotation = 0;
		NewPiece.Position = { (GRID_WIDTH / 2) - 2, 0 };
		NewPiece.Color = MGR.Randomizer.Random(sf::Color::Black, sf::Color::White);
		return NewPiece;
	}

	sf::Vector2i Game::GetPointRotated(int Type, int Index, int Rotation) const
	{
		sf::Vector2f Point(PIECE_SHAPES[Type][Index]);
		sf::Vector2f Pivot = PIECE_PIVOTS[Type];
		sf::Vector2f Origin = Point - Pivot;
		sf::Vector2f Rotated;
		switch (Rotation)
		{
		case 0: Rotated = Origin; break;
		case 1: Rotated = { -Origin.y, Origin.x }; break;
		case 2: Rotated = { -Origin.x, -Origin.y }; break;
		case 3: Rotated = { Origin.y, -Origin.x }; break;
		}
		Rotated += Pivot;
		return { int(std::round(Rotated.x)), int(std::round(Rotated.y)) };
	}

	sf::Vector2f Game::GetBoardOrigin() const
	{
		return { (EConfig.WindowSize.x - GRID_WIDTH * BLOCK_SIZE) / 2,
				 (EConfig.WindowSize.y - GRID_HEIGHT * BLOCK_SIZE) / 2 };
	}

	bool Game::IsPieceValid(const Piece& PieceToCheck) const
	{
		for (int i = 0; i < PIECE_BLOCK_COUNT; i++)
		{
			sf::Vector2i Block = GetPointRotated(PieceToCheck.Type, i, PieceToCheck.Rotation) + PieceToCheck.Position;

			if (Block.x < 0 || Block.x >= GRID_WIDTH ||
				Block.y < 0 || Block.y >= GRID_HEIGHT ||
				(Block.y >= 0 && TetrisBoard[Block.y][Block.x].has_value()))
			{
				return false;
			}
		}
		return true;
	}

	void Game::RenderBoard() const
	{
		sf::Vector2f Origin = GetBoardOrigin();
		sf::RectangleShape Shape({ BLOCK_SIZE, BLOCK_SIZE });
		Shape.setOutlineColor(GRID_COLOR);
		Shape.setOutlineThickness(-1);

		for (int y = 0; y < GRID_HEIGHT; y++)
		{
			for (int x = 0; x < GRID_WIDTH; x++)
			{
				Shape.setFillColor(TetrisBoard[y][x].value_or(sf::Color::Transparent));
				Shape.setPosition(Origin + sf::Vector2f(x * BLOCK_SIZE, y * BLOCK_SIZE));
				MGR.Renderer.Draw(Shape);
			} 
		}
	}

	void Game::RenderPiece(const Piece& PieceToRender, sf::Vector2f Origin, bool World) const
	{
		sf::RectangleShape Shape({ BLOCK_SIZE, BLOCK_SIZE });
		Shape.setFillColor(PieceToRender.Color);

		for (int i = 0; i < PIECE_BLOCK_COUNT; i++)
		{
			sf::Vector2i Block = GetPointRotated(PieceToRender.Type, i, PieceToRender.Rotation);
			if (World)
			{
				Block += PieceToRender.Position;
			}
			Shape.setPosition(Origin + sf::Vector2f(Block.x * BLOCK_SIZE, Block.y * BLOCK_SIZE));
			MGR.Renderer.Draw(Shape);
		}
	}

	void Game::HandleEvent(const sf::Event::TextEntered&)
	{

	}

	void Game::HandleEvent(const sf::Event::KeyPressed&)
	{

	}

	void Game::HandleEvent(const sf::Event::JoystickButtonPressed&)
	{

	}

	void Game::HandleEvent(const sf::Event::MouseButtonPressed&)
	{

	}

	void Game::HandleEvent(const sf::Event::MouseWheelScrolled&)
	{

	}
}
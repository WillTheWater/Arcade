#include "Clicker.h"

Clicker::Game::Game(Managers& Mgr)
	:Scene{ Mgr }
{
	InitTarget();
	InitStats();
}

void Clicker::Game::InitTarget()
{
	ClickTarget.Shape.setTexture(MGR.Assets.GetTexture(TARGET_TEXTURE_FILENAME));
	ClickTarget.Shape.setRadius(TARGET_RADIUS);
	ClickTarget.Shape.setOrigin(ClickTarget.Shape.getGeometricCenter());
	ClickTarget.TeleportCooldown.SetDuration(TARGET_TELEPORT_COOLDOWN_DURATION);
}

void Clicker::Game::InitStats()
{
	ClickStats.ScoreText.setFillColor(STATS_SCORE_TEXT_COLOR);
	ClickStats.ScoreText.setCharacterSize(STATS_TEXT_SIZE);
	ClickStats.ScoreText.setPosition({ 25, 25 });
	ClickStats.ScoreText.setOutlineColor(OUTLINE_COLOR);
	ClickStats.ScoreText.setOutlineThickness(OUTLINE_THICKNESS);

	ClickStats.FinalCooldown.SetDuration(STATS_FINAL_COOLDOWN_DURATION);
	ClickStats.FinalCooldownText.setFillColor(STATS_FINAL_COOLDOWN_TEXT_COLOR);
	ClickStats.FinalCooldownText.setCharacterSize(STATS_TEXT_SIZE);
	ClickStats.FinalCooldownText.setPosition({ 25,75 });
	ClickStats.FinalCooldownText.setOutlineColor(OUTLINE_COLOR);
	ClickStats.FinalCooldownText.setOutlineThickness(OUTLINE_THICKNESS);

	ClickStats.BestTimeText.setFillColor(STATS_BEST_TIME_TEXT_COLOR);
	ClickStats.BestTimeText.setCharacterSize(STATS_TEXT_SIZE);
	ClickStats.BestTimeText.setPosition({ 25,125 });
	ClickStats.BestTimeText.setOutlineColor(OUTLINE_COLOR);
	ClickStats.BestTimeText.setOutlineThickness(OUTLINE_THICKNESS);
}

void Clicker::Game::Start()
{
	MGR.Cursor.SetCursorSpeed(CURSOR_SPEED);

	BindInput();
	StartTarget();
	StartStats();
}

void Clicker::Game::BindInput()
{
	MGR.Input.Bind(Click, Input::Mouse{ sf::Mouse::Button::Left });
	MGR.Input.Bind(Click, Input::Gamepad{ GamepadButton::South });
}

void Clicker::Game::StartTarget()
{
	ClickTarget.Shape.setFillColor(TARGET_COLOR);
	ClickTarget.Shape.setPosition(EConfig.WindowSize / 2.f);
}

void Clicker::Game::StartStats()
{
	ClickStats.Score = 0;
	ClickStats.ScoreText.setString("Score: 0/" + std::to_string(STATS_SCORE_GOAL));

	ClickStats.FinalCooldown.Reset();

	float TotalTime = ClickStats.FinalCooldown.GetDuration();
	ClickStats.FinalCooldownText.setString("Time Left: " + std::to_string((int)TotalTime));

	ClickStats.BestTime = MGR.Save.Get<float>(STATS_BEST_TIME_KEY, TotalTime);
	ClickStats.BestTimeText.setString(std::format("Best Time: {:.2f}", ClickStats.BestTime));
}

void Clicker::Game::Update()
{
	if (MGR.Input.Pressed(Click) && IsTargetHovered())
	{
		EventTargetClicked();
	}
	if (ClickStats.FinalCooldown.IsRunning())
	{
		UpdateTarget();
		UpdateStats();
	}
}

void Clicker::Game::UpdateTarget()
{
	if (ClickTarget.TeleportCooldown.IsOver())
	{
		EventTargetTeleport();
		ClickTarget.TeleportCooldown.Restart();
	}
	UpdateTargetColor();
}

void Clicker::Game::UpdateTargetColor()
{
	float Progress = ClickTarget.TeleportCooldown.GetElapsedTime() / ClickTarget.TeleportCooldown.GetDuration();
	sf::Color Color = ClickTarget.Shape.getFillColor();
	Color.a = std::uint8_t(255 * (1 - std::min(Progress, 1.f)));
	ClickTarget.Shape.setFillColor(Color);
}

void Clicker::Game::UpdateStats()
{
	float TimeLeft = ClickStats.FinalCooldown.GetDuration() - ClickStats.FinalCooldown.GetElapsedTime();
	ClickStats.FinalCooldownText.setString(std::format("Time Left: {:.1f}", TimeLeft));

	if (ClickStats.FinalCooldown.IsOver())
	{
		MGR.Scene.ReloadScene();
	}
}

void Clicker::Game::EventTargetClicked()
{
	EventTargetTeleport();
	ClickTarget.TeleportCooldown.Restart();
	if (ClickStats.Score == 0)
	{
		ClickStats.FinalCooldown.Start();
	}
	EventStatsScoreIncrease();
}

void Clicker::Game::EventTargetTeleport()
{
	auto Min = sf::Vector2f({ ClickTarget.Shape.getGlobalBounds().size.x, EConfig.WindowSize.y * 0.25f });
	auto Max = sf::Vector2f({ EConfig.WindowSize.x - ClickTarget.Shape.getGlobalBounds().size.x, EConfig.WindowSize.y - ClickTarget.Shape.getGlobalBounds().size.y });
	ClickTarget.Shape.setPosition(MGR.Randomizer.Random(Min, Max ));
}

void Clicker::Game::EventStatsScoreIncrease()
{
	ClickStats.Score++;
	ClickStats.ScoreText.setString(std::format("Score: {}/{}", ClickStats.Score, STATS_SCORE_GOAL));

	if (ClickStats.Score == STATS_SCORE_GOAL)
	{
		float ElapsedTime = ClickStats.FinalCooldown.GetElapsedTime();
		MGR.Save.Set(STATS_BEST_TIME_KEY, std::min(ElapsedTime, ClickStats.BestTime));
		MGR.Scene.ReloadScene();
	}
}

bool Clicker::Game::IsTargetHovered() const
{
	if (Contains(ClickTarget.Shape, MGR.Cursor.GetPosition()))
	{
		LOG("HOVERED!");
	}
	return Contains(ClickTarget.Shape, MGR.Cursor.GetPosition());
}

void Clicker::Game::Render() const
{
	MGR.Renderer.Draw(ClickTarget.Shape);
	MGR.Renderer.Draw(ClickStats.ScoreText);
	MGR.Renderer.Draw(ClickStats.FinalCooldownText);
	MGR.Renderer.Draw(ClickStats.BestTimeText);
}

void Clicker::Game::OnPause(bool Paused)
{
	if (Paused)
	{
		ClickTarget.TeleportCooldown.Stop();
		ClickStats.FinalCooldown.Stop();
	}
	else
	{
		ClickTarget.TeleportCooldown.Start();
		ClickStats.FinalCooldown.Start();
	}
}
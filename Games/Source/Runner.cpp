#include "Runner.h"

Dino::Game::Game(Managers& Mgr)
    : Scene{ Mgr }
    , ObstacleSpawnTimer{ OBSTACLE_SPAWN_INTERVAL }
{
    InitDino();
    InitGround();
    InitStats();
}

void Dino::Game::InitDino()
{
    Dino.Shape.setSize(DINO_SIZE);
    Dino.Shape.setOrigin(DINO_SIZE / 2.f);
    Dino.Shape.setFillColor(DINO_COLOR);

    Dino.Shape.setPosition({
        EConfig.WindowSize.x * 0.2f,
        -DINO_SIZE.y    // ABOVE window
        });

    Dino.Velocity = { 0.f, 0.f };
    Dino.IsOnGround = false;
    Dino.IsDucking = false;
}

float Dino::Game::GetGroundSnapY(float dinoHeight) const
{
    return Ground.getPosition().y
        - GROUND_SIZE.y / 2.f
        - dinoHeight / 2.f;
}

void Dino::Game::InitGround()
{
    Ground.setSize(GROUND_SIZE);
    Ground.setFillColor(GROUND_COLOR);
    Ground.setOrigin(GROUND_SIZE / 2.f);

    // CHANGED: ground moved up (lower 1/3 of screen)
    Ground.setPosition({EConfig.WindowSize.x * 0.5f, EConfig.WindowSize.y * 0.70f});
}

void Dino::Game::InitStats()
{
    Stats.Score = 0;

    Stats.ScoreText.setFillColor(STATS_TEXT_COLOR);
    Stats.ScoreText.setCharacterSize(STATS_TEXT_SIZE);
    Stats.ScoreText.setPosition({ 30, 30 });
    Stats.ScoreText.setString("Score: 0");

    Stats.HighScore = MGR.Save.Get<int>(STATS_HIGH_SCORE_KEY);

    Stats.HighScoreText.setFillColor(STATS_TEXT_COLOR);
    Stats.HighScoreText.setCharacterSize(STATS_TEXT_SIZE);
    Stats.HighScoreText.setPosition({ 30, 90 });
    Stats.HighScoreText.setString("High Score: " + std::to_string(Stats.HighScore));
}

bool Dino::Game::CanJump()
{
    return Dino.IsOnGround;
}

void Dino::Game::Start()
{
    MGR.Cursor.SetVisibility(false);

    BindInputs();
    Obstacles.clear();
    ObstacleSpawnTimer.Restart();

    Dino.Velocity = { 0.f, 0.f };
    Dino.IsOnGround = false;
    Dino.IsDucking = false;

    Dino.Shape.setSize(DINO_SIZE);
    Dino.Shape.setOrigin(DINO_SIZE / 2.f);

    Dino.Shape.setPosition({
        Dino.Shape.getPosition().x,
        -DINO_SIZE.y   // FORCE fall-in every start
        });

    ScoreStartTime = MGR.Timer.GetElapsedTime();
    Stats.Score = 0;
    Stats.ScoreText.setString("Score: 0");
}


void Dino::Game::BindInputs()
{
    // Duck
    MGR.Input.Bind(Duck, Input::Keyboard{ sf::Keyboard::Scancode::Down });
    MGR.Input.Bind(Duck, Input::Keyboard{ sf::Keyboard::Scancode::S });
}

void Dino::Game::UpdateDino()
{
    const float dt = MGR.Timer.GetDeltaTime();


    if (MGR.Input.Pressed(Duck) && !Dino.IsDucking)
    {
        Dino.IsDucking = true;
        Dino.Shape.setSize(DINO_DUCK_SIZE);
        Dino.Shape.setOrigin(DINO_DUCK_SIZE / 2.f);

        if (Dino.IsOnGround)
        {
            Dino.Shape.setPosition({
                Dino.Shape.getPosition().x,
                GetGroundSnapY(DINO_DUCK_SIZE.y)
                });
        }
    }
    else if (!MGR.Input.Pressed(Duck) && Dino.IsDucking)
    {
        Dino.IsDucking = false;
        Dino.Shape.setSize(DINO_SIZE);
        Dino.Shape.setOrigin(DINO_SIZE / 2.f);

        if (Dino.IsOnGround)
        {
            Dino.Shape.setPosition({
                Dino.Shape.getPosition().x,
                GetGroundSnapY(DINO_SIZE.y)
                });
        }
    }

    // REMOVED: auto-jump logic (was causing infinite jumping)

    // Gravity
    Dino.Velocity.y += GRAVITY * dt;
    Dino.Shape.move(Dino.Velocity * dt);

    // Ground collision
    float groundY = GetGroundSnapY(Dino.Shape.getSize().y);

    if (Dino.Shape.getPosition().y >= groundY)
    {
        Dino.Shape.setPosition({ Dino.Shape.getPosition().x, groundY });
        Dino.Velocity.y = 0.f;
        Dino.IsOnGround = true;
    }
    else
    {
        Dino.IsOnGround = false;
    }
}

void Dino::Game::UpdateObstacles()
{
    if (ObstacleSpawnTimer.IsOver())
    {
        EventObstacleSpawn();
        ObstacleSpawnTimer.Restart();
    }

    float dt = MGR.Timer.GetDeltaTime();
    for (auto& O : Obstacles)
        O.Shape.move({ -OBSTACLE_SPEED * dt, 0.f });

    // Remove off-screen
    std::erase_if(Obstacles, [](const Obstacle& O)
        {
            return IsOutsideWindowLeft(O.Shape);
        });
}

void Dino::Game::EventObstacleSpawn()
{
    auto& O = Obstacles.emplace_back();
    O.Shape.setSize(OBSTACLE_SIZE);
    O.Shape.setOrigin(OBSTACLE_SIZE / 2.f);
    O.Shape.setFillColor(OBSTACLE_COLOR);

    float x = EConfig.WindowSize.x + OBSTACLE_SIZE.x;
    float y = Ground.getPosition().y - GROUND_SIZE.y / 2.f - OBSTACLE_SIZE.y / 2.f;
    O.Shape.setPosition({ x, y });
}

void Dino::Game::HandleCollisions()
{
    for (auto& O : Obstacles)
    {
        if (Intersects(Dino.Shape, O.Shape))
        {
            EventDinoHit();
            break;
        }
    }
}

void Dino::Game::EventDinoHit()
{
    MGR.Save.Set(STATS_HIGH_SCORE_KEY, std::max(Stats.Score, Stats.HighScore));
    MGR.Scene.ReloadScene();
}

void Dino::Game::EventJump()
{
    if (!CanJump())
        return;

    Dino.Velocity.y = -DINO_JUMP_FORCE;
    Dino.IsOnGround = false;
}


void Dino::Game::Update()
{
    UpdateDino();
    UpdateObstacles();

    HandleCollisions();

    float elapsed = MGR.Timer.GetElapsedTime() - ScoreStartTime;

    Stats.Score = static_cast<int>(elapsed * 10.f);
    Stats.ScoreText.setString("Score: " + std::to_string(Stats.Score));
}

void Dino::Game::Render() const
{
    MGR.Renderer.Draw(Ground);
    MGR.Renderer.Draw(Dino.Shape);

    for (const auto& O : Obstacles)
        MGR.Renderer.Draw(O.Shape);

    MGR.Renderer.Draw(Stats.ScoreText);
    MGR.Renderer.Draw(Stats.HighScoreText);
}

void Dino::Game::OnEvent(const sf::Event& event)
{
    event.visit([this](const auto& type) { this->HandleEvent(type); });
}

void Dino::Game::HandleEvent(const sf::Event::JoystickButtonPressed& Gamepad)
{
    if (auto button = Input::HardwareToLogic(Gamepad.button, Gamepad.joystickId))
    {
        if (*button == GamepadButton::South && CanJump())
        {
            EventJump();
        }
    }
}

void Dino::Game::HandleEvent(const sf::Event::KeyPressed& Key)
{
    
    if (Key.scancode == sf::Keyboard::Scan::Space || Key.scancode == sf::Keyboard::Scan::Up
        || Key.scancode == sf::Keyboard::Scan::W)
    {
        EventJump();
    }
}
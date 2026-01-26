#include "Runner.h"

Duck::Game::Game(Managers& Mgr)
    : Scene{ Mgr }
    , ObstacleSpawnTimer{ OBSTACLE_SPAWN_INTERVAL }
{
    InitDuck();
    InitGround();
    InitStats();
}

void Duck::Game::InitDuck()
{
    Duck.Shape.setSize(DUCK_SIZE);
    Duck.Shape.setOrigin(DUCK_SIZE / 2.f);
    Duck.Shape.setFillColor(DUCK_COLOR);

    Duck.Shape.setPosition({
        EConfig.WindowSize.x * 0.2f,
        -DUCK_SIZE.y    // ABOVE window
        });

    Duck.Velocity = { 0.f, 0.f };
    Duck.IsOnGround = false;
    Duck.IsDucking = false;
}

float Duck::Game::GetGroundSnapY(float dinoHeight) const
{
    return Ground.getPosition().y
        - GROUND_SIZE.y / 2.f
        - dinoHeight / 2.f;
}

void Duck::Game::InitGround()
{
    Ground.setSize(GROUND_SIZE);
    Ground.setFillColor(GROUND_COLOR);
    Ground.setOrigin(GROUND_SIZE / 2.f);

    // CHANGED: ground moved up (lower 1/3 of screen)
    Ground.setPosition({EConfig.WindowSize.x * 0.5f, EConfig.WindowSize.y * 0.70f});
}

void Duck::Game::InitStats()
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

bool Duck::Game::CanJump()
{
    return Duck.IsOnGround;
}

void Duck::Game::Start()
{
    MGR.Cursor.SetVisibility(false);

    BindInputs();
    Obstacles.clear();
    ObstacleSpawnTimer.Restart();

    Duck.Velocity = { 0.f, 0.f };
    Duck.IsOnGround = false;
    Duck.IsDucking = false;

    Duck.Shape.setSize(DUCK_SIZE);
    Duck.Shape.setOrigin(DUCK_SIZE / 2.f);

    Duck.Shape.setPosition({
        Duck.Shape.getPosition().x,
        -DUCK_SIZE.y   // FORCE fall-in every start
        });

    ScoreStartTime = MGR.Timer.GetElapsedTime();
    Stats.Score = 0;
    Stats.ScoreText.setString("Score: 0");
}


void Duck::Game::BindInputs()
{
    // Duck
    MGR.Input.Bind(Ducked, Input::Keyboard{ sf::Keyboard::Scancode::Down });
    MGR.Input.Bind(Ducked, Input::Keyboard{ sf::Keyboard::Scancode::S });
}

void Duck::Game::UpdateDuck()
{
    const float dt = MGR.Timer.GetDeltaTime();


    if (MGR.Input.Pressed(Ducked) && !Duck.IsDucking)
    {
        Duck.IsDucking = true;
        Duck.Shape.setSize(DUCK_DUCK_SIZE);
        Duck.Shape.setOrigin(DUCK_DUCK_SIZE / 2.f);

        if (Duck.IsOnGround)
        {
            Duck.Shape.setPosition({
                Duck.Shape.getPosition().x,
                GetGroundSnapY(DUCK_DUCK_SIZE.y)
                });
        }
    }
    else if (!MGR.Input.Pressed(Ducked) && Duck.IsDucking)
    {
        Duck.IsDucking = false;
        Duck.Shape.setSize(DUCK_SIZE);
        Duck.Shape.setOrigin(DUCK_SIZE / 2.f);

        if (Duck.IsOnGround)
        {
            Duck.Shape.setPosition({
                Duck.Shape.getPosition().x,
                GetGroundSnapY(DUCK_SIZE.y)
                });
        }
    }

    // REMOVED: auto-jump logic (was causing infinite jumping)

    // Gravity
    Duck.Velocity.y += GRAVITY * dt;
    Duck.Shape.move(Duck.Velocity * dt);

    // Ground collision
    float groundY = GetGroundSnapY(Duck.Shape.getSize().y);

    if (Duck.Shape.getPosition().y >= groundY)
    {
        Duck.Shape.setPosition({ Duck.Shape.getPosition().x, groundY });
        Duck.Velocity.y = 0.f;
        Duck.IsOnGround = true;
    }
    else
    {
        Duck.IsOnGround = false;
    }
}

void Duck::Game::UpdateObstacles()
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

void Duck::Game::EventObstacleSpawn()
{
    auto& O = Obstacles.emplace_back();
    O.Shape.setSize(OBSTACLE_SIZE);
    O.Shape.setOrigin(OBSTACLE_SIZE / 2.f);
    O.Shape.setFillColor(OBSTACLE_COLOR);

    float x = EConfig.WindowSize.x + OBSTACLE_SIZE.x;
    float y = Ground.getPosition().y - GROUND_SIZE.y / 2.f - OBSTACLE_SIZE.y / 2.f;
    O.Shape.setPosition({ x, y });
}

void Duck::Game::HandleCollisions()
{
    for (auto& O : Obstacles)
    {
        if (Intersects(Duck.Shape, O.Shape))
        {
            EventDuckHit();
            break;
        }
    }
}

void Duck::Game::EventDuckHit()
{
    MGR.Save.Set(STATS_HIGH_SCORE_KEY, std::max(Stats.Score, Stats.HighScore));
    MGR.Scene.ReloadScene();
}

void Duck::Game::EventJump()
{
    if (!CanJump())
        return;

    Duck.Velocity.y = -DUCK_JUMP_FORCE;
    Duck.IsOnGround = false;
}


void Duck::Game::Update()
{
    UpdateDuck();
    UpdateObstacles();

    HandleCollisions();

    float elapsed = MGR.Timer.GetElapsedTime() - ScoreStartTime;

    Stats.Score = static_cast<int>(elapsed * 10.f);
    Stats.ScoreText.setString("Score: " + std::to_string(Stats.Score));
}

void Duck::Game::Render() const
{
    MGR.Renderer.Draw(Ground);
    MGR.Renderer.Draw(Duck.Shape);

    for (const auto& O : Obstacles)
        MGR.Renderer.Draw(O.Shape);

    MGR.Renderer.Draw(Stats.ScoreText);
    MGR.Renderer.Draw(Stats.HighScoreText);
}

void Duck::Game::OnEvent(const sf::Event& event)
{
    event.visit([this](const auto& type) { this->HandleEvent(type); });
}

void Duck::Game::HandleEvent(const sf::Event::JoystickButtonPressed& Gamepad)
{
    if (auto button = Input::HardwareToLogic(Gamepad.button, Gamepad.joystickId))
    {
        if (*button == GamepadButton::South && CanJump())
        {
            EventJump();
        }
    }
}

void Duck::Game::HandleEvent(const sf::Event::KeyPressed& Key)
{
    
    if (Key.scancode == sf::Keyboard::Scan::Space || Key.scancode == sf::Keyboard::Scan::Up
        || Key.scancode == sf::Keyboard::Scan::W)
    {
        EventJump();
    }
}
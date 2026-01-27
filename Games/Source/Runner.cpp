#include "Runner.h"

Duck::Game::Game(Managers& Mgr)
    : Scene{ Mgr }
    , ObstacleSpawnTimer{ OBSTACLE_SPAWN_INTERVAL }
{
    InitDuck();
    InitGround();
    InitStats();
    InitAnimations(); // ADDED
}

void Duck::Game::InitDuck()
{
    Duck.Shape.setSize(DUCK_COLLISION_SIZE);
    Duck.Shape.setOrigin({
     DUCK_COLLISION_SIZE.x / 2.f,
     DUCK_COLLISION_SIZE.y
        });

    Duck.Shape.setPosition({
        EConfig.WindowSize.x * 0.2f,
        -DUCK_COLLISION_SIZE.y    // ABOVE window
        });

    Duck.Velocity = { 0.f, 0.f };
    Duck.IsOnGround = false;
    Duck.IsDucking = false;
}

float Duck::Game::GetGroundSnapY() const
{
    // bottom-center: collision origin is bottom, so just align with top of ground
    return Ground.getPosition().y - GROUND_SIZE.y / 2.f;
}

// ============================== OBSTACLES ======================================
float Duck::Game::GetObstacleY(ObstacleType type) const
{
    float groundTop = Ground.getPosition().y - GROUND_SIZE.y / 2.f;

    switch (type)
    {
    case ObstacleType::JumpOnly:
        // On ground
        return groundTop - OBSTACLE_SIZE.y / 2.f;

    case ObstacleType::Either:
        // Mid height
        return groundTop - OBSTACLE_MID_HEIGHT;

    case ObstacleType::DuckOnly:
        // High enough to require ducking
        return groundTop - OBSTACLE_DUCK_HEIGHT;
    }

    return groundTop;
}

void Duck::Game::EventObstacleSpawn()
{
    auto& O = Obstacles.emplace_back();

    O.Shape.setSize(OBSTACLE_SIZE);
    O.Shape.setOrigin(OBSTACLE_SIZE / 2.f);
    O.Shape.setFillColor(OBSTACLE_COLOR);

    // Random obstacle type
    int r = MGR.Randomizer.Random(0, 2);
    if (r == 0)      O.Type = ObstacleType::JumpOnly;
    else if (r == 1) O.Type = ObstacleType::Either;
    else             O.Type = ObstacleType::DuckOnly;

    float x = EConfig.WindowSize.x + OBSTACLE_SIZE.x;
    float y = GetObstacleY(O.Type);

    O.Shape.setPosition({ x, y });
}

void Duck::Game::UpdateObstacles()
{
    if (ObstacleSpawnTimer.IsOver())
    {
        EventObstacleSpawn();
        ObstacleSpawnTimer.Restart();
    }

    float dt = MGR.Timer.GetDeltaTime();

    float t = GameTimer.GetElapsedTime();
    float ramp = (OBSTACLE_MAX_SPEED - OBSTACLE_BASE_SPEED) / OBSTACLE_SPEED_RAMP;

    float speed = OBSTACLE_BASE_SPEED + ramp * t;
    if (speed > OBSTACLE_MAX_SPEED)
        speed = OBSTACLE_MAX_SPEED;

    for (auto& O : Obstacles)
        O.Shape.move({ -speed * dt, 0.f });

    std::erase_if(Obstacles, [](const Obstacle& O)
        {
            return IsOutsideWindowLeft(O.Shape);
        });
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

void Duck::Game::InitAnimations()
{
    RunAnimConfig = {
        MGR.Assets.GetTexture(RUN_SPRITESHEET_FILENAME),
        { 3, 2 },
        0.1f
    };

    DuckRunAnimConfig = {
        MGR.Assets.GetTexture(DUCKING_RUN_SPRITESHEET_FILENAME),
        { 3, 2 },
        0.1f
    };

    DuckAnim.setSize(DUCK_SPRITE_SIZE);
    DuckAnim.Start(RunAnimConfig);
    DuckAnim.setOrigin({ DUCK_SPRITE_SIZE.x / 2.f,DUCK_SPRITE_SIZE.y });
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
    GameTimer.Restart();

    Duck.Velocity = { 0.f, 0.f };
    Duck.IsOnGround = false;
    Duck.IsDucking = false;

    Duck.Shape.setSize(DUCK_COLLISION_SIZE);
    //Duck.Shape.setOrigin(DUCK_COLLISION_SIZE / 2.f);
    // Debug
    Duck.Shape.setFillColor({ 255,0,0,120 });

    Duck.Shape.setPosition({Duck.Shape.getPosition().x, -DUCK_COLLISION_SIZE.y });

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

    // Preserve foot position before resizing
    const float footY = Duck.Shape.getPosition().y;

    // Duck input
    if (MGR.Input.Pressed(Ducked) && !Duck.IsDucking)
    {
        Duck.IsDucking = true;
        Duck.Shape.setSize(DUCK_DUCK_COLLISION_SIZE);
        Duck.Shape.setOrigin({
            DUCK_DUCK_COLLISION_SIZE.x / 2.f,
            DUCK_DUCK_COLLISION_SIZE.y
            });
        Duck.Shape.setPosition({ Duck.Shape.getPosition().x, footY });
    }
    else if (!MGR.Input.Pressed(Ducked) && Duck.IsDucking)
    {
        Duck.IsDucking = false;
        Duck.Shape.setSize(DUCK_COLLISION_SIZE);
        Duck.Shape.setOrigin({
            DUCK_COLLISION_SIZE.x / 2.f,
            DUCK_COLLISION_SIZE.y
            });
        Duck.Shape.setPosition({ Duck.Shape.getPosition().x, footY });
    }

    // Gravity
    Duck.Velocity.y += GRAVITY * dt;
    Duck.Shape.move(Duck.Velocity * dt);

    // Ground collision
    float groundY = GetGroundSnapY();
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

    // Update state
    if (!Duck.IsOnGround)
        Duck.State = Duck.IsDucking ? AnimState::DuckJump : AnimState::Jump;
    else
        Duck.State = Duck.IsDucking ? AnimState::DuckRun : AnimState::Run;
}

void Duck::Game::UpdateDuckAnimation()
{
    if (Duck.State != PrevState)
    {
        switch (Duck.State)
        {
        case AnimState::Run:
            DuckAnim.Start(RunAnimConfig);
            DuckAnim.SetRow(0);
            break;

        case AnimState::DuckRun:
            DuckAnim.Start(DuckRunAnimConfig);
            DuckAnim.SetRow(0);
            break;

        case AnimState::Jump:
            DuckAnim.Start(RunAnimConfig);
            DuckAnim.SetFrame({ 0, 1 });   // static jump pose
            break;

        case AnimState::DuckJump:
            DuckAnim.Start(DuckRunAnimConfig);
            DuckAnim.SetFrame({ 0, 1 });
            break;
        }

        PrevState = Duck.State;
    }

    // Only animate ground states
    if (Duck.State == AnimState::Run || Duck.State == AnimState::DuckRun)
        DuckAnim.Update(MGR.Timer.GetDeltaTime());

    DuckAnim.setPosition({
        Duck.Shape.getPosition().x,
        Duck.Shape.getPosition().y + OFFSET
        });
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
    UpdateDuckAnimation();
    UpdateObstacles();

    HandleCollisions();

    float elapsed = MGR.Timer.GetElapsedTime() - ScoreStartTime;
    Stats.Score = static_cast<int>(elapsed * 10.f);
    Stats.ScoreText.setString("Score: " + std::to_string(Stats.Score));
}

void Duck::Game::Render() const
{
    MGR.Renderer.Draw(Ground);
    MGR.Renderer.Draw(DuckAnim);

    // Debug
    //MGR.Renderer.Draw(Duck.Shape);

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
#include "Animation/SpriteSheets.h"

#include <algorithm>

void Animation::Start(const AnimationConfig& config)
{
    AnimConfig = config;
    setTexture(AnimConfig.TextureSheet);

    CurrentFrame = {};
    ElapsedTime = 0;

    const int cols = *std::ranges::max_element(AnimConfig.FrameCountPerRow);
    const int rows = (int)AnimConfig.FrameCountPerRow.size();

    FrameSize = sf::Vector2i(AnimConfig.TextureSheet->getSize()).componentWiseDiv({ cols, rows });
    setTextureRect({ {}, FrameSize });
}

bool Animation::Update(float deltaTime)
{
    if (IsFinished())
    {
        return false;
    }

    ElapsedTime += deltaTime;
    if (ElapsedTime >= AnimConfig.FrameDuration)
    {
        CurrentFrame.x = (CurrentFrame.x + 1) % AnimConfig.FrameCountPerRow[CurrentFrame.y];
        setTextureRect({ CurrentFrame.componentWiseMul(FrameSize), FrameSize });

        ElapsedTime -= AnimConfig.FrameDuration;
        return true;
    }

    return false;
}

void Animation::SetRow(int rowIndex)
{
    if (CurrentFrame.y != rowIndex)
    {
        CurrentFrame = { 0, rowIndex };
        setTextureRect({ CurrentFrame.componentWiseMul(FrameSize), FrameSize });

        ElapsedTime = 0;
    }
}

bool Animation::IsFinished() const
{
    return !AnimConfig.Looping && CurrentFrame.x == AnimConfig.FrameCountPerRow[CurrentFrame.y] - 1;
}
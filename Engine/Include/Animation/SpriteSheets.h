#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <vector>

struct AnimationConfig
{
    const sf::Texture* TextureSheet;
    std::vector<int> FrameCountPerRow;
    float FrameDuration;
    bool Looping = true;
};

class Animation : public sf::RectangleShape
{

public:
    void Start(const AnimationConfig& Config);
    bool Update(float DeltaTime);

    void SetRow(int RowIndex);
    void SetFrame(sf::Vector2i frame);
    bool IsFinished() const;

private:
    AnimationConfig AnimConfig;
    sf::Vector2i CurrentFrame;
    sf::Vector2i FrameSize;
    float ElapsedTime;
};


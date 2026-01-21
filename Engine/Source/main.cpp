#include <SFML/GpuPreference.hpp>
#include <SFML/System/Sleep.hpp>

#include "Core/Engine.h"

SFML_DEFINE_DISCRETE_GPU_PREFERENCE

int main()
{
    Engine GameEngine;
    while (GameEngine.IsRunning())
    {
        GameEngine.ProcessEvents();
        if (!GameEngine.HasFocus())
        {
            sf::sleep(sf::microseconds(10));
            continue;
        }
        GameEngine.Update();
        GameEngine.Render();
    }
}
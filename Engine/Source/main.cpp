#include <SFML/GpuPreference.hpp>

#include "Core/Engine.h"

SFML_DEFINE_DISCRETE_GPU_PREFERENCE

int main()
{
    Engine GameEngine;
    while (GameEngine.IsRunning())
    {
        GameEngine.ProcessEvents();
        GameEngine.Update();
        GameEngine.Render();
    }
}
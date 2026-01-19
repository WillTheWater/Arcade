#include <algorithm>

#include "SystemManagers/TimeManager.h"
#include "Core/EngineConfig.h"

void TimeManager::Tick()
{
	const sf::Time CurrentTime = DeltaClock.getElapsedTime();
	DeltaTime = std::min(CurrentTime - PreviousTime, EConfig.MaxDeltaTime);
	PreviousTime = CurrentTime;
}

float TimeManager::GetDeltaTime() const
{
	return DeltaTime.asSeconds();
}

float TimeManager::GetElapsedTime() const
{
	return DeltaClock.getElapsedTime().asSeconds();
}

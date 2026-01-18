#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

class TimeManager
{
public:
	float GetDeltaTime() const;
	float GetElapsedTime() const;

private:
	friend class Engine;
	void Tick();

private:
	sf::Clock DeltaClock;
	sf::Time DeltaTime;
	sf::Time PreviousTime;
};
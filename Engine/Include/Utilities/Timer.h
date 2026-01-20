#pragma once

#include <sfml/System/Clock.hpp>

class Timer
{
public:
	Timer(float Duration = 0);

	void Start();
	void Stop();
	void Restart();
	void Reset();
	void SetDuration(float NewDuration);
	float GetDuration() const;
	float GetElapsedTime() const;
	bool IsRunning() const;
	bool IsOver() const;

private:
	sf::Clock Timerhandle;
	float Duration;
};
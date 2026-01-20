#include <sfml/System/Time.hpp>

#include "Utilities/Timer.h"

Timer::Timer(float Duration)
	: Duration{Duration}
{
}

void Timer::Start()
{
	Timerhandle.start();
}

void Timer::Stop()
{
	Timerhandle.stop();
}

void Timer::Restart()
{
	Timerhandle.restart();
}

void Timer::Reset()
{
	Timerhandle.reset()
}

void Timer::SetDuration(float NewDuration)
{
	Duration = NewDuration;
}

float Timer::GetDuration() const
{
	return Duration;
}

float Timer::GetElapsedTime() const
{
	return Timerhandle.getElapsedTime().asSeconds();
}

bool Timer::IsRunning() const
{
	return Timerhandle.isRunning()
}

bool Timer::IsOver() const
{
	return GetElapsedTime() > Duration;
}

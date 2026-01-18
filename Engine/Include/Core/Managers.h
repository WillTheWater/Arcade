#pragma once

#include "SystemManagers/RandomManager.h"
#include "SystemManagers/TimeManager.h"
#include "SystemManagers/SaveManager.h"

struct Managers
{
	RandomManager Randomizer;
	TimeManager Timer;
	SaveManager Save;
};
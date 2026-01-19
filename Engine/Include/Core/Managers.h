#pragma once

#include "SystemManagers/RandomManager.h"
#include "SystemManagers/TimeManager.h"
#include "SystemManagers/SaveManager.h"
#include "SystemManagers/ClipboardManager.h"
#include "SystemManagers/AssetManager.h"

struct Managers
{
	RandomManager Randomizer;
	TimeManager Timer;
	SaveManager Save;
	ClipboardManager Clipboard;
	AssetManager Assets;
};
#pragma once

#include "SystemManagers/RandomManager.h"
#include "SystemManagers/TimeManager.h"
#include "SystemManagers/SaveManager.h"
#include "SystemManagers/ClipboardManager.h"
#include "SystemManagers/AssetManager.h"
#include "SystemManagers/AudioManager.h"
#include "SystemManagers/InputManager.h"

struct Managers
{
	RandomManager Randomizer;
	TimeManager Timer;
	SaveManager Save;
	ClipboardManager Clipboard;
	AssetManager Assets;
	AudioManager Audio;
	InputManager Input;
};
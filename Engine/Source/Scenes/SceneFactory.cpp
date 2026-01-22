#include "Scenes/SceneFactory.h"

// ============================================================================
//                                   GAMES
								#include "MainMenu.h"
								#include "Bounce.h"
								#include "Clicker.h"
// ============================================================================

namespace SceneFactory
{
	Scenes CreateScenes(Managers& Manager)
	{
		Scenes SceneList;

		SceneList.emplace("MainMenu", std::make_unique<Menu::Game>(Manager));
		SceneList.emplace("Bounce", std::make_unique<Bounce::Game>(Manager));
		SceneList.emplace("Clicker", std::make_unique<Clicker::Game>(Manager));

		return SceneList;
	}
}
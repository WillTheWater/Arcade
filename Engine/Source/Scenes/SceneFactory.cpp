#include "Scenes/SceneFactory.h"

// ============================================================================
//                                   GAMES
								#include "MainMenu.h"
								#include "Bounce.h"
								#include "Clicker.h"
								#include "Tetris.h"
								#include "Runner.h"
// ============================================================================

namespace SceneFactory
{
	Scenes CreateScenes(Managers& Manager)
	{
		Scenes SceneList;

		SceneList.emplace("MainMenu", std::make_unique<Menu::Game>(Manager));
		SceneList.emplace("Bounce", std::make_unique<Bounce::Game>(Manager));
		SceneList.emplace("Clicker", std::make_unique<Clicker::Game>(Manager));
		SceneList.emplace("Tetris", std::make_unique<Tetris::Game>(Manager));
		SceneList.emplace("Runner", std::make_unique<Duck::Game>(Manager));

		return SceneList;
	}
}
#include "Scenes/SceneFactory.h"

// ============================================================================
//                                   GAMES
								#include "Bounce.h"
								#include "Clicker.h"
// ============================================================================

namespace SceneFactory
{
	Scenes CreateScenes(Managers& Manager)
	{
		Scenes SceneList;

		SceneList.emplace("Bounce", std::make_unique<Bounce::Game>(Manager));
		SceneList.emplace("Clicker", std::make_unique<Clicker::Game>(Manager));

		return SceneList;
	}
}
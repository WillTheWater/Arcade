#include "Scenes/SceneFactory.h"

// ============================================================================
//                                   GAMES
								#include "Bounce.h"
// ============================================================================

namespace SceneFactory
{
	Scenes CreateScenes(Managers& Manager)
	{
		Scenes SceneList;

		SceneList.emplace("Bounce", std::make_unique<Bounce::Game>(Manager));

		return SceneList;
	}
}
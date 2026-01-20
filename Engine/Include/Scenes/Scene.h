#pragma once

#include <sfml/Window/Event.hpp>

#include "Core/Managers.h"
#include "Scenes/SceneUtilities.h"

class Scene
{
public:
	Scene(Managers& Manager) : MGR{Manager}{}
	virtual ~Scene() = default;

	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Render() const = 0;
	virtual void OnEvent(const sf::Event&) {};
	virtual void OnPause(bool /*Pause*/) {}
	virtual void OnCleanup() {};

protected:
	Managers& MGR;
};
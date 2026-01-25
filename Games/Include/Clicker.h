#pragma once

#include "Types/ClickerTypes.h"

namespace Clicker
{
	class Game : public Scene
	{
	public:
		Game(Managers&);

		void Start();
		void Update();
		void Render() const;
		void OnEvent(const sf::Event&);
		void OnPause(bool);

	private:
		Target ClickTarget;
		Stats ClickStats;

	private:
		void InitTarget();
		void InitStats();

		void BindInput();

		void StartTarget();
		void StartStats();

		void UpdateStats();
		void UpdateTarget();
		void UpdateTargetColor();

		void EventTargetClicked();
		void EventTargetTeleport();
		void EventStatsScoreIncrease();

		bool IsTargetHovered() const;

		void HandleEvent(const sf::Event::JoystickButtonPressed&);
		void HandleEvent(const sf::Event::MouseButtonPressed&);
		void HandleEvent(const auto&) {}
	};
}

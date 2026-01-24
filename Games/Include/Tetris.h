#pragma once

#include "Types/TetrisTypes.h"

namespace Tetris
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
		Board TetrisBoard;
		Piece CurrentPiece;
		Piece NextPiece;
		Stats PlayerStats;
		Timer ActionCooldown;
		Timer FallCooldown;
		Timer DropCooldown;

		// ADDED: new timers
		Timer StartCountdown;
		Timer LevelTimer;

		bool HasStarted = false; // ADDED

	private:
		void InitStats();
		void BindInput();

		void StartGrid();
		void StartNextPiece();
		void StartStats();

		void EventPieceSpawn();
		void EventPieceRotate();
		void EventPieceMove(sf::Vector2i Offset);
		void EventPieceFall();
		void EventPieceDropHard();
		void EventPieceMerge();
		void EventLinesClear();

		// ADDED
		void AdvanceLevel();
		void UpdateScoreOverTime();

		Piece GenerateRandomPiece() const;
		sf::Vector2i GetPointRotated(int Type, int Index, int Rotation) const;
		sf::Vector2f GetBoardOrigin() const;
		bool IsPieceValid(const Piece& PieceToCheck) const;

		void RenderBoard() const;
		void RenderPiece(const Piece& PieceToRender, sf::Vector2f Origin, bool World) const;

		void HandleEvent(const sf::Event::TextEntered&);
		void HandleEvent(const sf::Event::KeyPressed&);
		void HandleEvent(const sf::Event::JoystickButtonPressed&);
		void HandleEvent(const sf::Event::MouseButtonPressed&);
		void HandleEvent(const sf::Event::MouseWheelScrolled&);
		void HandleEvent(const auto&) {}
	};
}
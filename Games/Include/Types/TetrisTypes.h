#pragma once

#include "Config/TetrisConfig.h"

namespace Tetris
{
	enum Action
	{
		MoveRight, MoveLeft, Rotate, SoftDrop
	};

	using Board = std::array<std::array<std::optional<sf::Color>, GRID_WIDTH>, GRID_HEIGHT>;

	struct Piece
	{
		int Type;
		int Rotation;
		sf::Vector2i Position;
		sf::Color Color;
	};

	struct Stats
	{
		int Score;
		sf::Text ScoreText{ GetDefaultFont() };
	};
}
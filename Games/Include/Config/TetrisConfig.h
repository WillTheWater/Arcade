#pragma once

#include "Scenes/Scene.h"

namespace Tetris
{
	const int GRID_WIDTH = 10;
	const int GRID_HEIGHT = 20;

	const float BLOCK_SIZE = 48;

	const int STATS_SCORE_PER_LINE = 100;

	const float ACTION_COOLDOWN_DURATION = 0.2f;
	const float PIECE_FALL_COOLDOWN_DURATION = 0.5f;
	const float SOFT_DROP_COOLDOWN_DURATION = 0.05f;

	const int PIECE_TYPE_COUNT = 7;
	const int PIECE_BLOCK_COUNT = 4;

	const int STATS_TEXT_SIZE = 36;
	const sf::Color OUTLINE_COLOR({ 65,96,157 });
	const float OUTLINE_THICKNESS(3.f);

	const sf::Color GRID_COLOR(125, 125, 125);
	const sf::Color BOARD_COLOR(255, 255, 255, 125);
	const std::array<sf::Color, 5> PIECE_COLORS =
	{
		sf::Color(85, 114, 156),
		sf::Color(189, 125, 193),
		sf::Color(109, 1, 73),
		sf::Color(107, 26, 152),
		sf::Color(162, 9, 121)
	};

	// ADDED: countdown + level tuning
	const float START_COUNTDOWN_DURATION = 3.f;
	const float LEVEL_DURATION = 30.f;
	const float FALL_SPEED_MULTIPLIER = 0.85f;

	// ADDED: save key
	const std::string_view STATS_HIGHSCORE_KEY = "TETRIS_HIGHSCORE";
	const std::string BLOCK_TEXTURE_FILENAME = "block.png";

	const std::vector<std::vector<sf::Vector2i>> PIECE_SHAPES =
	{
		{{0,1},{1,1},{2,1},{3,1}},
		{{1,0},{2,0},{1,1},{2,1}},
		{{1,0},{0,1},{1,1},{2,1}},
		{{1,0},{2,0},{0,1},{1,1}},
		{{0,0},{1,0},{1,1},{2,1}},
		{{0,0},{0,1},{1,1},{2,1}},
		{{2,0},{0,1},{1,1},{2,1}}
	};

	const std::vector<sf::Vector2f> PIECE_PIVOTS =
	{
		{1.5f, 1.5f},
		{1.5f, 0.5f},
		{1.f, 1.f},
		{1.f, 1.f},
		{1.f, 1.f},
		{1.f, 1.f},
		{1.f, 1.f}
	};
}
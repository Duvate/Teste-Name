#pragma once

enum class GameState
{
	MAIN_MENU,
	RUNNING,
	PAUSED,
	GAME_OVER
};
extern GameState gameState;
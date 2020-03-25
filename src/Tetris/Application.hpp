#pragma once
#include "Table.hpp"
#include "FallingPiece.hpp"

enum class GameMode;

class Application
{
	static const std::string SOUNDS_PATH;
	Table table;

	sf::SoundBuffer originalSongBuffer;
	sf::Sound originalSong;

	sf::SoundBuffer rotationFailedBuffer;
	sf::Sound rotationFailed;

	sf::SoundBuffer completedPlaneBuffer;
	sf::Sound completedPlane;

	static constexpr float SPECIAL_EFFECTS_DURATION = 0.75f;
	float specialEffectsBegin = -SPECIAL_EFFECTS_DURATION;

	FallingPiece fallingPiece; // Represents a falling piece which will be copied randomly from the pieces' vector

	// Represents all possible piece's configurations
	std::vector<Piece> trollPieces;
	std::vector<Piece> normalPieces = {
		{{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, TileColor::DarkBlue},
		{{0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0}, TileColor::Purple},
		{{1}, TileColor::Red},
		{{1, 1, 1, 1, 1, 1, 1, 1}, TileColor::Yellow},
		{{0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, TileColor::LightBlue},
		{{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0}, TileColor::Orange},
		{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, TileColor::Yellow},
		{{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, TileColor::Orange},
		{{1, 1, 0, 0, 1, 0, 0, 0}, TileColor::Purple},
		{{0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, TileColor::Green},
		{{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, TileColor::Red},
		{{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}, TileColor::Orange}
	};
	std::vector<Piece> _2DPieces = {
		{{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, TileColor::LightBlue},
		{{1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, TileColor::DarkBlue},
		{{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, TileColor::Orange},
		{{1, 0, 1, 0, 1, 0, 1, 0}, TileColor::Yellow},
		{{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, TileColor::Purple},
		{{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}, TileColor::Green},
		{{0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, TileColor::Red}
	};

	int pieceCounter = 0;
	/*
	 * Desc: block's speed
	 * 1. / TICKS is the amount of time taken for a piece to go down 1 cube
	 * This variable can be increased to make the game harder through time
	 */
	unsigned ticks = 2;
	float timeSinceLastFall = 0.0f; // auxiliary variable

	/*
	 * Parses .3dtetris file and saves all pieces in the pieces vector
	 * Returns true in success / false in failure
	 */
	bool parsePieces();

	void resetFallingPiece();

	/*
	 * Returns if it's game over
	 */
	bool isGameOver();

	void drawFallingPieceProjection();

	void getNextPieceAfterColliding();

public:
	bool init();
	void restart(GameMode mode);
	void handleEvents(const sf::Event& event);
	void togglePause();
	void update();
	void gotoMainMenu();
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	void rotateX_CW();
	void rotateX_CCW();
	void rotateY_CW();
	void rotateY_CCW();
	void rotateZ_CW();
	void rotateZ_CCW();
	void groundPiece();
};
extern std::unique_ptr<Application> application;
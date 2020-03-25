#include "Pch.hpp"
#include "Application.hpp"

#include "Camera.hpp"
#include "GlobalVariables.hpp"
#include "GameState.hpp"
#include "GameMode.hpp"
#include "Renderer.hpp"
#include "Table.hpp"
#include "Window.hpp"
#include "UI.hpp"

// Helper defines
#define TOGGLE(x) (x = !x)

#define PIECE_SIDE static_cast<int>(std::round(std::cbrt(fallingPiece.cubes.size())))

const std::string Application::SOUNDS_PATH = "res/sounds/";
std::unique_ptr<Application> application = std::make_unique<Application>();

static bool StartsWith(const std::string & s, const std::string & begin)
{
	return s.substr(0, begin.size()) == begin;
}

bool Application::parsePieces()
{
	/*
	 * .3dtetris internal format:
	 * New pieces start with "# n c", where n specifies the dimension of the piece (i.e. 2 means 2x2x2, 3 means 3x3x3, etc)
	 * and c is a number in the range [1, 7] representing the color of that piece
	 * Then the following line specifies the piece's format (0 or 1 representing if there's a cube or not)
	 * Order of insertion: width, depth, height (back to front)
	 */
	std::ifstream iFile("res/Pieces.3dtetris");
	if (!iFile.good())
	{
		std::cout << "Couldn't open file Pieces.3dtetris\n";
		return false;
	}
	std::vector<bool> v;
	unsigned n, c;
	bool b;
	std::string line;
	while (std::getline(iFile, line))
	{
		if (StartsWith(line, "#"))
		{
			std::istringstream stream(line.substr(1));
			stream >> n >> c;
			if (static_cast<int>(n) >= std::min(TABLE_WIDTH_TROLL, TABLE_DEPTH_TROLL))
			{
				std::cout << "ERROR: Piece is larger than table";
				return false;
			}
			if (c < 1 || c > 7)
			{
				std::cout << "ERROR: Colors must be in range [1, 7]";
				return false;
			}
			std::getline(iFile, line);
			stream = std::istringstream(line);
			while (stream >> b) v.emplace_back(b);
			if (v.size() != n * n * n)
			{
				std::cout << "ERROR: Piece's size is wrong!";
				return false;
			}
			trollPieces.emplace_back(v, static_cast<TileColor>(c));
			v.clear();
		}
	}
	if (trollPieces.empty())
	{
		std::cout << "ERROR: No pieces found!";
		return false;
	}
	return true;
}

void Application::resetFallingPiece()
{
	/*
	 * The pieces are permuted randomly, and each piece must be placed on the table once
	 * before a second permutation
	 */
	switch (gameMode)
	{
	case GameMode::Tetris3D:
	{
		if (pieceCounter == 0) std::random_shuffle(normalPieces.begin(), normalPieces.end());
		fallingPiece.reset(normalPieces[pieceCounter]);
		pieceCounter = (pieceCounter + 1) % normalPieces.size();
	}
	break;
	case GameMode::Tetris3DTroll:
	{
		if (pieceCounter == 0) std::random_shuffle(trollPieces.begin(), trollPieces.end());
		fallingPiece.reset(trollPieces[pieceCounter]);
		pieceCounter = (pieceCounter + 1) % trollPieces.size();
	}
	break;
	case GameMode::Tetris2D:
	{
		if (pieceCounter == 0) std::random_shuffle(_2DPieces.begin(), _2DPieces.end());
		fallingPiece.reset(_2DPieces[pieceCounter]);
		pieceCounter = (pieceCounter + 1) % _2DPieces.size();
	}
	break;
	}
}

bool Application::isGameOver()
{
	if (fallingPiece.pos.y + PIECE_SIDE > height)
	{
		for (int x = 0; x < PIECE_SIDE; x++)
			for (int y = height - fallingPiece.pos.y; y < PIECE_SIDE; y++)
				for (int z = 0; z < PIECE_SIDE; z++)
					if (fallingPiece(x, y, z)) return true;
		return false;
	}
	return false;
}

void Application::drawFallingPieceProjection()
{
	int oldY = fallingPiece.pos.y;
	while (!fallingPiece.isCollide(table))
	{
		fallingPiece.pos.y--;
	}
	fallingPiece.pos.y++;
	fallingPiece.draw(0.2f);
	fallingPiece.pos.y = oldY;
}

void Application::getNextPieceAfterColliding()
{
	fallingPiece.pos.y++;
	// check for completed planes
	bool scored = table.check(fallingPiece);
	if (isGameOver())
	{
		originalSong.stop();
		gameState = GameState::GAME_OVER;
	}
	else
	{
		if (scored)
		{
			// apply special effects
			completedPlane.play();
			specialEffectsBegin = totalTime;
		}
		resetFallingPiece();
	}
}

bool Application::init()
{
	if (!originalSongBuffer.loadFromFile(SOUNDS_PATH + "originalSong.ogg")) return false;
	originalSong.setBuffer(originalSongBuffer);
	originalSong.setLoop(true);

	if (!rotationFailedBuffer.loadFromFile(SOUNDS_PATH + "rotationFailed.ogg")) return false;
	rotationFailed.setBuffer(rotationFailedBuffer);

	if (!completedPlaneBuffer.loadFromFile(SOUNDS_PATH + "completedPlane.wav")) return false;
	completedPlane.setBuffer(completedPlaneBuffer);

	if (!parsePieces()) return false;
	return true;
}

void Application::restart(GameMode mode)
{
	// update game mode
	gameMode = mode;

	// update table's dimensions
	switch (gameMode)
	{
	case GameMode::Tetris3D: 
	{
		width = TABLE_WIDTH_3D; height = TABLE_HEIGHT_3D; depth = TABLE_DEPTH_3D;
	}
	break;
	case GameMode::Tetris3DTroll: 
	{
		width = TABLE_WIDTH_TROLL; height = TABLE_HEIGHT_TROLL; depth = TABLE_DEPTH_TROLL;
	}
	break;
	case GameMode::Tetris2D:
	{
		width = TABLE_WIDTH_2D; height = TABLE_HEIGHT_2D; depth = TABLE_DEPTH_2D;
	}
	break;
	}

	// restart camera
	camera->distanceToTable = sqrt(pow(static_cast<float>(width) / 2.0f, 2) + pow(static_cast<float>(depth) / 2.0f, 2)) * 6.0f;
	table.updateTableDimensions(width, height, depth);
	camera->pitch = 0.0f;
	camera->yaw = 270.0f;
	camera->zoom = 45.0f;
	camera->pos.y = height / 2.0f;

	// restart song
	originalSong.stop();
	originalSong.play();

	// update gameState
	gameState = GameState::RUNNING;

	// restart score
	score = 0;
	
	// restart pieceCounter
	pieceCounter = 0;

	// get falling piece
	resetFallingPiece();
	table.clear();
}

void Application::handleEvents(const sf::Event& event)
{
	ui->handleEvents(event);
	switch (event.type)
	{
	case sf::Event::KeyPressed:
	{
		switch (event.key.code)
		{
		// Using sf::Keyboard::Return for compatibility with SFML 2.4.2
		case sf::Keyboard::Return: if (gameState == GameState::PAUSED || gameState == GameState::GAME_OVER) gotoMainMenu(); break;
		case sf::Keyboard::R: if (gameState == GameState::PAUSED || gameState == GameState::GAME_OVER) restart(gameMode); break;
		case sf::Keyboard::Q: if (gameState == GameState::PAUSED || gameState == GameState::GAME_OVER) gotoMainMenu(); break;

		case sf::Keyboard::W: if (gameState == GameState::RUNNING) moveUp(); break;
		case sf::Keyboard::S: if (gameState == GameState::RUNNING) moveDown(); break;
		case sf::Keyboard::A: if (gameState == GameState::RUNNING) moveLeft(); break;
		case sf::Keyboard::D: if (gameState == GameState::RUNNING) moveRight(); break;
		case sf::Keyboard::Space: if (gameState == GameState::RUNNING) groundPiece(); break;
		case sf::Keyboard::Num1: if (gameState == GameState::RUNNING) rotateX_CW(); break;
		case sf::Keyboard::Num2: if (gameState == GameState::RUNNING) rotateX_CCW(); break;
		case sf::Keyboard::Num3: if (gameState == GameState::RUNNING) rotateY_CW(); break;
		case sf::Keyboard::Num4: if (gameState == GameState::RUNNING) rotateY_CCW(); break;
		case sf::Keyboard::Num5: if (gameState == GameState::RUNNING) rotateZ_CW(); break;
		case sf::Keyboard::Num6: if (gameState == GameState::RUNNING) rotateZ_CCW(); break;
		// These were for testing purposes
		//case sf::Keyboard::F2: postProcessIndex != N_POSTPROCESSES ? postProcessIndex++ : postProcessIndex = 0; break;
		//case sf::Keyboard::F10: TOGGLE(wireframeMode); break;
		case sf::Keyboard::Escape: togglePause(); break;
		}
	}
	break;
	}
}

void Application::togglePause()
{
	if (gameState == GameState::RUNNING)
	{
		originalSong.pause();
		gameState = GameState::PAUSED;
	}
	else if (gameState == GameState::PAUSED)
	{
		originalSong.play();
		gameState = GameState::RUNNING;
	}
}

void Application::update()
{
	if (gameState == GameState::RUNNING)
	{
		/*
		 * Update here only game logic
		 */
		timeSinceLastFall += dt;
		unsigned int tempTicks;
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) tempTicks = ticks;
		else tempTicks = 20;
		if (timeSinceLastFall >= 1.0f / static_cast<float>(tempTicks))
		{
			timeSinceLastFall = 0.0f;
			fallingPiece.pos.y--;
			if (fallingPiece.isCollide(table)) getNextPieceAfterColliding();
		}

		float offset = totalTime - specialEffectsBegin;
		postProcessIndex = 0;
		if (offset < SPECIAL_EFFECTS_DURATION)
		{
			if (offset < 0.2f * SPECIAL_EFFECTS_DURATION) postProcessIndex = 1;
			else if (offset < 0.4f * SPECIAL_EFFECTS_DURATION) postProcessIndex = 2;
			else if (offset < 0.6f * SPECIAL_EFFECTS_DURATION) postProcessIndex = 3;
			else if (offset < 0.8f * SPECIAL_EFFECTS_DURATION) postProcessIndex = 4;
			else if (offset < SPECIAL_EFFECTS_DURATION) postProcessIndex = 5;
		}
	}
	if (gameState != GameState::MAIN_MENU)
	{
		/*
		 * Render stuff
		 */
		Renderer::PrepareForCubesDrawing();
		table.draw();
		// Now we draw all cubes
		fallingPiece.draw();
		if (gameMode != GameMode::Tetris3DTroll && gameState != GameState::GAME_OVER) drawFallingPieceProjection(); // a versão troll pode dar muito lag já que as peças podem ser grandes
	}
}

void Application::gotoMainMenu()
{
	gameState = GameState::MAIN_MENU;
}

void Application::moveUp()
{
	fallingPiece.pos.z--;
	if (fallingPiece.isCollide(table)) fallingPiece.pos.z++;
}

void Application::moveDown()
{
	fallingPiece.pos.z++;
	if (fallingPiece.isCollide(table)) fallingPiece.pos.z--;
}

void Application::moveLeft()
{
	fallingPiece.pos.x--;
	if (fallingPiece.isCollide(table)) fallingPiece.pos.x++;
}

void Application::moveRight()
{
	fallingPiece.pos.x++;
	if (fallingPiece.isCollide(table)) fallingPiece.pos.x--;
}

void Application::rotateX_CW()
{
	fallingPiece.transposeX();
	fallingPiece.mirrorX_X();
	if (fallingPiece.isCollide(table))
	{
		rotationFailed.play();
		fallingPiece.mirrorX_X();
		fallingPiece.transposeX();
	}
}

void Application::rotateX_CCW()
{
	fallingPiece.transposeX();
	fallingPiece.mirrorX_Y();
	if (fallingPiece.isCollide(table))
	{
		rotationFailed.play();
		fallingPiece.mirrorX_Y();
		fallingPiece.transposeX();
	}
}

void Application::rotateY_CW()
{
	fallingPiece.transposeY();
	fallingPiece.mirrorY_X();
	if (fallingPiece.isCollide(table))
	{
		rotationFailed.play();
		fallingPiece.mirrorY_X();
		fallingPiece.transposeY();
	}
}

void Application::rotateY_CCW()
{
	fallingPiece.transposeY();
	fallingPiece.mirrorY_Y();
	if (fallingPiece.isCollide(table))
	{
		rotationFailed.play();
		fallingPiece.mirrorY_Y();
		fallingPiece.transposeY();
	}
}

void Application::rotateZ_CW()
{
	fallingPiece.transposeZ();
	fallingPiece.mirrorZ_X();
	if (fallingPiece.isCollide(table))
	{
		rotationFailed.play();
		fallingPiece.mirrorZ_X();
		fallingPiece.transposeZ();
	}
}

void Application::rotateZ_CCW()
{
	fallingPiece.transposeZ();
	fallingPiece.mirrorZ_Y();
	if (fallingPiece.isCollide(table))
	{
		rotationFailed.play();
		fallingPiece.mirrorZ_Y();
		fallingPiece.transposeZ();
	}
}

void Application::groundPiece()
{
	while (!fallingPiece.isCollide(table))
	{
		fallingPiece.pos.y--;
	}
	getNextPieceAfterColliding();
}

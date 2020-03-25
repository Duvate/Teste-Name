#include "Pch.hpp"
#include "GlobalVariables.hpp"

#include "Application.hpp"
#include "Window.hpp"
#include "UI.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"
#include "GameState.hpp"
#include "GameMode.hpp"

int main()
{
	// choose seed for a deterministic way to get the pieces
	std::srand(42);

	// Initialize resources
	if (!fontArial.loadFromFile("res/arial.ttf")) return -1;
	if (!ui->init()) return -1;
	if (!application->init()) return -1;

	// Create Window
	window->create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
		"Tetris",
		WINDOW_WIDTH == SCREEN_RESOLUTION_WIDTH ? sf::Style::Fullscreen : sf::Style::Default,
		sf::ContextSettings(24, 8, 16));
	window->setKeyRepeatEnabled(false);
	window->setVerticalSyncEnabled(true);
	sf::Image iconImage;
	if (!iconImage.loadFromFile("res/textures/windowIcon.jpg")) return -1;
	window->setIcon(iconImage.getSize().x, iconImage.getSize().y, iconImage.getPixelsPtr());

	// OpenGL needs a window, so it must be initialized after we create our window
	if (!Renderer::Init()) return -1;

	sf::Event event;
	sf::Clock clock;
	while (window->isOpen())
	{
		while (window->pollEvent(event))
		{
			application->handleEvents(event);
			switch (event.type)
			{
			case sf::Event::Closed:
			{
				window->close(); 
			}
			break;
			case sf::Event::Resized:
			{
				WINDOW_WIDTH = event.size.width;
				WINDOW_HEIGHT = event.size.height;
				firstMouse = true;
			}
			break;
			case sf::Event::LostFocus:
			{
				if (gameState == GameState::RUNNING) application->togglePause();
			}
			break;
			case sf::Event::MouseWheelScrolled:
			{
				camera->processMouseScroll(event.mouseWheelScroll.delta);
			}
			break;
			}
		}
		dt = clock.restart().asSeconds();
		totalTime += dt;

		if (gameState == GameState::RUNNING || gameState == GameState::GAME_OVER)
		{
			window->setMouseCursorVisible(false);
			window->setMouseCursorGrabbed(true);
	
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) camera->pos.y += dt * camera->verticalSpeed;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) camera->pos.y -= dt * camera->verticalSpeed;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				camera->distanceToTable -= dt * camera->stepSpeed;
				if (camera->distanceToTable < 0.0f) camera->distanceToTable = 0.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) camera->distanceToTable += dt * camera->stepSpeed;
		}
		else
		{
			window->setMouseCursorVisible(true);
			window->setMouseCursorGrabbed(false);
		}
	
		Renderer::PrepareForRendering();
		Renderer::DrawTableLayout();
		application->update();
		Renderer::Display();
	}
	// Terminate OpenGL resources.
	Renderer::Terminate();

	// Returns 0 by default
}
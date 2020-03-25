#include "Pch.hpp"
#include "UI.hpp"

#include "GameState.hpp"
#include "Window.hpp"
#include "GlobalVariables.hpp"

std::unique_ptr<UI> ui = std::make_unique<UI>();

bool UI::init()
{
	if (!mainMenu.init()) return false;

	gameOverText.setFont(fontArial);
	gameOverText.setString("Game Over!\nRestart (R)\nMain Menu (Q)");
	gameOverText.setFillColor(sf::Color::White);
	gameOverText.setOutlineColor(sf::Color::Red);
	gameOverText.setOutlineThickness(4.0f);
	gameOverText.setCharacterSize(50);

	scoreText.setFont(fontArial);
	scoreText.setFillColor(sf::Color::Green);
	scoreText.setOutlineColor(sf::Color::Cyan);
	scoreText.setCharacterSize(50);
	scoreText.setPosition(0.0f, static_cast<float>(WINDOW_HEIGHT_MACRO) - 75.0f);

	return true;
}

void UI::handleEvents(const sf::Event& event)
{
	if (gameState == GameState::MAIN_MENU) mainMenu.handleEvents(event);
}

void UI::drawSFML()
{
	switch (gameState)
	{
	case GameState::MAIN_MENU: mainMenu.drawSFML(); break;
	case GameState::PAUSED: pauseMenu.drawSFML(); break;
	case GameState::GAME_OVER: window->draw(gameOverText); break;
	}
	if (gameState != GameState::MAIN_MENU)
	{
		scoreText.setString(std::to_string(score)); window->draw(scoreText);
	}
}
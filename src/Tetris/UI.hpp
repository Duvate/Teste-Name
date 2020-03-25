#pragma once
#include "MainMenu.hpp"
#include "PauseMenu.hpp"

class UI
{
	MainMenu mainMenu;
	PauseMenu pauseMenu;
	sf::Text gameOverText;
	sf::Text scoreText;

public:
	bool init();
	void handleEvents(const sf::Event& event);
	void drawSFML();
};
extern std::unique_ptr<UI> ui;
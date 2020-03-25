#include "Pch.hpp"
#include "MainMenu.hpp"

#include "Application.hpp"
#include "GlobalVariables.hpp"
#include "Window.hpp"
#include "GameMode.hpp"

bool MainMenu::init()
{
	if (!backgroundTexture.loadFromFile("res/textures/menu_background.jpg")) return false;

	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.scale(static_cast<float>(WINDOW_WIDTH_MACRO) / backgroundSprite.getTextureRect().width, static_cast<float>(WINDOW_HEIGHT_MACRO) / backgroundSprite.getTextureRect().height); // Set image size to window size

	text.setFont(fontArial);
	text.setCharacterSize(40);
	text.setFillColor(sf::Color(0x255B8E));
	text.setOutlineColor(sf::Color::Green);
	text.setOutlineThickness(3.0f);

	return true;
}

void MainMenu::handleEvents(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::KeyPressed:
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Up:
		{
			currentInstruction--;
			if (currentInstruction < 0) currentInstruction = instructionMaxIndex;
		}
		break;
		case sf::Keyboard::Down:
		{
			currentInstruction++;
			if (currentInstruction > instructionMaxIndex) currentInstruction = 0;
		}
		break;
		case sf::Keyboard::Return:
		{
			switch (currentInstruction)
			{
			case 0: application->restart(GameMode::Tetris2D); break;
			case 1: application->restart(GameMode::Tetris3D); break;
			case 2: application->restart(GameMode::Tetris3DTroll); break;
			case 3: window->close(); break;
			}
		}
		break;
		case sf::Keyboard::Q:
		{
			window->close();
		}
		break;
		}
	}
	break;
	}
}

void MainMenu::drawSFML()
{
	window->draw(backgroundSprite);

	// Se alguém quiser fazer isso melhor...

	text.setFillColor(sf::Color(0x255B8E));
	text.setString("\tMain Menu (Use arrow keys)");
	window->draw(text);

	if (currentInstruction == 0) text.setFillColor(sf::Color::Red);
	else text.setFillColor(sf::Color(0x255B8E));
	text.setString("\n\n Tetris 2D");
	window->draw(text);

	if (currentInstruction == 1) text.setFillColor(sf::Color::Red);
	else text.setFillColor(sf::Color(0x255B8E));
	text.setString("\n\n\n Tetris 3D");
	window->draw(text);

	if (currentInstruction == 2) text.setFillColor(sf::Color::Red);
	else text.setFillColor(sf::Color(0x255B8E));
	text.setString("\n\n\n\n Tetris 3D Troll");
	window->draw(text);

	if (currentInstruction == 3) text.setFillColor(sf::Color::Red);
	else text.setFillColor(sf::Color(0x255B8E));
	text.setString("\n\n\n\n\n Quit Game (Q)");
	window->draw(text);
}

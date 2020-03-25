#include "Pch.hpp"
#include "PauseMenu.hpp"

#include "Window.hpp"
#include "GlobalVariables.hpp"

PauseMenu::PauseMenu()
{
	text.setFont(fontArial);
	text.setString("\t\tPAUSED\n\nResume (Escape)\nRestart (R)\nMain Menu (Q)");
	text.setCharacterSize(40);
	text.setFillColor(sf::Color(0x255B8E));
	text.setOutlineColor(sf::Color::Green);
	text.setOutlineThickness(3.0f);
}

void PauseMenu::drawSFML()
{
	window->draw(text);
}

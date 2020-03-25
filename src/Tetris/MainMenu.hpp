#pragma once

class MainMenu
{
	static constexpr int instructionMaxIndex = 3;

	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;

	sf::Text text;

	int currentInstruction = 0;

public:
	bool init();
	void handleEvents(const sf::Event& event);
	void drawSFML();
};
#include "Pch.hpp"
#include "Window.hpp"

#if FULLSCREEN == 1
unsigned WINDOW_WIDTH = SCREEN_RESOLUTION_WIDTH;
unsigned WINDOW_HEIGHT = SCREEN_RESOLUTION_HEIGHT;
#elif FULLSCREEN == 0
unsigned WINDOW_WIDTH = WINDOW_WIDTH_MACRO;
unsigned WINDOW_HEIGHT = WINDOW_HEIGHT_MACRO;
#endif

std::unique_ptr<sf::RenderWindow> window = std::make_unique<sf::RenderWindow>();
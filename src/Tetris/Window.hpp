#pragma once

#ifdef _DEBUG
#define FULLSCREEN 0
#define WINDOW_WIDTH_MACRO 800
#define WINDOW_HEIGHT_MACRO 600
#else
#define FULLSCREEN 1
#define WINDOW_WIDTH_MACRO (sf::VideoMode::getDesktopMode().width)
#define WINDOW_HEIGHT_MACRO (sf::VideoMode::getDesktopMode().height)
#endif

const size_t SCREEN_RESOLUTION_WIDTH = sf::VideoMode::getDesktopMode().width;
const size_t SCREEN_RESOLUTION_HEIGHT = sf::VideoMode::getDesktopMode().height;

extern unsigned WINDOW_WIDTH;
extern unsigned WINDOW_HEIGHT;

extern std::unique_ptr<sf::RenderWindow> window;

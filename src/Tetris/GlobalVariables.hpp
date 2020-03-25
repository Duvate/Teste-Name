#pragma once

constexpr int SPAWN_Y_OFFSET = 3;

// Time stuff
extern float totalTime;
extern float dt;

// Fix mouse flickering
extern bool firstMouse;

// View distance (useless for this game)
extern float viewDistance;

extern bool wireframeMode;
extern int postProcessIndex; constexpr int N_POSTPROCESSES = 5; // F2 = Change post-process: 0 == None, 1 == Inversion, 2 == Grayscale, 3 == Edge Detection, 4 == Blur, 5 == Test 

extern sf::Font fontArial;

extern unsigned score;
#pragma once
/*
 * 3D Rendering abstraction layer
 */
#include "TileColor.hpp"

class Renderer
{
	static void RenderSFML();

public:
	/*
	 * Desc: should be called once, after gladLoadGL()
	 * Returns true in success / false in failure
	 */
	static bool Init();

	/*
	 * Desc: should be called once, to clean up OpenGL resources
	 */
	static void Terminate();

	/*
	 * Should be called once per frame before drawing anything
	 */
	static void PrepareForRendering();

	/*
	 * Desc: should be called once before any number of calls to DrawCube
	 */
	static void PrepareForCubesDrawing();

	/*
	 * The tetris volume is 10x10x20.
	 * Desc: by default, pieces will start falling from (table height) + SPAWN_Y_OFFSET, so it takes SPAWN_Y_OFFSET + 1 ticks to get to the first placeable slot
	 * Height starts at 0
	 * @param pos: where to render
	 * @param color: color of the cube to be rendered
	 */
	static void DrawCube(const sf::Vector3i & pos, TileColor color, float alpha = 1.0f);

	/*
	 * Desc: draws table layout
	 */
	static void DrawTableLayout();
	/*
	 * Displays all drawn itens in the screen
	 */
	static void Display();
};
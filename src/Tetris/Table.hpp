#pragma once
#include "TableDimensions.hpp"
#include "TileColor.hpp"

struct FallingPiece;

struct Table
{
	std::vector<TileColor> data; // 0 represents empty slot

	void updateTableDimensions(int width, int height, int depth);
	/*
	 * Places FallingPiece on the table
	 */
	void draw();

	/*
	 * Accesses TileColor in this pos
	 */
	TileColor & operator() (int x, int y, int z);

	/*
	 * Clears table
	 */
	void clear();

	/*
	 * Removes planes completed and cubes from the fallingPiece if they were in a plane completed
	 * Returns if there were planes completed
	 */
	bool check(FallingPiece& fallingPiece);

private:
	void place(const FallingPiece & fallingPiece);
};
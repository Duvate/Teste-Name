#pragma once
#include "TileColor.hpp"

struct Piece
{
	/*
	 * Saves if a certain position has a cube or not
	 * The cube root of cubes.size() determines the size of the piece
	 */
	std::vector<bool> cubes;
	/*
	 * Accesses position (x, y, z) of the cube 3D array
	 */
	const bool operator() (const int x, const int y, const int z) const;

	void setCube(int x, int y, int z, bool enable);

	/*
	 * Swaps 2 blocks of the cubes vector given its indexes
	 */
	void swapBlocks(const int index0, const int index1);
	TileColor color; // piece's color
	Piece();
	Piece(const std::vector<bool> & cubes, TileColor color);
};
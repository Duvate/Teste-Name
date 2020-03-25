#include "Pch.hpp"
#include "FallingPiece.hpp"

#include "Renderer.hpp"
#include "TableDimensions.hpp"
#include "GlobalVariables.hpp"
#include "Table.hpp"

#define PIECE_SIDE static_cast<int>(std::round(std::cbrt(cubes.size())))
#define BLOCK_AT(x, y, z) ((x) * PIECE_SIDE * PIECE_SIDE + (y) * PIECE_SIDE + (z))

bool FallingPiece::isCollide(Table & table)
{
	for (int x = 0; x < PIECE_SIDE; x++)
		for (int y = 0; y < PIECE_SIDE; y++)
			for (int z = 0; z < PIECE_SIDE; z++)
				if ((*this)(x, y, z))
				{
					// We check if the square is not inside the table first
					if (pos.y + y < 0 || pos.x + x < 0 || pos.x + x >= width || pos.z + z < 0 || pos.z + z >= depth) return true;
					if (pos.y + y >= height) continue;
					if (table(x + pos.x, y + pos.y, z + pos.z) != TileColor::None) return true;
				}
	return false;
}

void FallingPiece::reset(const Piece & piece)
{
	cubes = piece.cubes;
	color = piece.color;
	pos = sf::Vector3i((PIECE_SIDE <= width) ? (width - PIECE_SIDE) / 2 : 0,
					   height + SPAWN_Y_OFFSET,
					   (PIECE_SIDE <= depth) ? (depth - PIECE_SIDE) / 2 : 0);
}

void FallingPiece::draw(float alpha)
{
	for (int x = 0; x < PIECE_SIDE; x++)
		for (int y = 0; y < PIECE_SIDE; y++)
			for (int z = 0; z < PIECE_SIDE; z++)
			{
				if ((*this)(x, y, z))
				{
					sf::Vector3i cubePos(pos + sf::Vector3i(x, y, z));
					Renderer::DrawCube(cubePos, color, alpha);
				}
			}
}

void FallingPiece::transposeX()
{
	for (int x = 0; x < PIECE_SIDE; x++)
		for (int y = 0; y < PIECE_SIDE - 1; y++)
			for (int z = y + 1; z < PIECE_SIDE; z++)
				swapBlocks(BLOCK_AT(x, y, z), BLOCK_AT(x, z, y));
}

void FallingPiece::transposeY()
{
	for (int y = 0; y < PIECE_SIDE; y++)
		for (int x = 0; x < PIECE_SIDE - 1; x++)
			for (int z = x + 1; z < PIECE_SIDE; z++)
				swapBlocks(BLOCK_AT(x, y, z), BLOCK_AT(z, y, x));
}

void FallingPiece::transposeZ()
{
	for (int z = 0; z < PIECE_SIDE; z++)
		for (int x = 0; x < PIECE_SIDE - 1; x++)
			for (int y = x + 1; y < PIECE_SIDE; y++)
				swapBlocks(BLOCK_AT(x, y, z), BLOCK_AT(y, x, z));
}

void FallingPiece::mirrorX_X()
{
	for (int x = 0; x < PIECE_SIDE; x++)
		for (int y = 0; y < PIECE_SIDE / 2; y++)
			for (int z = 0; z < PIECE_SIDE; z++)
				swapBlocks(BLOCK_AT(x, y, z), BLOCK_AT(x, PIECE_SIDE - y - 1, z));
}

void FallingPiece::mirrorX_Y()
{
	for (int x = 0; x < PIECE_SIDE; x++)
		for (int y = 0; y < PIECE_SIDE; y++)
			for (int z = 0; z < PIECE_SIDE / 2; z++)
				swapBlocks(BLOCK_AT(x, y, z), BLOCK_AT(x, y, PIECE_SIDE - z - 1));
}

void FallingPiece::mirrorY_X()
{
	for (int x = 0; x < PIECE_SIDE / 2; x++)
		for (int y = 0; y < PIECE_SIDE; y++)
			for (int z = 0; z < PIECE_SIDE; z++)
				swapBlocks(BLOCK_AT(x, y, z), BLOCK_AT(PIECE_SIDE - x - 1, y, z));
}

void FallingPiece::mirrorY_Y()
{
	for (int x = 0; x < PIECE_SIDE; x++)
		for (int y = 0; y < PIECE_SIDE; y++)
			for (int z = 0; z < PIECE_SIDE / 2; z++)
				swapBlocks(BLOCK_AT(x, y, z), BLOCK_AT(x, y, PIECE_SIDE - z - 1));
}

void FallingPiece::mirrorZ_X()
{
	for (int x = 0; x < PIECE_SIDE / 2; x++)
		for (int y = 0; y < PIECE_SIDE; y++)
			for (int z = 0; z < PIECE_SIDE; z++)
				swapBlocks(BLOCK_AT(x, y, z), BLOCK_AT(PIECE_SIDE - x - 1, y, z));
}

void FallingPiece::mirrorZ_Y()
{
	for (int x = 0; x < PIECE_SIDE; x++)
		for (int y = 0; y < PIECE_SIDE / 2; y++)
			for (int z = 0; z < PIECE_SIDE; z++)
				swapBlocks(BLOCK_AT(x, y, z), BLOCK_AT(x, PIECE_SIDE - y - 1, z));
}

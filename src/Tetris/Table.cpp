#include "Pch.hpp"
#include "Table.hpp"

#include "FallingPiece.hpp"
#include "Renderer.hpp"
#include "GlobalVariables.hpp"

#define PIECE_SIDE static_cast<int>(std::round(std::cbrt(fallingPiece.cubes.size())))

void Table::updateTableDimensions(int width, int height, int depth)
{
	data.resize(width * height * depth);
}

void Table::place(const FallingPiece & fallingPiece)
{
	for (int x = 0; x < PIECE_SIDE; x++)
		for (int y = 0; y < PIECE_SIDE; y++)
			for (int z = 0; z < PIECE_SIDE; z++)
				if (fallingPiece(x, y, z) && fallingPiece.pos.y + y < height) (*this)(fallingPiece.pos.x + x, fallingPiece.pos.y + y, fallingPiece.pos.z + z) = fallingPiece.color;
}

void Table::draw()
{
	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
			for (int z = 0; z < depth; z++)
				Renderer::DrawCube(sf::Vector3i(x, y, z), (*this)(x, y, z));
}

TileColor & Table::operator()(int x, int y, int z)
{
	return data[x * height * depth + y * depth + z];
}

void Table::clear()
{
	std::fill(data.begin(), data.end(), TileColor::None);
}

bool Table::check(FallingPiece& fallingPiece)
{
	place(fallingPiece);
	int planesCleared = 0;
	bool comparison1 = fallingPiece.pos.y > height - PIECE_SIDE; // so we don't do this every time
	// checking for areas in constant y
	for (int y = height - 1; y >= 0; y--) // iterate from top to bottom
	{
		for (int x = 0; x < width; x++)
			for (int z = 0; z < depth; z++)
				if ((*this)(x, y, z) == TileColor::None) goto didntCompleteThisPlane;

		planesCleared++;
		// table's completed plane
		for (int i = 0; i < width; i++)
			for (int k = 0; k < depth; k++)
				(*this)(i, y, k) = TileColor::None;

		// fallingPiece's completed plane
		for (int x = 0; x < PIECE_SIDE; x++)
			for (int z = 0; z < PIECE_SIDE; z++)
				fallingPiece.setCube(x, y - fallingPiece.pos.y, z, false);

		// bringing top planes down
		// table
		// all planes except top one
		for (int i = 0; i < width; i++)
			for (int j = y + 1; j < height; j++)
				for (int k = 0; k < depth; k++)
					(*this)(i, j - 1, k) = (*this)(i, j, k);
		// finish pushing down top plane
		for (int i = 0; i < width; i++)
			for (int k = 0; k < depth; k++)
			{
				// treat case when it has to be filled with part of the fallingPiece
				if (comparison1 &&
					i >= fallingPiece.pos.x && i < fallingPiece.pos.x + PIECE_SIDE &&
					k >= fallingPiece.pos.z && k < fallingPiece.pos.z + PIECE_SIDE &&
					fallingPiece(i - fallingPiece.pos.x, height - fallingPiece.pos.y, k - fallingPiece.pos.z)
					) (*this)(i, height - 1, k) = fallingPiece.color;
				else (*this)(i, height - 1, k) = TileColor::None;
			}

		// fallingPiece
		// all planes except top one
		for (int i = 0; i < PIECE_SIDE; i++)
			for (int j = y + fallingPiece.pos.y; j < PIECE_SIDE - 1; j++)
				for (int k = 0; k < PIECE_SIDE; k++)
					fallingPiece.setCube(i, j, k, fallingPiece(i, j + 1, k));
		// finish pushing down top plane
		for (int i = 0; i < PIECE_SIDE; i++)
			for (int k = 0; k < PIECE_SIDE; k++)
				fallingPiece.setCube(i, PIECE_SIDE - 1, k, false);

	didntCompleteThisPlane:;
	}
	float multiplier = 1.0f;
	// calculating score multiplier (when multiple planes are cleared at once)
	for (int i = 2; i <= planesCleared; i++)
		multiplier *= 1.5;
	score += static_cast<unsigned int>(planesCleared * width * depth * multiplier);
	return planesCleared;
}

#pragma once
#include "Piece.hpp"

struct Table;

struct FallingPiece : Piece
{
	/*
	 * Represents the 3d position of the FallingPiece
	 * By default, it will be the piece's origin at (0, 0, 0)
	 */
	sf::Vector3i pos;

	/*
	 * Returns if the piece is in an invalid position (colliding with something on the table)
	 */
	bool isCollide(Table& table);

	/*
	 * Resets initial values for falling piece
	 */
	void reset(const Piece & piece);

	/*
	 * Draws each cube of the piece with specified texture alpha
	 */
	void draw(float alpha = 1.0f);

	void transposeX();
	void transposeY();
	void transposeZ();
	void mirrorX_X();
	void mirrorX_Y();
	void mirrorY_X();
	void mirrorY_Y();
	void mirrorZ_X();
	void mirrorZ_Y();
};
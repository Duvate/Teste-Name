#include "Pch.hpp"
#include "Piece.hpp"

#define PIECE_SIDE static_cast<int>(std::round(std::cbrt(cubes.size())))

const bool Piece::operator()(const int x, const int y, const int z) const
{
	return cubes[x * PIECE_SIDE * PIECE_SIDE + y * PIECE_SIDE + z];
}

void Piece::setCube(int x, int y, int z, bool enable)
{
	cubes[x * PIECE_SIDE * PIECE_SIDE + y * PIECE_SIDE + z] = enable;
}

void Piece::swapBlocks(const int index0, const int index1)
{
	bool temp = cubes[index0];
	cubes[index0] = cubes[index1];
	cubes[index1] = temp;
	//std::swap(cubes[index0], cubes[index1]);
}

Piece::Piece() :
	color(TileColor::None)
{
}

Piece::Piece(const std::vector<bool> & cubes, TileColor color) :
	cubes(cubes),
	color(color)
{

}

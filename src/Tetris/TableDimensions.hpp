#pragma once

constexpr int TABLE_WIDTH_3D = 8;
constexpr int TABLE_HEIGHT_3D = 20;
constexpr int TABLE_DEPTH_3D = 8;

constexpr int TABLE_WIDTH_TROLL = 20;
constexpr int TABLE_HEIGHT_TROLL = 40;
constexpr int TABLE_DEPTH_TROLL = 20;

constexpr int TABLE_WIDTH_2D = 10;
constexpr int TABLE_HEIGHT_2D = 20;
constexpr int TABLE_DEPTH_2D = 1;

// represents the table's current dimensions (used in many places including the renderer)
extern int width;
extern int height;
extern int depth;
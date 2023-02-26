#pragma once

#include <stdint.h>


struct TileCoords {
	int16_t x, y;

	TileCoords(int x = -1, int y = -1) : x(x), y(y) {};
};
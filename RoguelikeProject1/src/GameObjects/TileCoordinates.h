#pragma once

#include <stdint.h>


struct TileCoordinates {
	int16_t x, y;

	TileCoordinates(int x = -1, int y = -1) : x(x), y(y) {};
};
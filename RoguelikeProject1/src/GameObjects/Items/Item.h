#pragma once

#include "../TileGraphics.h"

class Item {
private:
	TileDisplay display;

public:
	TileDisplay* getDisplay();
};
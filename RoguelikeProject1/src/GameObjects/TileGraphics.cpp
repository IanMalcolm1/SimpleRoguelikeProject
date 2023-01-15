#include "TileGraphics.h"

void MyColor::set(uint8_t red, uint8_t green, uint8_t blue) {
	r = red;
	g = green;
	b = blue;
}

void TileDisplay::copy(TileDisplay* display) {
	symbol = display->symbol;
	symbolColor = display->symbolColor;
	backColor = display->backColor;
}

#ifndef TILEGRAPHICS_H
#define	TILEGRAPHICS_H

#include "../Enums/AsciiSymbols.h"

//Unsigned 8-bit integers for red, green, and blue
struct MyColor {
	uint8_t r;
	uint8_t g;
	uint8_t b;

	MyColor(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) : r(r), g(g), b(b) {};

	void set(uint8_t red, uint8_t green, uint8_t blue);
};


//Display info for tiles. Meant to take up minimal space possible.
struct TileDisplay {
	uint8_t symbol;
	MyColor symbolColor;
	MyColor backColor;

	TileDisplay(uint8_t symbol = ASYM_SMILE_THIN, MyColor symbolColor = MyColor(255,255,255),
		MyColor backColor = MyColor()) : symbol(symbol), symbolColor(symbolColor),
		backColor(backColor) {};

	void copy(TileDisplay* display);
};

#endif

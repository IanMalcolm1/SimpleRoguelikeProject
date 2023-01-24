#pragma once

#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "../../GraphicsThings/GameText/GameText.h"


struct TextRenderingSpecifications {
	int fontSize;
	int fontSizePixels;
	int margin;
	int lineSpacing;
	int messageSpacing;
	int maxLettersPerLine;
	int scrollOffset;

	TextRenderingSpecifications(int fontSize = 2, int margin = 16) : fontSize(fontSize),
		fontSizePixels(8 * fontSize), margin(margin), lineSpacing(fontSizePixels / 4),
		messageSpacing(fontSizePixels / 2), scrollOffset(0), maxLettersPerLine(0) {};

	void modifyFontSize(int modification);
};


class TextRenderer {
private:
	SDL_Renderer* renderer;
	SDL_Texture* spritesheet;

	std::pair<std::string, int> makeFormattedMessage(int maxLettersPerLine, std::string message);

public:
	TextRenderer() : renderer(NULL), spritesheet(NULL) {};

	void initialize(SDL_Renderer* renderer, SDL_Texture* spriteSheet);

	void renderMessage(TextRenderingSpecifications specs, GameText message, SDL_Rect& destinationRect);
};
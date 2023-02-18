#pragma once

#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "../../GraphicsThings/GameText/GameText.h"


struct TextRenderingSpecs {
	int fontSize;
	int fontSizePixels;
	int margin;
	int lineSpacing;
	int messageSpacing;
	int maxLettersPerLine;
	int scrollOffset;

	TextRenderingSpecs(int fontSize = 2, int margin = 16) : fontSize(fontSize),
		fontSizePixels(8 * fontSize), margin(margin), lineSpacing(fontSizePixels / 4),
		messageSpacing(fontSizePixels / 2), scrollOffset(0), maxLettersPerLine(0) {};

	void modifyFontSize(int modification);
};


class TextRenderer {
private:
	SDL_Renderer* renderer;
	SDL_Texture* spritesheet;

	void renderFormattedText(TextRenderingSpecs& specs, std::string& fText, GameText& gameText, int startY);

public:
	TextRenderer() : renderer(NULL), spritesheet(NULL) {};

	void initialize(SDL_Renderer* renderer, SDL_Texture* spriteSheet);

	//Renders a GameText object down from startY. Returns the ending y coordinate.
	int renderGameTextDown(TextRenderingSpecs& specs, GameText& gameText, int startY);
	//Renders a GameText object up from startY. Returns the ending y coordinate.
	int renderGameTextUp(TextRenderingSpecs& specs, GameText& gameText, int startY);

	//Renders a formatted GameText (a string and a height) down from startY. Returns ending y coordinate.
	int renderFormattedTextDown(TextRenderingSpecs& specs, std::pair<std::string, int>& fText, GameText& gameText, int startY);
	//Renders a formatted GameText (a string and a height) up from startY. Returns ending y coordinate.
	int renderFormattedTextUp(TextRenderingSpecs& specs, std::pair<std::string, int>& fText, GameText& gameText, int startY);

	std::pair<std::string, int> formatGameText(TextRenderingSpecs& specs, GameText& gameText);
};
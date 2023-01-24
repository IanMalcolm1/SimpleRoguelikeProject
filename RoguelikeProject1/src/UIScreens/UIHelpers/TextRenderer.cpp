#include "TextRenderer.h"
#include "../../Enums/AsciiSymbols.h"


void TextRenderer::initialize(SDL_Renderer* renderer, SDL_Texture* spritesheet) {
	this->renderer = renderer;
	this->spritesheet = spritesheet;
}


std::pair<std::string, int> TextRenderer::makeFormattedMessage(int maxLettersPerLine, std::string message) {
	int lines = 1;
	int index = maxLettersPerLine - 1;

	while (index < (int)message.size()) {

		if (message[index] == ASYM_SPACE) {
			message[index] = '\n';
			lines++;
		}

		else if (message[index + 1] == ASYM_SPACE) {
			index++;
			message[index] = '\n';
			lines++;
		}

		else {
			int prevIndex = index;
			while (index > 0) {
				index--;
				if (message[index] == ASYM_SPACE) {
					message[index] = '\n';
					lines++;
					break;
				}
				else if (prevIndex - index == maxLettersPerLine) {
					message.insert(message.begin() + prevIndex + 1, '\n');
					lines++;
					index = prevIndex + 1;
					break;
				}
			}
			if (index == 0) {
				message.insert(message.begin() + prevIndex + 1, '\n');
				lines++;
				index = prevIndex + 1;
			}
		}

		index += maxLettersPerLine;
	}

	return std::make_pair(message, lines);
}


void TextRenderer::renderMessage(TextRenderingSpecifications specs, GameText message, SDL_Rect& destinationRect) {

	SDL_Rect sourceRect;
	sourceRect.w = sourceRect.h = 8;

	std::pair<std::string, int> messageTextAndNumberOfLines;
	messageTextAndNumberOfLines = makeFormattedMessage(specs.maxLettersPerLine, message.getText());

	std::string formattedText = messageTextAndNumberOfLines.first;
	int lines = messageTextAndNumberOfLines.second;

	int unformattedIndex = 0;
	MyColor currentColor;

	destinationRect.x = specs.margin;

	destinationRect.y -= lines * specs.fontSizePixels + (lines - 1) * specs.lineSpacing;

	int startY = destinationRect.y;

	for (int i = 0; i < formattedText.size(); i++) {
		char test = formattedText[i];

		if (test == '\n') {
			destinationRect.y += specs.fontSizePixels + specs.lineSpacing;
			destinationRect.x = specs.margin;
			continue;
		}

		sourceRect.x = formattedText[i] % 16 * 8;
		sourceRect.y = formattedText[i] / 16 * 8;

		currentColor = message.getColorAtIndex(unformattedIndex);

		SDL_SetTextureColorMod(spritesheet, currentColor.r, currentColor.g, currentColor.b);
		SDL_RenderCopy(renderer, spritesheet, &sourceRect, &destinationRect);

		destinationRect.x += specs.fontSizePixels;

		unformattedIndex++;
	}

	destinationRect.y = startY - specs.messageSpacing - specs.lineSpacing;
}


void TextRenderingSpecifications::modifyFontSize(int modification) {
	int temporary = fontSize + modification;

	if (temporary < 1 || temporary > 10) {
		return;
	}

	fontSize += modification;
	fontSizePixels = 8 * fontSize;
	lineSpacing = fontSizePixels / 4;
	messageSpacing = fontSizePixels / 2;
}

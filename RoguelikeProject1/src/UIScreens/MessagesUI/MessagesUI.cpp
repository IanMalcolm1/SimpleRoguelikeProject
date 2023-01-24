#include "MessagesUI.h"


void MessagesUI::initialize(SDL_Renderer* renderer, SDL_Texture* spritesheet) {
	this->renderer = renderer;
	this->spritesheet = spritesheet;

	textRenderer.initialize(renderer, spritesheet);
}


void MessagesUI::render(SDL_Rect& viewport) {
	SDL_RenderSetViewport(renderer, &viewport);

	if (messageLog->getRecentMessages()->size() == 0) {
		return;
	}

	textSpecs.maxLettersPerLine = (viewport.w - 2 * textSpecs.margin) / textSpecs.fontSizePixels;

	SDL_Rect destinationRect;
	destinationRect.w = textSpecs.fontSizePixels;
	destinationRect.h = textSpecs.fontSizePixels;
	destinationRect.x = textSpecs.margin;
	destinationRect.y = viewport.h - (textSpecs.margin) + textSpecs.scrollOffset;

	std::vector<GameText>* recentMessages = messageLog->getRecentMessages();

	for (int i = 0; i < recentMessages->size(); i++) {
		if (destinationRect.y < -textSpecs.fontSizePixels) {
			break;
		}

		textRenderer.renderMessage(textSpecs, recentMessages->at(i), destinationRect);
	}
}

void MessagesUI::changeScrollOffset(int offset) {
	textSpecs.scrollOffset += 4 * offset;
	textSpecs.scrollOffset = (textSpecs.scrollOffset > 0) ? textSpecs.scrollOffset : 0;
}

void MessagesUI::modifyFontSize(int modification) {
	textSpecs.modifyFontSize(modification);
}
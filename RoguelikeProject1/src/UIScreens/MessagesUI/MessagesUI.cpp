#include "MessagesUI.h"


void MessagesUI::initialize(SDL_Renderer* renderer, SDL_Texture* spritesheet) {
	this->renderer = renderer;
	this->spritesheet = spritesheet;

	textRenderer.initialize(renderer, spritesheet);
}

void MessagesUI::makeFormattedMessages() {
	textSpecs.maxLettersPerLine = (currViewport.w - 2 * textSpecs.margin) / textSpecs.fontSizePixels;

}



void MessagesUI::render(SDL_Rect& viewport) {
	SDL_RenderSetViewport(renderer, &viewport);

	if (messageLog->getRecentMessages()->size() == 0) {
		return;
	}

	//if saved viewport specs are dirty, remake the formatted messages
	if (currViewport.w != viewport.w || currViewport.h != viewport.h) {
		currViewport.w = viewport.w;
		currViewport.h = viewport.h;
		makeFormattedMessages();
	}

	if (messageLog->getRecentMessages()->size() != formattedMessages.size()) {
		makeFormattedMessages();
	}

	SDL_Rect destinationRect;
	destinationRect.w = textSpecs.fontSizePixels;
	destinationRect.h = textSpecs.fontSizePixels;
	destinationRect.x = textSpecs.margin;
	destinationRect.y = viewport.h - (textSpecs.margin) + textSpecs.scrollOffset;

	std::vector<GameText>* recentMessages = messageLog->getRecentMessages();

	for (int i = recentMessages->size()-1; i >= 0; i--) {
		if (destinationRect.y < -textSpecs.fontSizePixels) {
			break;
		}

		textRenderer.renderMessage(textSpecs, recentMessages->at(i), destinationRect);
	}
}

void MessagesUI::processMouseScroll(int offset, bool ctrlDown) {
	if (!ctrlDown) {
		textSpecs.scrollOffset += offset * textSpecs.fontSizePixels;
		textSpecs.scrollOffset = (textSpecs.scrollOffset > 0) ? textSpecs.scrollOffset : 0;
	}
	else {
		textSpecs.modifyFontSize(offset);
		makeFormattedMessages();
	}
}
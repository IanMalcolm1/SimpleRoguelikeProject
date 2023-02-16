#include "MessagesUI.h"


void MessagesUI::initialize(SDL_Renderer* renderer, SDL_Texture* spritesheet) {
	this->renderer = renderer;
	this->spritesheet = spritesheet;

	textRenderer.initialize(renderer, spritesheet);
}


void MessagesUI::render(const SDL_Rect viewport) {
	SDL_RenderSetViewport(renderer, &viewport);

	if (messageLog->getRecentMessages()->size() == 0) {
		return;
	}

	textSpecs.maxLettersPerLine = (viewport.w - 2 * textSpecs.margin) / textSpecs.fontSizePixels;


	int startY = viewport.h - textSpecs.margin + textSpecs.scrollOffset;

	std::vector<GameText>* recentMessages = messageLog->getRecentMessages();

	for (int i = recentMessages->size()-1; i >= 0; i--) {
		if (startY < 0) {
			break;
		}

		startY = textRenderer.renderMessageUp(textSpecs, recentMessages->at(i), startY, viewport.h);
		startY -= textSpecs.messageSpacing;
	}
	
	startY += textSpecs.messageSpacing;
	if (startY > textSpecs.margin) {
		textSpecs.scrollOffset += (textSpecs.margin-startY);
	}

}

void MessagesUI::processMouseScroll(int offset, bool ctrlDown) {
	if (!ctrlDown) {
		textSpecs.scrollOffset += offset * textSpecs.fontSizePixels;
		textSpecs.scrollOffset = (textSpecs.scrollOffset < 0) ? 0 : textSpecs.scrollOffset;
	}
	else {
		textSpecs.modifyFontSize(offset);
	}
}
#include "MessagesUI.h"


void MessagesUI::initialize(SDL_Renderer* renderer, SDL_Texture* spritesheet) {
	this->renderer = renderer;
	this->spritesheet = spritesheet;

	textRenderer.initialize(renderer, spritesheet);
}


void MessagesUI::render(const SDL_Rect viewport) {
	SDL_RenderSetViewport(renderer, &viewport);

	std::vector<GameText>* recentMessages = messageLog->getRecentMessages();

	if (recentMessages->size() == 0) {
		return;
	}

	if (viewport.w != currWidth) {
		currWidth = viewport.w;
		makeFormattedMessages();
	}
	else if (recentMessages->size() != formattedMsgs.size()) {
		makeFormattedMessages();
	}
	
	int startY = viewport.h - textSpecs.margin + textSpecs.scrollOffset;

	for (int i = recentMessages->size()-1; i >= 0; i--) {
		if (startY < 0) {
			break;
		}

		startY = textRenderer.renderGameTextUp(textSpecs, recentMessages->at(i), startY, viewport.h);
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
		makeFormattedMessages();
	}
}

void MessagesUI::makeFormattedMessages() {
	std::vector<GameText>* rawMessages = messageLog->getRecentMessages();

	textSpecs.maxLettersPerLine = (currWidth - 2 * textSpecs.margin) / textSpecs.fontSizePixels;

	formattedMsgs.clear();

	for (int i = 0; i < rawMessages->size(); i++) {
		formattedMsgs.push_back(textRenderer.formatGameText(textSpecs, rawMessages->at(i)));
	}
}
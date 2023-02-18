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

	if (viewport.h != viewportHeight) {
		viewportHeight = viewport.h;
	}

	if (viewport.w != viewportWidth) {
		viewportWidth = viewport.w;
		makeFormattedMessages();
	}
	else if (recentMessages->size() != formattedMsgs.size()) {
		makeFormattedMessages();
	}
	
	int startY = viewport.h - textSpecs.margin + textSpecs.scrollOffset;

	for (int i = formattedMsgs.size()-1; i >= 0; i--) {
		if (startY < 0) {
			break;
		}

		if (startY - formattedMsgs.at(i).second > viewportHeight) {
			startY -= (formattedMsgs.at(i).second + textSpecs.messageSpacing);
			continue;
		}

		startY = textRenderer.renderFormattedTextUp(textSpecs, formattedMsgs.at(i), recentMessages->at(i), startY);
		startY -= textSpecs.messageSpacing;
	}
}

void MessagesUI::processMouseScroll(int offset, bool ctrlDown) {
	if (ctrlDown) {
		textSpecs.modifyFontSize(offset);
		makeFormattedMessages();
	}
	else {
		textSpecs.scrollOffset += offset * textSpecs.fontSizePixels;
	}

	if (textSpecs.scrollOffset < 0 || totalHeight < viewportHeight - 2 * textSpecs.margin) {
		textSpecs.scrollOffset = 0;
	}
	else if (totalHeight < textSpecs.scrollOffset + viewportHeight - textSpecs.margin) {
		textSpecs.scrollOffset = totalHeight - (viewportHeight - 2*textSpecs.margin);
	}
}

void MessagesUI::makeFormattedMessages() {
	std::vector<GameText>* recentMessages = messageLog->getRecentMessages();
	int i, entriesAdded;

	textSpecs.maxLettersPerLine = (viewportWidth - 2 * textSpecs.margin) / textSpecs.fontSizePixels;

	entriesAdded = recentMessages->size() - formattedMsgs.size();
	if (entriesAdded>0) {
		i = formattedMsgs.size();
	}
	else {
		formattedMsgs.clear();
		totalHeight = 0;
		entriesAdded = recentMessages->size();
		i = 0;
	}

	std::pair<std::string, int> fMessage;
	for (i = 0; i < recentMessages->size(); i++) {
		fMessage = textRenderer.formatGameText(textSpecs, recentMessages->at(i));
		totalHeight += fMessage.second;
		formattedMsgs.push_back(fMessage);
	}

	totalHeight += (entriesAdded - 1) * textSpecs.messageSpacing;
}
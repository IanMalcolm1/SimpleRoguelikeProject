#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "../UIHelpers/TextRenderer.h"
#include "../../GameLog/GameLog.h"


class MessagesUI {
private:
	std::shared_ptr<GameLog> messageLog;

	SDL_Renderer* renderer;
	SDL_Texture* spritesheet;

	int viewportWidth, viewportHeight;
	int totalHeight;
	std::vector<std::pair<std::string, int>> formattedMsgs;

	TextRenderer textRenderer;
	TextRenderingSpecs textSpecs;

	void makeFormattedMessages();

public:
	MessagesUI(std::shared_ptr<GameLog> messageLog,
		int fontSize = 2) :
		renderer(NULL), spritesheet(NULL), viewportWidth(0), viewportHeight(0), totalHeight(0),
		messageLog(messageLog), textRenderer(TextRenderer()), textSpecs(TextRenderingSpecs(fontSize)) {};

	void initialize(SDL_Renderer* renderer, SDL_Texture* spritesheet);

	void render(const SDL_Rect viewport);

	void processMouseScroll(int offset, bool ctrlDown);
};
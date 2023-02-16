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

	SDL_Rect currViewport;
	std::vector<std::pair<int, std::string>> formattedMessages;

	TextRenderer textRenderer;
	TextRenderingSpecifications textSpecs;

	void makeFormattedMessages();

public:
	MessagesUI(std::shared_ptr<GameLog> messageLog,
		int fontSize = 2) :
		renderer(NULL), spritesheet(NULL), currViewport({0,0,0,0}), messageLog(messageLog), textRenderer(TextRenderer()),
		textSpecs(TextRenderingSpecifications(fontSize)) {};

	void initialize(SDL_Renderer* renderer, SDL_Texture* spritesheet);

	void render(SDL_Rect& viewport);

	void processMouseScroll(int offset, bool ctrlDown);
};
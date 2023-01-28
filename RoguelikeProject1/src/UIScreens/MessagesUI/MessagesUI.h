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

	TextRenderer textRenderer;
	TextRenderingSpecifications textSpecs;

public:
	MessagesUI(std::shared_ptr<GameLog> messageLog,
		int fontSize = 2) :
		renderer(NULL), spritesheet(NULL), messageLog(messageLog), textRenderer(TextRenderer()),
		textSpecs(TextRenderingSpecifications(fontSize)) {};

	void initialize(SDL_Renderer* renderer, SDL_Texture* spritesheet);

	void render(SDL_Rect& viewport);

	void processMouseScroll(int offset);
	void modifyFontSize(int modification);
};
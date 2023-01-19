#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "../UIHelpers/TextRenderer.h"
#include "../../GameLog/GameLog.h"


class MessagesUI {
private:
	std::shared_ptr<GameLog> messageLog;

	TextRenderingSpecifications textSpecs;

	std::unique_ptr<TextRenderer> textRenderer;

	SDL_Renderer* renderer;
	SDL_Texture* spritesheet;

public:
	MessagesUI(SDL_Renderer* renderer, SDL_Texture* spritesheet, std::shared_ptr<GameLog> messageLog,
		int fontSize = 2) :
		renderer(renderer), spritesheet(spritesheet), messageLog(messageLog),
		textSpecs(TextRenderingSpecifications(fontSize)),
		textRenderer(std::make_unique<TextRenderer>(renderer, spritesheet)) {};

	void render(SDL_Rect& viewport);

	void changeScrollOffset(int offset);
	void modifyFontSize(int modification);
};
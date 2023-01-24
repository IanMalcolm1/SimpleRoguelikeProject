#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "../UIHelpers/TextRenderer.h"
#include "../../GameLog/GameLog.h"


class MessagesUI {
private:
	std::shared_ptr<GameLog> messageLog;

	TextRenderingSpecifications textSpecs;

public:
	MessagesUI(std::shared_ptr<GameLog> messageLog,	int fontSize = 2) :
		messageLog(messageLog), textSpecs(TextRenderingSpecifications(fontSize)) {};

	void render(SDL_Renderer* renderer,	SDL_Texture* spritesheet, SDL_Rect& viewport);

	void changeScrollOffset(int offset);
	void modifyFontSize(int modification);
};
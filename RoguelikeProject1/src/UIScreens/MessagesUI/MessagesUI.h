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

<<<<<<< HEAD
public:
	MessagesUI(std::shared_ptr<GameLog> messageLog,
		int fontSize = 2) :
		renderer(NULL), spritesheet(NULL), messageLog(messageLog), textRenderer(TextRenderer()),
		textSpecs(TextRenderingSpecifications(fontSize)) {};

	void initialize(SDL_Renderer* renderer, SDL_Texture* spritesheet);
=======
	std::unique_ptr<TextRenderer> textRenderer;

	SDL_Renderer* renderer;
	SDL_Texture* spritesheet;

public:
	MessagesUI(SDL_Renderer* renderer, SDL_Texture* spritesheet, std::shared_ptr<GameLog> messageLog,
		int fontSize = 2) :
		renderer(renderer), spritesheet(spritesheet), messageLog(messageLog),
		textSpecs(TextRenderingSpecifications(fontSize)),
		textRenderer(std::make_unique<TextRenderer>(renderer, spritesheet)) {};
>>>>>>> parent of ccaf4f0 (Updated SDL version. Save point before I make MapUI class)

	void render(SDL_Rect& viewport);

	void changeScrollOffset(int offset);
	void modifyFontSize(int modification);
};
#pragma once

#include "../../Interface/InputManager/InputState.h"
#include "../UIHelpers/TextRenderer.h"
#include <SDL.h>

class ConfirmationUI {
private:
	InputState* state;

	SDL_Renderer* renderer;
	SDL_Texture* spritesheet;

	GameTextMaker textMaker;
	TextRenderer textRenderer;
	TextRenderingSpecs textSpecs;

	GameText message, yes, no;
	SDL_Rect parentViewport;
	SDL_Rect screenViewport, yesViewport, noViewport;

	bool highlightYes, highlightNo;

	void calcDimensions(SDL_Rect& viewport);

public:
	bool hidden;

	ConfirmationUI() : state(NULL), renderer(NULL), spritesheet(NULL), parentViewport({ 0,0,0,0 }),
		yesViewport({ 0,0,0,0 }), noViewport({ 0,0,0,0 }), screenViewport({ 0,0,0,0 }),
		highlightYes(false), highlightNo(false), textRenderer(TextRenderer()),
		textSpecs(TextRenderingSpecs(4)), message(GameText()), yes(GameText()), no(GameText()),
		textMaker(GameTextMaker()), hidden(true) {};

	void initialize(SDL_Renderer* renderer, SDL_Texture* spritesheet);

	void setStateAndMessage(InputState* state, std::string message);

	void render(SDL_Rect& viewport);

	void processMouseLocation(int x, int y);
	void processMouseClick(int x, int y);
	void processPlayerCommand(PlayerCommand command);
};
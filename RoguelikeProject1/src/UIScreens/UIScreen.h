#pragma once

#include <SDL.h>
#include "UIBase.h"

class UIScreen {
public:
	virtual void initialize(UIBase* base, SDL_Renderer* renderer, SDL_Texture* spritesheet) = 0;

	virtual void render(SDL_Rect& viewport) = 0;

	virtual void processScroll(int offset, bool ctrlDown) = 0;
	virtual void processCursorLocation(int x, int y) = 0;
	virtual void processClick(int x, int y, bool ctrlDown) = 0;
};
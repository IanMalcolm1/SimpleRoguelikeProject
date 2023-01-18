#pragma once

#include "../UIHelpers/TextRenderer.h"

class MessagesUI {
private:
	TextRenderingSpecifications textSpecs;

public:
	void render(SDL_Renderer renderer, SDL_Rect viewport);
};
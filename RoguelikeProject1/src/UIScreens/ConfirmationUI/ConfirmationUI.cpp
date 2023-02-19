#include "ConfirmationUI.h"
#include "../UIHelpers/RectFiller.h"


void ConfirmationUI::initialize(SDL_Renderer* renderer, SDL_Texture* spritesheet) {
	this->renderer = renderer;
	this->spritesheet = spritesheet;

	textRenderer.initialize(renderer, spritesheet);

	yes = textMaker.makeGameText("Yes");
	no = textMaker.makeGameText("No");
}

void ConfirmationUI::setStateAndMessage(InputState* state, std::string message) {
	this->state = state;
	this->message = textMaker.makeGameText(message);
}

void ConfirmationUI::render(SDL_Rect& viewport) {
	if (viewport.w != parentViewport.w || viewport.h != parentViewport.h) {
		calcDimensions(viewport);
	}

	SDL_RenderSetViewport(renderer, &parentViewport);

	//gray out everything else
	SDL_SetTextureAlphaMod(spritesheet, 128);
	RectFiller::fill(renderer, spritesheet, viewport, { 0,0,0 });

	//black background for screen
	SDL_SetTextureAlphaMod(spritesheet, 255);
	RectFiller::fill(renderer, spritesheet, screenViewport, { 0,0,0 });

	//highlights
	if (highlightYes) {
		SDL_SetTextureAlphaMod(spritesheet, 128);
		RectFiller::fill(renderer, spritesheet, yesViewport, { 0, 255, 0 });
	}
	else if (highlightNo) {
		SDL_SetTextureAlphaMod(spritesheet, 128);
		RectFiller::fill(renderer, spritesheet, noViewport, { 255, 0, 0 });
	}
	SDL_SetTextureAlphaMod(spritesheet, 255);


	//main message
	SDL_RenderSetViewport(renderer, &screenViewport);
	textSpecs.calcMaxLettersPerLine(screenViewport.w);
	textRenderer.renderGameTextDown(textSpecs, message, textSpecs.margin);

	//yes
	SDL_RenderSetViewport(renderer, &yesViewport);
	textSpecs.calcMaxLettersPerLine(yesViewport.w);
	textRenderer.renderGameTextDown(textSpecs, yes, textSpecs.margin);

	//no
	SDL_RenderSetViewport(renderer, &noViewport);
	textSpecs.calcMaxLettersPerLine(noViewport.w);
	textRenderer.renderGameTextDown(textSpecs, no, textSpecs.margin);

	//borders
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderSetViewport(renderer, &parentViewport);
	SDL_RenderDrawRect(renderer, &yesViewport);
	SDL_RenderDrawRect(renderer, &noViewport);
	SDL_RenderDrawRect(renderer, &screenViewport);


	highlightNo = highlightYes = false;
}

void ConfirmationUI::processMouseLocation(int x, int y) {
	SDL_Point point = { x,y };
	if (!SDL_PointInRect(&point, &screenViewport)) {
		return;
	}

	if (SDL_PointInRect(&point, &yesViewport)) {
		highlightYes = true;
	}
	else if (SDL_PointInRect(&point, &noViewport)) {
		highlightNo = true;
	}
}

void ConfirmationUI::processMouseClick(int x, int y) {
	SDL_Point point = { x,y };
	if (SDL_PointInRect(&point, &yesViewport)) {
		state->confirmation = 1;
		hidden = true;
	}
	else if (SDL_PointInRect(&point, &noViewport)) {
		state->confirmation = 0;
		hidden = true;
	}
}

void ConfirmationUI::processKeyPress(SDL_Keycode keycode) {
	if (keycode == SDLK_y) {
		state->confirmation = 1;
		hidden = true;
	}
	else if (keycode == SDLK_n || keycode == SDLK_ESCAPE) {
		state->confirmation = 0;
		hidden = true;
	}
}


void ConfirmationUI::calcDimensions(SDL_Rect& viewport) {
	//TODO: Make these dimensions constant. No point in them being dynamic.
	parentViewport.w = viewport.w;
	parentViewport.h = viewport.h;

	screenViewport.w = parentViewport.w / 3;
	screenViewport.h = parentViewport.h / 3;
	screenViewport.x = (parentViewport.w - screenViewport.w) / 2;
	screenViewport.y = (parentViewport.h - screenViewport.h) / 2;

	yesViewport.h = noViewport.h = screenViewport.w / 5;
	yesViewport.w = noViewport.w = (screenViewport.w - 3 * 16) / 2;

	yesViewport.x = screenViewport.x + 16;
	noViewport.x = yesViewport.x + yesViewport.w + 16;

	yesViewport.y = noViewport.y = screenViewport.y + screenViewport.h - (16 + yesViewport.h);
}
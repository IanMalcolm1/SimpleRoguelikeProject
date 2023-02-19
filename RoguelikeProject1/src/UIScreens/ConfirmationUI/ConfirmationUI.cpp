#include "ConfirmationUI.h"
#include "../UIHelpers/RectFiller.h"


void ConfirmationUI::initialize(SDL_Renderer* renderer, SDL_Texture* spritesheet) {
	this->renderer = renderer;
	this->spritesheet = spritesheet;

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

	SDL_RenderSetViewport(renderer, &viewport);

	RectFiller::fill(renderer, spritesheet, viewport, { 255,255,255 });



	highlightNo = highlightYes = false;
}

void ConfirmationUI::processMouseLocation(int x, int y) {
	SDL_Point point = { x,y };
	if (SDL_PointInRect(&point, &yesViewport)) {
		highlightYes = true;
	}
	else if (SDL_PointInRect(&point, &yesViewport)) {
		highlightNo = true;
	}
}

void ConfirmationUI::processMouseClick(int x, int y) {
	SDL_Point point = { x,y };
	if (SDL_PointInRect(&point, &yesViewport)) {
		state->confirmation = 1;
	}
	else if (SDL_PointInRect(&point, &yesViewport)) {
		state->confirmation = 0;
	}
}

void ConfirmationUI::processPlayerCommand(PlayerCommand command) {
	if (command == PC_Y) {
		state->confirmation = 1;
	}
	else if (command == PC_N) {
		state->confirmation = 0;
	}
}


void ConfirmationUI::calcDimensions(SDL_Rect& viewport) {
	parentViewport.w = viewport.w;
	parentViewport.h = viewport.h;

	screenViewport.w = screenViewport.h = parentViewport.w / 3;
	screenViewport.x = (parentViewport.w - screenViewport.w) / 2;
	screenViewport.y = (parentViewport.h - screenViewport.h) / 2;

	yesViewport.w = noViewport.w = screenViewport.w / 4;
	yesViewport.h = noViewport.w = screenViewport.h / 2;

	yesViewport.x = ((2 * yesViewport.w + 24) - screenViewport.w) / 2;
	noViewport.x = yesViewport.x + yesViewport.w + 24;

	yesViewport.y = noViewport.y = screenViewport.h - (16 + yesViewport.h);
}
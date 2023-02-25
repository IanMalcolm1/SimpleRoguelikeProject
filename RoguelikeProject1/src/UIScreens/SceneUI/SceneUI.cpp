#include "SceneUI.h"


void SceneUI::initialize(SDL_Renderer* renderer, SDL_Texture* spritesheet) {
	confirmerUI.initialize(scene->presentConfirmationSignaller(), renderer, spritesheet);
	mapUI.initialize(scene->getMap(), renderer, spritesheet);
	playerUI.initialize(scene->getPlayerManager(), renderer, spritesheet);
}

void SceneUI::render(const SDL_Rect& mapViewport, const SDL_Rect& playerViewport) {
	mapUI.render(mapViewport);
	playerUI.render(playerViewport);
	confirmerUI.render(mapViewport);
}

void SceneUI::processCursorLocation(int x, int y) {
	confirmerUI.processMouseLocation(x, y);
	mapUI.processCursorLocation(x, y);
}

void SceneUI::processScroll(int x, int y, int offset, bool ctrlDown) {
	mapUI.processScroll(x, y, offset, ctrlDown);
}

void SceneUI::processClick(int x, int y, bool ctrlDown) {

}

void SceneUI::processKeyPress(SDL_Keycode keycode) {
	confirmerUI.processKeyPress(keycode);
}

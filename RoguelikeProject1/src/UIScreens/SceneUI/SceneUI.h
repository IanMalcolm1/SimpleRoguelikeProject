#pragma once

#include <SDL.h>
#include "../MapUI/MapUI.h"
#include "../MessagesUI/MessagesUI.h"
#include "../PlayerUI/PlayerUI.h"
#include "../ConfirmerUI/ConfirmerUI.h"
#include "../../Scene/Scene.h"

class SceneUI {
private:
	Scene* scene;

	MapUI mapUI;
	PlayerUI playerUI;
	ConfirmerUI confirmerUI;


public:
	bool hidden;

	SceneUI(Scene* scene) : hidden(false), mapUI(MapUI()), playerUI(PlayerUI()),
		confirmerUI(ConfirmerUI(3)), scene(scene) {};

	void initialize(SDL_Renderer* renderer, SDL_Texture* spritesheet);

	void render(const SDL_Rect& mapViewport, const SDL_Rect& playerViewport);

	void processCursorLocation(int x, int y);
	void processScroll(int x, int y, int offset, bool ctrlDown);
	void processClick(int x, int y, bool ctrlDown);
	void processKeyPress(SDL_Keycode keycode);
};
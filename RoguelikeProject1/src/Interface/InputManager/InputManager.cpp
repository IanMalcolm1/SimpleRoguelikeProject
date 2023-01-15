#include "InputManager.h"
#include <stdexcept>


InputManager::InputManager(std::shared_ptr<GameWindow> window, std::shared_ptr<Scene> scene) {
	gameWindow = window;
	this->scene = scene;

	keyMappings = std::unordered_map<SDL_Keycode, PlayerCommand>();

	//TODO: move these defaults to a settings file

	keyMappings.insert({ SDLK_UP, PC_NORTH });
	keyMappings.insert({ SDLK_DOWN, PC_SOUTH });
	keyMappings.insert({ SDLK_RIGHT, PC_EAST });
	keyMappings.insert({ SDLK_LEFT, PC_WEST });
	keyMappings.insert({ SDLK_u, PC_NORTH_EAST });
	keyMappings.insert({ SDLK_y, PC_NORTH_WEST });
	keyMappings.insert({ SDLK_j, PC_SOUTH_EAST });
	keyMappings.insert({ SDLK_h, PC_SOUTH_WEST });

	keyMappings.insert({ SDLK_PERIOD, PC_WAIT });

	keyMappings.insert({ SDLK_l, PC_TOGGLE_LOOK });

	keyMappings.insert({ SDLK_ESCAPE, PC_ESCAPEKEY });

	/* Unimplemented
	keyMappings.insert({ SDLK_i, PI_INV });
	keyMappings.insert({ SDLK_PERIOD, PI_WAIT });
	keyMappings.insert({ SDLK_COMMA, PI_INV });
	*/

}

void InputManager::setScene(std::shared_ptr<Scene> scene) {
	this->scene = scene;
}

bool InputManager::processInput() {
	bool returner = true;
	SDL_Event sdlEvent;
	

	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
		case SDL_KEYDOWN:
			processKeyPress(sdlEvent.key.keysym.sym, sdlEvent.key.keysym.mod);
			break;

		case SDL_MOUSEWHEEL:
			//zooming in/out
			gameWindow->changeMapScale(sdlEvent.wheel.y);
			break;

		case SDL_QUIT: //user closes window using the red x
			returner = false;
			break;

		case SDL_WINDOWEVENT:
			if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED) {
				gameWindow->updateWindowDimensions(sdlEvent.window.data1, sdlEvent.window.data2);
			}
			break;
		}
	}

	int x, y;

	SDL_GetMouseState(&x, &y);

	scene->setMouseTile(gameWindow->findMapTileFromScreenCoordinates(x, y));

	return returner;
}

void InputManager::processKeyPress(SDL_Keycode keycode, Uint16 modification) {
	PlayerCommand command;
	try {
		command = keyMappings.at(keycode);
	}
	catch (std::out_of_range e) {
		return;
	}

	if (gameWindow->getState() == WINDOW_STATE_MAP) {
		if (command < PC_END_OF_PLAYER_ACTIONS) {
			scene->processCommand(command, modification);
		}
	}
}
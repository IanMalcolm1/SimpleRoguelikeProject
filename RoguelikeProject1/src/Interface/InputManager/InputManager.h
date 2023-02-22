#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "../GameWindow/GameWindow.h"
#include "../../Enums/PlayerCommands.h"
#include "../../UIScreens/ConfirmationUI/InputConfirmer.h"
#include "../../Scene/Scene.h"
#include <unordered_map>
#include <SDL_events.h>

class InputManager {
private:
	InputConfirmer confirmer;

	std::shared_ptr<GameWindow> gameWindow;

	std::shared_ptr<Scene> scene;

	std::unordered_map<SDL_Keycode, PlayerCommand> keyMappings;

	void processKeyPress(SDL_Keycode keycode, Uint16 modification);
	bool testControlDown();

public:
	InputManager(std::shared_ptr<GameWindow> window, std::shared_ptr<Scene> scene);

	//Returns false if game needs to be closed
	bool processInput();

	InputConfirmer* presentConfirmer();

	void setScene(std::shared_ptr<Scene> scene);
};



#endif
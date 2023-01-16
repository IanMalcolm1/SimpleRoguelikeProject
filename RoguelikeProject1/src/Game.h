#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include "Interface/GameWindow/GameWindow.h"
#include "Scene/Scene.h"
#include "Interface/InputManager/InputManager.h"


const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
private:
	//game loop
	bool isRunning;
	int millisecsPrevFrame;

	//graphics
	std::shared_ptr<GameWindow> gameWindow;

	//input
	std::unique_ptr<InputManager> inputManager;
	
	std::shared_ptr<Scene> scene;

	std::shared_ptr<GameLog> messageLog;

public:
	Game();
	~Game();
	bool Initialize();
	void Run();
	void Update();
};

#endif
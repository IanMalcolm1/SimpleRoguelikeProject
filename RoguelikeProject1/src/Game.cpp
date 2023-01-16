#include "Game.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <memory>


Game::Game() {
	isRunning = false;
	millisecsPrevFrame = 0;

	gameWindow = std::make_shared<GameWindow>();

	//temporary
	scene = std::make_shared<Scene>();

	inputManager = std::make_unique<InputManager>(gameWindow, scene);

	printf("Game constructor called.\n");
}

Game::~Game() {
	printf("Game destructor called.\n");
}

bool Game::Initialize() {
	isRunning = true;
	bool success;

	success = gameWindow->initialize();

	return success;
}

void Game::Run() {
	while (isRunning) {
		//wasting time between frames
		int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPrevFrame);
		if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
			SDL_Delay(timeToWait);
		}

		Update();

		millisecsPrevFrame = SDL_GetTicks();
	}
}


void Game::Update() {
	//difference in ticks from last frame converted to seconds
	double deltaTime = (SDL_GetTicks() - millisecsPrevFrame) / 1000.0f;


	gameWindow->update(scene->getMapDisplay());

	isRunning = inputManager->processInput();

	scene->updateMapDisplay();
}
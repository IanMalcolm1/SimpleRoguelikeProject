#include "Game.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <memory>


Game::Game() {
	isRunning = false;
	millisecsPrevFrame = 0;

	messageLog = std::make_shared<GameLog>();
	messageLog->sendMessage("Hi, I'm </000255000:Ian/>. Te</000255000:eeeeeeeee/>est. Another </red:test/>. Now I have more </lightblue:colors/>!");
	messageLog->sendMessage("</red:Message 2. Going to make this reeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaallllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllly loooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooong!!!!!!!!!!!!!!!!!!/>");
	messageLog->sendMessage("</orange:Message 3/>");
	messageLog->sendMessage("</yellow:Message 4/>");
	messageLog->sendMessage("</green:Message 5/>");
	messageLog->sendMessage("</blue:Message 6/>");
	messageLog->sendMessage("</purple:Message 7/>");
	messageLog->sendMessage("</gold:The golden message/>");

	scene = std::make_shared<Scene>();

	gameWindow = std::make_shared<GameWindow>(scene->getMap(), messageLog);

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
<<<<<<< HEAD
	gameWindow->update();
=======
	//difference in ticks from last frame converted to seconds
	double deltaTime = (SDL_GetTicks() - millisecsPrevFrame) / 1000.0f;


	gameWindow->update(scene->getMapDisplay());
>>>>>>> parent of ccaf4f0 (Updated SDL version. Save point before I make MapUI class)

	isRunning = inputManager->processInput();

	scene->updateMapDisplay();
}
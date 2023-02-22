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

	terrainGenerator = TerrainGenerators();

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

	terrainGenerator.rectangleRooms(scene.get(), 20, 20);

	InputConfirmer* inputConfirmer = inputManager->presentConfirmer();
	InputConfirmer* sceneConfirmer = scene->presentConfirmationSignaller();
	success = gameWindow->initialize(inputConfirmer, sceneConfirmer);

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
	gameWindow->update();

	isRunning = inputManager->processInput();

	scene->updateMapDisplay();
}
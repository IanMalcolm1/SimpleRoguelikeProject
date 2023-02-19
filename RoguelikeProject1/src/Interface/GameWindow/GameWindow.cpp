#include "GameWindow.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

GameWindow::GameWindow(LocalMap* map, std::shared_ptr<GameLog> log,
	int windowWidth, int windowHeight) : mapUI(map), messagesUI(log) {
	screenDimensions.x = screenDimensions.y = 0;
	screenDimensions.w = windowWidth;
	screenDimensions.h = windowHeight;

	viewports.map.x = 1 * screenDimensions.w / 5;
	viewports.map.y = 0;
	viewports.map.h = 4 * screenDimensions.h / 5;
	viewports.map.w = 4 * screenDimensions.w / 5;

	updateMapViewports();
	
	confirmExit = ConfirmationUI();
	//TODO: remove this
	confirmExit.setStateAndMessage(new InputState, "Confirm quit?");
	confirmExit.hidden = false;

	window = NULL;
	renderer = NULL;
	spritesheet = NULL;
}

GameWindow::~GameWindow() {
	printf("Window destructor called.\n");

	SDL_DestroyTexture(spritesheet);
	spritesheet = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	IMG_Quit();
	SDL_Quit();
}

bool GameWindow::initialize() {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL didn't init. We're doomed. Error: %s\n", SDL_GetError());
		success = false;
	}
	if (IMG_Init(IMG_INIT_PNG) < 0) {
		printf("SDL_Image didn't innit. We're doomed. Error: %s\n", SDL_GetError());
		success = false;
	}

	window = SDL_CreateWindow(
		"Testing things out",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screenDimensions.w, screenDimensions.h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	if (window == NULL) {
		printf("Couldn't make a window. Error: %s\n", SDL_GetError());
		success = false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	spritesheet = IMG_LoadTexture(renderer, "./assets/CGA8x8thin.png");
	if (spritesheet == NULL) {
		printf("Couldn't find spritesheet, I guess. Error: %s\n", IMG_GetError());
		success = false;
	}

	//UI screens
	messagesUI.initialize(renderer, spritesheet);
	mapUI.initialize(renderer, spritesheet);
	confirmExit.initialize(renderer, spritesheet);

	return success;
}


GameWindowState GameWindow::getState() { return state; }
void GameWindow::setState(GameWindowState state) { this->state = state; }

void GameWindow::renderMap() {
	mapUI.render(viewports.map);

	resetRendererAndDrawBorder(viewports.map);
}

void GameWindow::renderRecentMessages() {
	messagesUI.render(viewports.messages);

	resetRendererAndDrawBorder(viewports.messages);
}

void GameWindow::renderPlayerInfo() {
	//TODO: draw player info, lol

	resetRendererAndDrawBorder(viewports.playerInfo);
}


void GameWindow::updateMapViewports() {
	int screenBorderSpacing = 5;
	int viewportSpacing = 8;

	/* x-axis */
	viewports.playerInfo.x = screenBorderSpacing;
	viewports.playerInfo.w = (1 * screenDimensions.w / 5) - (screenBorderSpacing + viewportSpacing/2);

	viewports.map.x = (1 * screenDimensions.w / 5) + (viewportSpacing/2);
	viewports.map.w = (4 * screenDimensions.w / 5) - (screenBorderSpacing + viewportSpacing/2);

	viewports.messages.x = (1 * screenDimensions.w / 5) + (viewportSpacing/2);
	viewports.messages.w = (4 * screenDimensions.w / 5) - (screenBorderSpacing + viewportSpacing/2);

	/* y-axis */
	viewports.playerInfo.y = screenBorderSpacing;
	viewports.playerInfo.h = screenDimensions.h - (2*screenBorderSpacing);

	viewports.map.y = screenBorderSpacing;
	viewports.map.h = (4 * screenDimensions.h / 5) - (screenBorderSpacing + viewportSpacing/2);

	viewports.messages.y = (4 * screenDimensions.h / 5) + (viewportSpacing/2);
	viewports.messages.h = (1 * screenDimensions.h / 5) - (screenBorderSpacing + viewportSpacing/2);
}


void GameWindow::resetRendererAndDrawBorder(SDL_Rect& currentViewport) {
	SDL_RenderSetViewport(renderer, &screenDimensions);
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &currentViewport);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetTextureAlphaMod(spritesheet, 255);
	SDL_SetTextureColorMod(spritesheet, 255, 255, 255);
}


//TODO: remove tests
void GameWindow::update() {
	SDL_RenderClear(renderer);

	int startTime, mapTime, messagesTime, playerTime, renderingTime;
	startTime = SDL_GetTicks();

	renderMap();
	mapTime = SDL_GetTicks();

	renderRecentMessages();
	messagesTime = SDL_GetTicks();

	renderPlayerInfo();
	playerTime = SDL_GetTicks();

	if (!confirmExit.hidden) {
		confirmExit.render(screenDimensions);
		resetRendererAndDrawBorder(screenDimensions);
	}

	SDL_RenderPresent(renderer);
	renderingTime = SDL_GetTicks();

	renderingTime -= playerTime;
	playerTime -= messagesTime;
	messagesTime -= mapTime;
	mapTime -= startTime;

	printf("Map: %i\nMessages: %i\nPlayer: %i\nRendering: %i\n", mapTime, messagesTime, playerTime, renderingTime);
}


void GameWindow::processScroll(int x, int y, int scrollOffset, bool ctrlDown) {
	if (!confirmExit.hidden) {
		return;
	}

	SDL_Point point = { x,y };
	if (SDL_PointInRect(&point, &viewports.map)) {
		mapUI.processScroll(scrollOffset, ctrlDown);
	}
	else if (SDL_PointInRect(&point, &viewports.messages)) {
		messagesUI.processScroll(scrollOffset, ctrlDown);
	}
}


void GameWindow::updateWindowDimensions(int width, int height) {
	screenDimensions.w = width;
	screenDimensions.h = height;

	updateMapViewports();
}


void GameWindow::processCursorLocation(int x, int y) {
	SDL_Point point = { x,y };

	if (!confirmExit.hidden) {
		confirmExit.processMouseLocation(x, y);
	}
	else if (SDL_PointInRect(&point, &viewports.map)) {
		x -= viewports.map.x;
		y -= viewports.map.y;
		mapUI.processCursorLocation(x, y);
	}
}
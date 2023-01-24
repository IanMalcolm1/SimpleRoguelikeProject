#include "GameWindow.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

GameWindow::GameWindow(std::shared_ptr<LocalMap> map, std::shared_ptr<GameLog> log,
	int windowWidth, int windowHeight) : mapUI(map), messagesUI(log) {
	screenDimensions.x = screenDimensions.y = 0;
	screenDimensions.w = windowWidth;
	screenDimensions.h = windowHeight;

	viewports.map.x = 1 * screenDimensions.w / 5;
	viewports.map.y = 0;
	viewports.map.h = 4 * screenDimensions.h / 5;
	viewports.map.w = 4 * screenDimensions.w / 5;

	updateMapViewports();

	window = NULL;
	renderer = NULL;
<<<<<<< HEAD
	spritesheet = NULL;
=======
	spriteSheet = NULL;

	displayedTilesMap = std::make_unique<DisplayedTilesMap>();
>>>>>>> parent of ccaf4f0 (Updated SDL version. Save point before I make MapUI class)
}

GameWindow::~GameWindow() {
	printf("Window destructor called.\n");

	SDL_DestroyTexture(spriteSheet);
	spriteSheet = NULL;

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

	spriteSheet = IMG_LoadTexture(renderer, "./assets/CGA8x8thin.png");
	if (spriteSheet == NULL) {
		printf("Couldn't find spritesheet, I guess. Error: %s\n", IMG_GetError());
		success = false;
	}

	//UI screens
<<<<<<< HEAD
	messagesUI.initialize(renderer, spritesheet);
	mapUI.initialize(renderer, spritesheet);
=======
	messagesUI = std::make_unique<MessagesUI>(renderer, spriteSheet, messageLog);
>>>>>>> parent of ccaf4f0 (Updated SDL version. Save point before I make MapUI class)

	return success;
}


GameWindowState GameWindow::getState() { return state; }
void GameWindow::setState(GameWindowState state) { this->state = state; }

<<<<<<< HEAD
void GameWindow::renderMap() {
	mapUI.render(viewports.map);
=======

void GameWindow::renderMap(std::shared_ptr<MapDisplay> map) {
	SDL_RenderSetViewport(renderer, &viewports.map);

	//number of pixels to one side of a tile
	int scaleSize = mapScale * 8;

	MapRenderingData renderingData;
	renderingData = calculateMapRenderingDimensions(viewports.map, map->getWidth(), map->getHeight(), map->getFocusTile());

	displayedTilesMap->clearAndSetDimensions(viewports.map, renderingData, map->getWidth());

	//for SDL_RenderCopy()
	SDL_Rect srcrect = { 0,0,8,8 };
	SDL_Rect dstrect = { 0,0, scaleSize, scaleSize };

	int currentIndex;


	for (int x = renderingData.startTile.x; x < renderingData.endTile.x; x++) {
		for (int y = renderingData.startTile.y; y < renderingData.endTile.y; y++) {
			currentIndex = y * map->getWidth() + x;
			TileDisplay* tile = map->getDisplay(currentIndex);

			dstrect.x = renderingData.pixelOffsetX + x * scaleSize;
			dstrect.y = renderingData.pixelOffsetY + y * scaleSize;

			//keep track of where tile is being displayed
			displayedTilesMap->pushTile(&dstrect);

			//unseen tiles are rendered black
			if ( !map->hasBeenSeen(currentIndex)) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderFillRect(renderer, &dstrect);
				continue;
			}

			srcrect.x = tile->symbol % 16 * 8;
			srcrect.y = tile->symbol / 16 * 8;


			//background
			SDL_SetRenderDrawColor(renderer, tile->backColor.r, tile->backColor.g, tile->backColor.b, 255);

			SDL_RenderFillRect(renderer, &dstrect);

			//foreground
			if (!map->isVisible(currentIndex)) {
				SDL_SetTextureColorMod(spriteSheet, 100, 100, 100);
			}
			else {
				SDL_SetTextureColorMod(spriteSheet, tile->symbolColor.r, tile->symbolColor.g,
					tile->symbolColor.b);
			}

			//render
			SDL_RenderCopy(renderer, spriteSheet, &srcrect, &dstrect);


			//focus tile reticle
			if (map->hasReticle(currentIndex)) {
				srcrect.x = ASYM_RETICLE % 16 * 8;
				srcrect.y = ASYM_RETICLE / 16 * 8;

				SDL_SetTextureColorMod(spriteSheet, 255, 255, 255);

				SDL_RenderCopy(renderer, spriteSheet, &srcrect, &dstrect);
			}
		}
	}
>>>>>>> parent of ccaf4f0 (Updated SDL version. Save point before I make MapUI class)

	resetRendererAndDrawBorder(viewports.map);
}

void GameWindow::renderRecentMessages() {
<<<<<<< HEAD
	messagesUI.render(viewports.messages);
=======
	messagesUI->render(viewports.messages);
>>>>>>> parent of ccaf4f0 (Updated SDL version. Save point before I make MapUI class)

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
	SDL_SetTextureColorMod(spriteSheet, 255, 255, 255);
}



void GameWindow::update() {
	SDL_RenderClear(renderer);

<<<<<<< HEAD
	int startTime, mapTime, messagesTime, playerTime, renderingTime;
	startTime = SDL_GetTicks();

	renderMap();
	mapTime = SDL_GetTicks();

=======
	renderMap(mapDisplay);
>>>>>>> parent of ccaf4f0 (Updated SDL version. Save point before I make MapUI class)
	renderRecentMessages();
	renderPlayerInfo();

	SDL_RenderPresent(renderer);
}

void GameWindow::processMouseScroll(int x, int y, int scrollOffset) {
	SDL_Point point = { x,y };
	if (SDL_PointInRect(&point, &viewports.map)) {
		mapUI.changeScale(scrollOffset);
	}
	else if (SDL_PointInRect(&point, &viewports.messages)) {
		messagesUI.changeScrollOffset(scrollOffset);
	}
}


void GameWindow::updateWindowDimensions(int width, int height) {
	screenDimensions.w = width;
	screenDimensions.h = height;

	updateMapViewports();
}


TileCoordinates GameWindow::processMouseLocation(int x, int y) {
	SDL_Point point = { x,y };
	if (SDL_PointInRect(&point, &viewports.map)) {
		return mapUI.findMapTileFromScreenCoords(x,y);
	}

	return TileCoordinates(-1, -1);
}
#include "GameWindow.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

GameWindow::GameWindow(std::shared_ptr<GameLog> messageLog, int scale, int wWidth, int wHeight) {
	mapScale = scale;
	
	screenDimensions.x = screenDimensions.y = 0;
	screenDimensions.w = wWidth;
	screenDimensions.h = wHeight;

	viewports.map.x = 1 * screenDimensions.w / 5;
	viewports.map.y = 0;
	viewports.map.h = 4 * screenDimensions.h / 5;
	viewports.map.w = 4 * screenDimensions.w / 5;

	updateMapViewports();

	window = NULL;
	renderer = NULL;
	spriteSheet = NULL;

	displayedTilesMap = std::make_unique<DisplayedTilesMap>();

	this->messageLog = messageLog;

	//TODO: remove this line and everything below it in the function
	messageLog->sendMessage("Hi, I'm </000255000:Ian/>. Te</000255000:eeeeeeeee/>est. Another </red:test/>. Now I have more </lightblue:colors/>!");
	messageLog->sendMessage("</red:Message 2. Going to make this reeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaallllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllly loooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooong!!!!!!!!!!!!!!!!!!/>");
	messageLog->sendMessage("</orange:Message 3/>");
	messageLog->sendMessage("</yellow:Message 4/>");
	messageLog->sendMessage("</green:Message 5/>");
	messageLog->sendMessage("</blue:Message 6/>");
	messageLog->sendMessage("</purple:Message 7/>");
	messageLog->sendMessage("</gold:The golden message/>");

	textSpecs = TextRenderingSpecifications(2, 16);
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

	return success;
}


GameWindowState GameWindow::getState() { return state; }
void GameWindow::setState(GameWindowState state) { this->state = state; }


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

	resetRendererAndDrawBorder(viewports.map);
}


MapRenderingData GameWindow::calculateMapRenderingDimensions(SDL_Rect viewport, int mapWidthTiles, int mapHeightTiles, TileCoordinates focusTile) {
	/*
	Intent:
	The below algorithm ensures we see as many tiles as possible given the scale of the
	map and the tile being focused on. Essentially this means we don't always center the
	'camera' on the focused tile if that tile is too close to one edge and doing so would
	create empty space. Of course, if the map display would be smaller than the window's
	dimensions (aka, when we're really zoomed out), we can't really avoid empty space and
	just center the map.
	*/

	MapRenderingData renderingData = MapRenderingData();

	int scaleSize = mapScale * 8;

	int mapWidthPixels = mapWidthTiles * scaleSize;
	int mapHeightPixels = mapHeightTiles * scaleSize;

	//x-axis
	if (mapWidthPixels <= viewport.w) {
		renderingData.pixelOffsetX = (viewport.w - mapWidthPixels) / 2;
		renderingData.startTile.x = 0;
		renderingData.endTile.x = mapWidthTiles;
	}
	else if (scaleSize * (mapWidthTiles - focusTile.x) < (viewport.w / 2)) {
		renderingData.pixelOffsetX = viewport.w - mapWidthPixels;
		renderingData.startTile.x = -renderingData.pixelOffsetX / scaleSize;
		renderingData.endTile.x = mapWidthTiles;
	}
	else if (scaleSize * focusTile.x > (viewport.w / 2)) {
		renderingData.pixelOffsetX = ((viewport.w - scaleSize) / 2) - (scaleSize * focusTile.x);
		renderingData.startTile.x = -renderingData.pixelOffsetX / scaleSize;
		renderingData.endTile.x = (-renderingData.pixelOffsetX + viewport.w) / scaleSize;
		if (renderingData.endTile.x < mapWidthTiles && (-renderingData.pixelOffsetX + viewport.w) % scaleSize > 0) {
			renderingData.endTile.x++;
		}
	}
	else {
		renderingData.startTile.x = 0;
		renderingData.endTile.x = (-renderingData.pixelOffsetX + viewport.w) / scaleSize;
		if (renderingData.endTile.x < mapWidthTiles && (-renderingData.pixelOffsetX + viewport.w) % scaleSize > 0) {
			renderingData.endTile.x++;
		}
	}

	//y-axis
	if (mapHeightPixels < viewport.h) {
		renderingData.pixelOffsetY = (viewport.h - mapHeightPixels) / 2;
		renderingData.endTile.y = mapHeightTiles;
		renderingData.startTile.y = 0;
	}
	else if (scaleSize * (mapHeightTiles - focusTile.y) < (viewport.h / 2)) {
		renderingData.pixelOffsetY = viewport.h - mapHeightPixels;
		renderingData.startTile.y = -renderingData.pixelOffsetY / scaleSize;
		renderingData.endTile.y = mapHeightTiles;
	}
	else if (scaleSize * focusTile.y > (viewport.h / 2)) {
		renderingData.pixelOffsetY = ((viewport.h - scaleSize) / 2) - (scaleSize * focusTile.y);
		renderingData.startTile.y = -renderingData.pixelOffsetY / scaleSize;
		renderingData.endTile.y = (-renderingData.pixelOffsetY + viewport.h) / scaleSize;
		if (renderingData.endTile.y < mapHeightTiles && (-renderingData.pixelOffsetY + viewport.h) % scaleSize > 0) {
			renderingData.endTile.y++;
		}
	}
	else {
		renderingData.startTile.y = 0;
		renderingData.endTile.y = (-renderingData.pixelOffsetY + viewport.h) / scaleSize;
		if (renderingData.endTile.y < mapHeightTiles && (-renderingData.pixelOffsetY + viewport.h) % scaleSize > 0) {
			renderingData.endTile.y++;
		}
	}

	return renderingData;
}


//TODO: include color support. Use start and end characters to indicated beginnings and ends of 
//		colored text. Use stack to allow nested colors? (Maybe superflous and relatively performance
//		intensive) Either way, first 3 chars after beginning char should be treated as rgb values.
void GameWindow::renderRecentMessages() {
	SDL_RenderSetViewport(renderer, &viewports.messages);

	if (messageLog->getRecentMessages()->size() == 0) {
		return;
	}

	textSpecs.maxLettersPerLine = (viewports.messages.w - 2*textSpecs.margin) / textSpecs.fontSizePixels;

	SDL_Rect destinationRect;
	destinationRect.w = textSpecs.fontSizePixels;
	destinationRect.h = textSpecs.fontSizePixels;
	destinationRect.x = textSpecs.margin;
	destinationRect.y = viewports.messages.h - (textSpecs.margin) + textSpecs.scrollOffset;

	std::vector<GameMessage>* recentMessages = messageLog->getRecentMessages();

	for (int i = 0; i < recentMessages->size(); i++) {
		if (destinationRect.y < -textSpecs.fontSizePixels) {
			break;
		}

		renderIndividualMessage(recentMessages->at(i), destinationRect);
	}

	resetRendererAndDrawBorder(viewports.messages);
}


std::pair<std::string, int> GameWindow::makeFormattedMessage(int maxLettersPerLine, std::string message) {
	int lines = 1;
	int index = maxLettersPerLine-1;

	while (index < (int)message.size()) {

		if (message[index] == ASYM_SPACE) {
			message[index] = '\n';
			lines++;
		}

		else if (message[index + 1] == ASYM_SPACE) {
			index++;
			message[index] = '\n';
			lines++;
		}

		else {
			int prevIndex = index;
			while (index > 0) {
				index--;
				if (message[index] == ASYM_SPACE) {
					message[index] = '\n';
					lines++;
					break;
				}
				else if (prevIndex - index == maxLettersPerLine) {
					message.insert(message.begin() + prevIndex+1, '\n');
					lines++;
					index = prevIndex + 1;
					break;
				}
			}
			if (index == 0) {
				message.insert(message.begin() + prevIndex+1, '\n');
				lines++;
				index = prevIndex + 1;
			}
		}

		index += maxLettersPerLine;
	}

	return std::make_pair(message, lines);
}

void GameWindow::renderIndividualMessage(GameMessage message, SDL_Rect& destinationRect) {
	SDL_Rect sourceRect;
	sourceRect.w = sourceRect.h = 8;

	std::pair<std::string, int> messageTextAndNumberOfLines = makeFormattedMessage(textSpecs.maxLettersPerLine, message.text);

	std::string formattedText = messageTextAndNumberOfLines.first;
	int lines = messageTextAndNumberOfLines.second;

	int unformattedIndex = 0;
	MyColor currentColor;

	destinationRect.x = textSpecs.margin;

	destinationRect.y -= lines * textSpecs.fontSizePixels + (lines - 1) * textSpecs.lineSpacing;

	int startY = destinationRect.y;

	for (int i = 0; i < formattedText.size(); i++) {
		char test = formattedText[i];

		if (test == '\n') {
			destinationRect.y += textSpecs.fontSizePixels + textSpecs.lineSpacing;
			destinationRect.x = textSpecs.margin;
			continue;
		}

		sourceRect.x = formattedText[i] % 16 * 8;
		sourceRect.y = formattedText[i] / 16 * 8;

		currentColor = message.getColorAtIndex(unformattedIndex);

		SDL_SetTextureColorMod(spriteSheet, currentColor.r, currentColor.g, currentColor.b);
		SDL_RenderCopy(renderer, spriteSheet, &sourceRect, &destinationRect);

		destinationRect.x += textSpecs.fontSizePixels;

		unformattedIndex++;
	}

	destinationRect.y = startY - textSpecs.messageSpacing - textSpecs.lineSpacing;
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



void GameWindow::update(std::shared_ptr<MapDisplay> mapDisplay) {
	SDL_RenderClear(renderer);

	renderMap(mapDisplay);
	renderRecentMessages();
	renderPlayerInfo();

	SDL_RenderPresent(renderer);
}


void GameWindow::changeMapScale(int offset) {
	mapScale += offset;
	if (mapScale < 1) { mapScale = 1; }
	else if (mapScale > 24) { mapScale = 24;  }
}

void GameWindow::changeMessagesScrollOffset(int offset) {
	textSpecs.scrollOffset += 4*offset;
	textSpecs.scrollOffset = (textSpecs.scrollOffset > 0) ? textSpecs.scrollOffset : 0;
}

void GameWindow::processMouseScroll(int x, int y, int scrollOffset) {
	SDL_Point point = { x,y };
	if (SDL_PointInRect(&point, &viewports.map)) {
		changeMapScale(scrollOffset);
	}
	else if (SDL_PointInRect(&point, &viewports.messages)) {
		changeMessagesScrollOffset(scrollOffset);
	}
}


void GameWindow::updateWindowDimensions(int width, int height) {
	screenDimensions.w = width;
	screenDimensions.h = height;

	updateMapViewports();
}
#include "MapUI.h"

void MapUI::initialize(SDL_Renderer* renderer, SDL_Texture* spritesheet) {
	this->renderer = renderer;
	this->spritesheet = spritesheet;

	int textureWidth = map->getWidth() * 8;
	int textureHeight = map->getHeight() * 8;
	mapTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
		textureWidth, textureHeight);
}


void MapUI::render(SDL_Rect& viewport) {
	SDL_SetRenderTarget(renderer, mapTexture);

	calculateMapRenderingData(viewport);

	//for SDL_RenderCopy()
	SDL_Rect dstrect = { 0,0, 8, 8 };

	int index;

	for (int x = rData.startTile.x; x < rData.endTile.x; x++) {
		for (int y = rData.startTile.y; y < rData.endTile.y; y++) {
			index = y * mapDisplay->getWidth() + x;

			if (mapDisplay->isDirty(index)) {
				dstrect.x = x * 8;
				dstrect.y = y * 8;
				renderTile(index, dstrect);
			}
		}
	}


	SDL_SetRenderTarget(renderer, NULL);

	SDL_RenderSetViewport(renderer, &viewport);

	SDL_RenderCopy(renderer, mapTexture, &rData.srcRect, &rData.dstRect);
}


void MapUI::calculateMapRenderingData(SDL_Rect& viewport) {
	/*
	Intent:
	The below algorithm ensures we see as many tiles as possible given the scale of the
	map and the tile being focused on. Essentially this means we don't always center the
	'camera' on the focused tile if that tile is too close to one edge and doing so would
	create empty space. Of course, if the map display would be smaller than the window's
	dimensions (aka, when we're really zoomed out), we can't really avoid empty space and
	just center the map.
	*/

	TileCoordinates focusTile = mapDisplay->getFocusTile();

	int mapWidthTiles = map->getWidth();
	int mapHeightTiles = map->getHeight();

	int mapWidthPixels = mapWidthTiles * rData.scaleSize;
	int mapHeightPixels = mapHeightTiles * rData.scaleSize;

	//x-axis
	if (mapWidthPixels <= viewport.w) {
		rData.dstRect.x = (viewport.w - mapWidthPixels) / 2;
		rData.startTile.x = 0;
		rData.endTile.x = mapWidthTiles;
	}
	else if (rData.scaleSize * (mapWidthTiles - focusTile.x) < (viewport.w / 2)) {
		rData.dstRect.x = viewport.w - mapWidthPixels;
		rData.startTile.x = -rData.dstRect.x / rData.scaleSize;
		rData.endTile.x = mapWidthTiles;
	}
	else if (rData.scaleSize * focusTile.x > (viewport.w / 2)) {
		rData.dstRect.x = ((viewport.w - rData.scaleSize) / 2) - (rData.scaleSize * focusTile.x);
		rData.startTile.x = -rData.dstRect.x / rData.scaleSize;
		rData.endTile.x = (-rData.dstRect.x + viewport.w) / rData.scaleSize;
		if (rData.endTile.x < mapWidthTiles && (-rData.dstRect.x + viewport.w) % rData.scaleSize > 0) {
			rData.endTile.x++;
		}
	}
	else {
		rData.startTile.x = 0;
		rData.endTile.x = (-rData.dstRect.x + viewport.w) / rData.scaleSize;
		if (rData.endTile.x < mapWidthTiles && (-rData.dstRect.x + viewport.w) % rData.scaleSize > 0) {
			rData.endTile.x++;
		}
	}

	//y-axis
	if (mapHeightPixels < viewport.h) {
		rData.dstRect.y = (viewport.h - mapHeightPixels) / 2;
		rData.endTile.y = mapHeightTiles;
		rData.startTile.y = 0;
	}
	else if (rData.scaleSize * (mapHeightTiles - focusTile.y) < (viewport.h / 2)) {
		rData.dstRect.y = viewport.h - mapHeightPixels;
		rData.startTile.y = -rData.dstRect.y / rData.scaleSize;
		rData.endTile.y = mapHeightTiles;
	}
	else if (rData.scaleSize * focusTile.y > (viewport.h / 2)) {
		rData.dstRect.y = ((viewport.h - rData.scaleSize) / 2) - (rData.scaleSize * focusTile.y);
		rData.startTile.y = -rData.dstRect.y / rData.scaleSize;
		rData.endTile.y = (-rData.dstRect.y + viewport.h) / rData.scaleSize;
		if (rData.endTile.y < mapHeightTiles && (-rData.dstRect.y + viewport.h) % rData.scaleSize > 0) {
			rData.endTile.y++;
		}
	}
	else {
		rData.startTile.y = 0;
		rData.endTile.y = (-rData.dstRect.y + viewport.h) / rData.scaleSize;
		if (rData.endTile.y < mapHeightTiles && (-rData.dstRect.y + viewport.h) % rData.scaleSize > 0) {
			rData.endTile.y++;
		}
	}

	rData.srcRect.x = rData.startTile.x % mapWidthTiles;
	rData.srcRect.y = rData.startTile.y / mapWidthTiles;
	rData.srcRect.w = (rData.endTile.x - rData.startTile.x) * 8;
	rData.srcRect.h = (rData.endTile.y - rData.startTile.y) * 8;

	rData.dstRect.w = (rData.endTile.x - rData.startTile.x) * rData.scaleSize;
	rData.dstRect.h = (rData.endTile.y - rData.startTile.y) * rData.scaleSize;
}


void MapUI::renderTile(int index, SDL_Rect dstrect) {
	TileDisplay* tile = mapDisplay->getDisplay(index);

	SDL_Rect srcrect = { 0,0,8,8 };

	//unseen tiles are rendered black
	if (!mapDisplay->hasBeenSeen(index)) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(renderer, &dstrect);
		return;
	}

	//tile background
	SDL_SetRenderDrawColor(renderer, tile->backColor.r, tile->backColor.g, tile->backColor.b, 255);
	SDL_RenderFillRect(renderer, &dstrect);

	//tile foreground
	if (!mapDisplay->isVisible(index)) {
		SDL_SetTextureColorMod(spritesheet, 100, 100, 100);
	}
	else {
		SDL_SetTextureColorMod(spritesheet, tile->symbolColor.r, tile->symbolColor.g,
			tile->symbolColor.b);
	}

	srcrect.x = tile->symbol % 16 * 8;
	srcrect.y = tile->symbol / 16 * 8;

	SDL_RenderCopy(renderer, spritesheet, &srcrect, &dstrect);

	//reticles
	if (mapDisplay->hasReticle(index)) {
		srcrect.x = ASYM_RETICLE % 16 * 8;
		srcrect.y = ASYM_RETICLE / 16 * 8;

		SDL_SetTextureColorMod(spritesheet, 255, 255, 255);

		SDL_RenderCopy(renderer, spritesheet, &srcrect, &dstrect);
	}
}


void MapUI::changeScale(int offset) {
	rData.scale += offset;
	if (rData.scale < 1) { rData.scale = 1; }
	else if (rData.scale > 24) { rData.scale = 24; }

	rData.scaleSize = rData.scale * 8;
}

//TODO
TileCoordinates MapUI::findMapTileFromScreenCoords(int x, int y) {
	TileCoordinates returnCoords = TileCoordinates(-1, -1);

	SDL_Point point = { x,y };
	if (SDL_PointInRect(&point, &rData.dstRect)) {
		return returnCoords;
	}

	if (x % rData.scaleSize == 0) {
		returnCoords.x = x / rData.scaleSize;
		returnCoords.x += rData.startTile.x - 1;
	}
	else {
		returnCoords.x = x / rData.scaleSize;
		returnCoords.x += rData.startTile.x;
	}
	int tileY = rData.startTile.y + y / rData.scaleSize;

	return returnCoords;
}

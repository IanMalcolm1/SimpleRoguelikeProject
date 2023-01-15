#include "DisplayedTilesMap.h"


void DisplayedTilesMap::clearAndSetDimensions(SDL_Rect viewport, MapRenderingData renderingData, int baseMapWidth) {
	this->viewport = viewport;

	startTile = renderingData.startTile;

	widthDisplayedTiles = renderingData.endTile.x - startTile.x;
	heightDisplayedTiles = renderingData.endTile.y - startTile.y;

	widthBaseMap = baseMapWidth;

	tiles.clear();
}

void DisplayedTilesMap::pushTile(SDL_Rect* rect) {
	tiles.push_back(*rect);
}

TileCoordinates DisplayedTilesMap::findTileFromScreenCoordinates(int x, int y) {
	TileCoordinates returnCoords;

	if (x < viewport.x || x > (viewport.x + viewport.w) ||
		y < viewport.y || y > (viewport.y + viewport.h) ) {
		returnCoords.x = returnCoords.y = -1;
		return returnCoords;
	}


	int tileX = x - (viewport.x + tiles[0].x);
	if (tileX % tiles[0].w == 0) {
		tileX /= tiles[0].w;
		tileX += startTile.x - 1;
	}
	else {
		tileX /= tiles[0].w;
		tileX += startTile.x;
	}
	int tileY = startTile.y + (y - (viewport.y + tiles[0].y)) / tiles[0].h;
	
	returnCoords.x = tileX;
	returnCoords.y = tileY;
	return returnCoords;
}
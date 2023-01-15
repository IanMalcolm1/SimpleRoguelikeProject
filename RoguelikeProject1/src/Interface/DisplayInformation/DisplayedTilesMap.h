#ifndef DISPLAYEDTILESMAP_H
#define DISPLAYEDTILESMAP_H

#include <SDL_rect.h>
#include <vector>
#include "../../GameObjects/TileCoordinates.h"


struct MapRenderingData {
	TileCoordinates startTile, endTile;
	int pixelOffsetX, pixelOffsetY;

	MapRenderingData() : startTile(), endTile(), pixelOffsetX(0), pixelOffsetY(0) {};
};


class DisplayedTilesMap {
private:
	SDL_Rect viewport;

	TileCoordinates startTile;
	int widthDisplayedTiles;
	int heightDisplayedTiles;
	int widthBaseMap;

	std::vector<SDL_Rect> tiles;

public:
	DisplayedTilesMap() : viewport(), startTile(), widthDisplayedTiles(0), heightDisplayedTiles(0), 
		widthBaseMap(0) {};

	void clearAndSetDimensions(SDL_Rect viewport, MapRenderingData renderingData, int baseMapWidth);
	void pushTile(SDL_Rect* rect);

	//Takes the coordinates of a point, returns the id of the tile at that point
	TileCoordinates findTileFromScreenCoordinates(int x, int y);
};

#endif
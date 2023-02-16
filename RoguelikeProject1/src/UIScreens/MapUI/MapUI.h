#pragma once

#include <memory>
#include <SDL.h>
#include "../../Topography/LocalMap/LocalMap.h"


struct MapRenderingData {
	SDL_Rect srcRect; //The part of the map texture to copy from
	SDL_Rect dstRect; //The part of the map viewport to paste on to

	TileCoordinates startTile, endTile;

	int scale; //Scale mulitiplier
	int scaleSize; //Actual number of pixels to a side of a tile

	MapRenderingData() : srcRect({0,0,0,0}), dstRect({0,0,0,0}), scale(12), scaleSize(12*8),
		startTile(), endTile() {};
};


class MapUI {
private:
	LocalMap* map;
	MapDisplay* mapDisplay; //get from map. holds display information for tiles
	MapRenderingData rData; //keeps track of which tiles have been rendered where

	SDL_Texture* mapTexture;

	SDL_Renderer* renderer;
	SDL_Texture* spritesheet;

	void calculateMapRenderingData(SDL_Rect& viewport);
	void calcDataForAxis(SDL_Rect& viewport, char axis);

	void renderTile(int index, SDL_Rect dstrect);
	//Uses SDL_RenderCopy() rather than normal SDL_RenderFillRect which is incredibly slow
	void fillRectImproved(SDL_Rect& destination, MyColor color);

public:
	MapUI(LocalMap* map) : map(map), renderer(NULL), spritesheet(NULL),
		mapTexture(NULL), mapDisplay(mapDisplay = map->getMapDisplay().get()),
		rData(MapRenderingData()) {}
	~MapUI();

	void initialize(SDL_Renderer* renderer, SDL_Texture* spritesheet);

	void render(SDL_Rect& viewport);

	void processMouseScroll(int offset, bool ctrlDown); //changes scale 

	TileCoordinates findMapTileFromScreenCoords(int x, int y);
};
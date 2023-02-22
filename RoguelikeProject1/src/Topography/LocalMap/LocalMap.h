#ifndef LOCALMAP_H
#define LOCALMAP_H


#include "../TerrainMap/TerrainMap.h"
#include "../MapDisplay/MapDisplay.h"
#include "../../GameObjects/Actors/Actor.h"
#include "../../Enums/PlayerCommands.h"
#include "../../Algorithms/Pathfinding/PathfindingRoute.h"


/* Class for Local Maps */
class LocalMap {
private:
	int width;
	int height;

	TileCoordinates mouseTile;
	TileCoordinates playerTile;

	PathfindingRoute pathToMouseTile;
	
	MapDisplay mapDisplay;
	std::vector<int16_t> visibleIndices;
	bool needToUpdateDisplay;

	TerrainMap terrainMap;

	std::vector<Actor*> actors;
	std::vector<std::vector<Item*>> items;

	void updateHighlightedTiles();

	void resetVisibleTileDisplays();
	void updateVisibleTileDisplays();

	TileDisplay* getDisplayAt(int index);
	TileDisplay* getDisplayAt(TileCoordinates location);
	void setDisplayAt(int index, TileDisplay* display);
	void setDisplayAt(TileCoordinates location, TileDisplay* display);

	bool hasReticle(int index);
	bool hasReticle(TileCoordinates tile);
	void setHasReticle(int index, bool hasReticle);
	void setHasReticle(TileCoordinates tile, bool hasReticle);

public:
	LocalMap(int width, int height);

	MapDisplay* getMapDisplay();
	void updateMapDisplay();

	int coordsToTileIndex(TileCoordinates coordinates);
	TileCoordinates tileIndexToCoords(int index);

	int getWidth();
	int getHeight();

	bool isInMapBounds(TileCoordinates location);
	void setTerrainAt(int index, TileDisplay* display, bool traversible, bool opaque);
	void setTerrainAt(TileCoordinates location, TileDisplay* display, bool traversible, bool opaque);
	bool isTraversibleAt(int index);
	bool isTraversibleAt(TileCoordinates location);
	bool isOpaqueAt(int index);
	bool isOpaqueAt(TileCoordinates location);

	bool thereIsAnActorAt(int index);
	bool thereIsAnActorAt(TileCoordinates location);
	Actor* getActorAt(int index);
	Actor* getActorAt(TileCoordinates location);
	void setActorAt(int index, Actor* actor);
	void setActorAt(TileCoordinates location, Actor* actor);
	void setPlayerLocation(Actor* player, TileCoordinates newLocation);

	TileCoordinates getFocusTileLocation();
	void setFocusTileLocation(TileCoordinates location);
	void stopLooking();
	void setLookTile(TileCoordinates newCoords);
	void setLookTile(TileCoordinates oldCoords, TileCoordinates newCoords);

	void makeVisible(TileCoordinates location);

	void setMouseTile(TileCoordinates coordinates);
	PathfindingRoute getRouteToMouseTile();

	void flagNeedToUpdateDisplay();
};


#endif
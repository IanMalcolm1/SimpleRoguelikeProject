#ifndef LOCALMAP_H
#define LOCALMAP_H


#include "../TerrainMap/TerrainMap.h"
#include "../MapDisplay/MapDisplay.h"
#include "../../GameObjects/Actors/Actor.h"
#include "../../Enums/PlayerCommands.h"
#include "../../Algorithms/Pathfinding/PathfindingRoute.h"


enum MovementState {
	MOVEMENT_PLAYER,
	MOVEMENT_FOCUSTILE
};

/* Class for Local Maps */
class LocalMap {
private:
	int width;
	int height;

	MovementState movementState;

	PathfindingRoute pathToMouseTile;

	TileCoordinates mouseTile;
	TileCoordinates playerTile;
	
	std::shared_ptr<MapDisplay> mapDisplay;
	std::vector<int16_t> visibleIndices;
	bool needToUpdateDisplay;

	std::unique_ptr<TerrainMap> terrainMap;

	std::unique_ptr<Actor*[]> actors;

	std::unique_ptr<std::vector<Item>[]> items;

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

	std::shared_ptr<MapDisplay> getMapDisplay();
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

	void makeVisible(TileCoordinates location);

	//Return value indicates whether a turn needs to be run
	bool attemptPlayerMovement(Actor* player, PlayerCommand direction);
	void switchMovementState();

	void setMouseTile(TileCoordinates coordinates);

	void flagNeedToUpdateDisplay();
};


#endif
#include "LocalMap.h"
#include "../../Algorithms/FieldOfView/FoV.h"
#include "../../GameObjects/Actors/Actor.h"
#include "../../Algorithms/Pathfinding/Pathfinding.h"

/* Local Map Functions */

LocalMap::LocalMap(int width, int height) {
	this->width = width;
	this->height = height;

	movementState = MOVEMENT_PLAYER;

	needToUpdateDisplay = true;

	mouseTile.x = mouseTile.y = -1;
	playerTile.x = playerTile.y = 0;

	mapDisplay = std::make_shared<MapDisplay>(width, height);

	terrainMap = std::make_unique<TerrainMap>(width, height);

	actors = std::make_unique<Actor*[]>(width * height);

	items = std::make_unique<std::vector<Item>[]>(width * height);
}


/* Graphics */

std::shared_ptr<MapDisplay> LocalMap::getMapDisplay() {
	return mapDisplay;
}


void LocalMap::updateMapDisplay() {
	if (needToUpdateDisplay) {
		resetVisibleTileDisplays();

		FoV::calcPlayerFoV(this, playerTile);

		updateVisibleTileDisplays();
	}
	
	updateHighlightedTiles();

	needToUpdateDisplay = false;
}


void LocalMap::updateHighlightedTiles() {
	pathToMouseTile.resetProgress();
	while (pathToMouseTile.hasNextTile()) {
		TileCoordinates tile = pathToMouseTile.getNextTile();
		setHasReticle(tile, false);
	}

	if (mouseTile.x == -1 || mouseTile.y == -1) {
		return;
	}

	Pathfinding::bresenhamLine(playerTile, mouseTile, this, &pathToMouseTile);

	while (pathToMouseTile.hasNextTile()) {
		TileCoordinates tile = pathToMouseTile.getNextTile();
		setHasReticle(tile, true);
	}
}




/* Utilities */

int LocalMap::getWidth() { return width; }
int LocalMap::getHeight() { return height; }


int LocalMap::coordsToTileIndex(TileCoordinates coordinates) {
	if (coordinates.x < 0 || coordinates.y < 0 || coordinates.x >= width || coordinates.x >= height) {
		printf("Coords to tile ID out of bounds\n");
		return -1;
	}

	return coordinates.y * width + coordinates.x;
}

TileCoordinates LocalMap::tileIndexToCoords(int index) {
	TileCoordinates coordinates;

	if (index <0 || index>width * height) {
		coordinates.x = coordinates.y = -1;
		return coordinates;
	}

	coordinates.x = index % width;
	coordinates.y = index / width;
	return coordinates;
}


TileDisplay* LocalMap::getDisplayAt(int index) {
	if (actors[index] != nullptr) {
		Actor* test = actors[index];
		return actors[index]->getDisplay();
	}

	if (items[index].size() > 0) {
		return items[index][0].getDisplay();
	}

	return terrainMap->getDisplayAtIndex(index);
}
TileDisplay* LocalMap::getDisplayAt(TileCoordinates location) {
	return getDisplayAt(coordsToTileIndex(location));
}
void LocalMap::setDisplayAt(int index, TileDisplay* display) {
	mapDisplay->setDisplayAt(index, display);
}
void LocalMap::setDisplayAt(TileCoordinates location, TileDisplay* display) {
	mapDisplay->setDisplayAt(coordsToTileIndex(location), display);
}

bool LocalMap::hasReticle(int index) { return mapDisplay->hasReticle(index); }
bool LocalMap::hasReticle(TileCoordinates tile) { return mapDisplay->hasReticle(coordsToTileIndex(tile)); }
void LocalMap::setHasReticle(int index, bool value) { mapDisplay->setHasReticle(index, value); }
void LocalMap::setHasReticle(TileCoordinates tile, bool value) {
	mapDisplay->setHasReticle(coordsToTileIndex(tile), value);
}

void LocalMap::setPlayerLocation(Actor* player, TileCoordinates newLocation) {
	actors[coordsToTileIndex(player->getLocation())] = nullptr;

	player->setLocation(newLocation);

	playerTile = newLocation;
	
	actors[coordsToTileIndex(newLocation)] = player;
	mapDisplay->setFocusTile(newLocation);
}


bool LocalMap::isInMapBounds(TileCoordinates location) {
	return (location.x>-1 && location.y>-1 && location.x<width && location.y<height);
}

void LocalMap::setTerrainAt(int index, TileDisplay* display, bool traversible, bool opaque) {
	terrainMap->setTile(index, display, traversible, opaque);
}
void LocalMap::setTerrainAt(TileCoordinates location, TileDisplay* display, bool traversible, bool opaque) {
	if (!isInMapBounds(location)) {
		printf("setTerrainAt() coordinates out of bounds!\n");
		return;
	}
	setTerrainAt(coordsToTileIndex(location), display, traversible, opaque);
}

bool LocalMap::isTraversibleAt(int index) {
	return (terrainMap->isTraversibleAtIndex(index) && actors[index]==nullptr);
}
bool LocalMap::isTraversibleAt(TileCoordinates location) {
	if (!isInMapBounds(location)) {
		printf("isTraversibleAt() coordinates out of bounds!\n");
		return false;
	}
	return terrainMap->isTraversibleAtIndex(coordsToTileIndex(location));
}

bool LocalMap::isOpaqueAt(int index) { return terrainMap->isOpaqueAtIndex(index); }
bool LocalMap::isOpaqueAt(TileCoordinates location) {
	if (!isInMapBounds(location)) {
		printf("isOpaqueAt() coordinates out of bounds!\n");
		return false;
	}
	return terrainMap->isOpaqueAtIndex(coordsToTileIndex(location));
}

bool LocalMap::thereIsAnActorAt(int index) { return actors[index] != nullptr; }
bool LocalMap::thereIsAnActorAt(TileCoordinates location) {
	if (!isInMapBounds(location)) {
		printf("thereIsAnActorAt() coordinates out of bounds!\n");
		return false;
	}
	return actors[coordsToTileIndex(location)] != nullptr;
}

Actor* LocalMap::getActorAt(int index) { return actors[index]; }
Actor* LocalMap::getActorAt(TileCoordinates location) {
	if (!isInMapBounds(location)) {
		printf("getActorAt() coordinates out of bounds!\n");
		return nullptr;
	}
	return getActorAt( coordsToTileIndex(location) );
}

void LocalMap::setActorAt(int index, Actor* actor) { actors[index] = actor; }
void LocalMap::setActorAt(TileCoordinates location, Actor* actor) {
	if (!isInMapBounds(location)) {
		printf("setActorAt() coordinates out of bounds!\n");
		return;
	}
	setActorAt(coordsToTileIndex(location), actor);
}

void LocalMap::makeVisible(TileCoordinates location) {
	if (!isInMapBounds(location)) {
		printf("setVisible() coordinates out of bounds!\n");
		return;
	}

	int index = coordsToTileIndex(location);

	mapDisplay->setVisibility(index, true);
	mapDisplay->setSeen(index, true);

	visibleIndices.push_back(index);
}

void LocalMap::resetVisibleTileDisplays() {
	for (int index : visibleIndices) {
		mapDisplay->setVisibility(index, false);

		//reset to view of terrain
		mapDisplay->setDisplayAt(index, terrainMap->getDisplayAtIndex(index));
	}

	visibleIndices.clear();
}

void LocalMap::updateVisibleTileDisplays() {
	for (int index : visibleIndices) {
		mapDisplay->setDisplayAt(index, getDisplayAt(index));
	}
}


void LocalMap::setMouseTile(TileCoordinates location) {
	mouseTile = location;
}

void LocalMap::flagNeedToUpdateDisplay() {
	needToUpdateDisplay = true;
}


bool LocalMap::attemptPlayerMovement(Actor* player, PlayerCommand direction) {
	TileCoordinates newCoordinates;

	if (movementState == MOVEMENT_PLAYER) {
		newCoordinates = playerTile;
	}
	else {
		newCoordinates = mapDisplay->getFocusTile();
		mapDisplay->setHasReticle(coordsToTileIndex(newCoordinates), false);
	}

	if (direction < 3) {
		newCoordinates.y--;
	}
	else if (direction < 6) {
		newCoordinates.y++;
	}
	if (direction % 3 == 1) {
		newCoordinates.x++;
	}
	else if (direction % 3 == 2) {
		newCoordinates.x--;
	}

	if (!isInMapBounds(newCoordinates)) {
		return false;
	}

	if (movementState == MOVEMENT_PLAYER) {
		if (isTraversibleAt(newCoordinates) && !thereIsAnActorAt(newCoordinates)) {
			setPlayerLocation(player, newCoordinates);
			return true;
		}
	}
	else if (movementState == MOVEMENT_FOCUSTILE) {
		mapDisplay->setFocusTile(newCoordinates);
		mapDisplay->setHasReticle(coordsToTileIndex(newCoordinates), true);
		return false;
	}
}


void LocalMap::switchMovementState() {
	if (movementState == MOVEMENT_PLAYER) {
		movementState = MOVEMENT_FOCUSTILE;
		setHasReticle(playerTile, true);
	}
	else {
		movementState = MOVEMENT_PLAYER;
		setHasReticle(mapDisplay->getFocusTile(), false);
		mapDisplay->setFocusTile(playerTile);
	}
}
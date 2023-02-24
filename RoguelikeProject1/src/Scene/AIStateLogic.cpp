#include "Scene.h"
#include "../Algorithms/FieldOfView/FoV.h"
#include "../Algorithms/Pathfinding/Pathfinding.h"
#include <random>
#include <functional>


int Scene::performAction(Actor* actor) {
	AIStateID actorState = actor->getState();

	FoV::calcActorFoV(&map, actor);


	switch (actorState) {
	
	}

	return testerLogic(actor); //temporary
}

int Scene::testerLogic(Actor* actor) {
	PathfindingRoute* currentRoute = actor->getCurrentRoute();

	if (currentRoute->hasNextTile()) {
		TileCoordinates newTile = currentRoute->getNextTile();
		if (map.isTraversibleAt(newTile)) {
			moveActor(actor, newTile);
			currentRoute->incrementProgress();
			return FULL_TURN_TIME;
		}
	}

	if (rand() % 50 > 2) {
		return FULL_TURN_TIME;
	}

	std::vector<TileCoordinates>* visibleTiles = actor->getVisibleTiles();
	
	int newTileIndex;
	TileCoordinates newLocation;

	do {
		newTileIndex = rand() % visibleTiles->size();
		newLocation = visibleTiles->at(newTileIndex);
	} while (!map.isTraversibleAt(newLocation));

	Pathfinding::bresenhamLine(actor->getLocation(), visibleTiles->at(newTileIndex), &map, actor->getCurrentRoute());

	return actor->getStats()->baseMoveSpeed;
}
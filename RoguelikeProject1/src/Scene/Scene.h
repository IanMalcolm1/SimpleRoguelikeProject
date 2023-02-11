#pragma once

#include "../Topography/LocalMap/LocalMap.h"
#include "ActorPool/ActorPool.h"
#include "TurnQueue/TurnQueue.h"
#include <SDL_stdinc.h>


struct Scene {
private:
	LocalMap map;
	ActorPool actorPool;
	TurnQueue turnQueue;

	int performAction(Actor* actor);
	int testerLogic(Actor* actor);

public:
	Scene() : map(LocalMap(100, 100)), actorPool(ActorPool()), turnQueue(TurnQueue()) {};

	LocalMap* getMap();

	void processCommand(PlayerCommand command, Uint16 modification);
	void setMouseTile(TileCoordinates location);
	void updateMapDisplay();

	void runTurn();
	void createPlayerAt(TileCoordinates location);
	void createActorAt(TileCoordinates location);
	void destroyActor(Actor* actor);
	void moveActor(Actor* actor, TileCoordinates newLocation);
};
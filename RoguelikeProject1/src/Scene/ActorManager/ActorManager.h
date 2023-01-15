#pragma once

#include "../../Topography/LocalMap/LocalMap.h"
#include "../ActorPool/ActorPool.h"
#include "../TurnQueue/TurnQueue.h"
#include <SDL_stdinc.h>


//TODO: Make array of logic function pointers, update runLogic to use them
struct ActorManager {
private:
	std::shared_ptr<LocalMap> map;
	std::unique_ptr<ActorPool> actorPool;
	std::unique_ptr<TurnQueue> turnQueue;

	int performAction(Actor* actor);
	int testerLogic(Actor* actor);

public:
	ActorManager(std::shared_ptr<LocalMap> map);

	void processCommand(PlayerCommand command, Uint16 modification);

	void runTurn();
	void createPlayerAt(TileCoordinates location);
	void createActorAt(TileCoordinates location);
	void destroyActor(Actor* actor);
	void moveActor(Actor* actor, TileCoordinates newLocation);
};
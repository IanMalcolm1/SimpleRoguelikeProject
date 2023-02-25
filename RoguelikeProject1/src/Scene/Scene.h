#pragma once

#include "../Topography/LocalMap/LocalMap.h"
#include "ActorManagement/ActorPool/ActorPool.h"
#include "ActorManagement/TurnQueue/TurnQueue.h"
#include <SDL_stdinc.h>
#include "../UIScreens/ConfirmerUI/InputConfirmer.h"
#include "../PlayerManager/PlayerManager.h"


struct Scene {
private:
	LocalMap map;
	PlayerManager playerManager;
	ActorPool actorPool;
	TurnQueue turnQueue;

	PathfindingRoute pathToMouseTile;

	InputConfirmer confirmer;

	bool alreadyRanTurn;

	int performAction(Actor* actor);
	int testerLogic(Actor* actor);

	void runTurn();


public:
	Scene() : map(LocalMap(100, 100)), actorPool(ActorPool()), turnQueue(TurnQueue()), 
		confirmer(InputConfirmer()), playerManager(PlayerManager()), alreadyRanTurn(false) {
		playerManager.initialize(&map, &confirmer, &turnQueue);
	};

	LocalMap* getMap();
	PlayerManager* getPlayerManager();
	InputConfirmer* presentConfirmationSignaller();

	void processCommand(PlayerCommand command, Uint16 modification);
	void updateMapDisplay();

	void setPlayerAt(TileCoordinates location);
	void createActorAt(TileCoordinates location);
	void destroyActor(Actor* actor);
	void moveActor(Actor* actor, TileCoordinates newLocation);

	void startAutoMove();
	void runTurnIfAutoMoving();
};
#include "ActorManager.h"
#include <random>


ActorManager::ActorManager(std::shared_ptr<LocalMap> map) {
	this->map = map;
	actorPool = std::make_unique<ActorPool>();
	turnQueue = std::make_unique<TurnQueue>();

	Actor player = Actor();
	actorPool->setPlayer(player);



	srand(time(0));
}

void ActorManager::processCommand(PlayerCommand command, Uint16 modification) {
	bool needToRunTurn = false;

	//process player move
	if (command < 9) {
		Actor* player = actorPool->getPlayer();

		needToRunTurn = map->attemptPlayerMovement(player, command);

		if (needToRunTurn) {
			turnQueue->insert(player, player->getStats()->baseMoveSpeed);
		}
	}

	else if (command == PC_WAIT) {
		Actor* player = actorPool->getPlayer();
		turnQueue->insert(player, FULL_TURN_TIME);
		needToRunTurn = true;
	}

	else if (command == PC_TOGGLE_LOOK) {
		map->switchMovementState();
	}


	if (needToRunTurn) {
		runTurn();
	}
}



void ActorManager::runTurn() {
	Actor* currentActor = turnQueue->pop();
	while (currentActor != nullptr && !currentActor->isPlayer()) {
		int timeTaken = performAction(currentActor);
		turnQueue->insert(currentActor, timeTaken);

		currentActor = turnQueue->pop();
	}

	map->flagNeedToUpdateDisplay();
}

void ActorManager::createPlayerAt(TileCoordinates location) {
	TileDisplay actorDisplay;
	actorDisplay.backColor.set(0, 0, 0);
	actorDisplay.symbolColor.set(255, 255, 255);
	actorDisplay.symbol = ASYM_AT;

	Actor player = Actor(location, actorDisplay, AI(), true);

	actorPool->setPlayer(player);

	map->setPlayerLocation(actorPool->getPlayer(), location);
}

void ActorManager::createActorAt(TileCoordinates location) {
	Actor newActor = Actor();

	newActor.setLocation(location);

	Actor* newActorPointer = actorPool->insert(newActor);

	map->setActorAt(location, newActorPointer);
	turnQueue->insert(newActorPointer, 0);
}

void ActorManager::destroyActor(Actor* actor) {
	map->setActorAt(actor->getLocation(), nullptr);

	turnQueue->remove(actor);

	actorPool->kill(actor);
}

void ActorManager::moveActor(Actor* actor, TileCoordinates newLocation) {
	map->setActorAt(actor->getLocation(), nullptr);

	actor->setLocation(newLocation);

	map->setActorAt(newLocation, actor);
}
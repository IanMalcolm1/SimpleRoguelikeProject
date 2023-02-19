#include "Scene.h"
#include <random>


LocalMap* Scene::getMap() {	return &map; }

void Scene::processCommand(PlayerCommand command, Uint16 modification) {
	bool needToRunTurn = false;

	//process player move
	if (command < 9) {
		Actor* player = actorPool.getPlayer();

		needToRunTurn = map.attemptPlayerMovement(player, command);

		if (needToRunTurn) {
			turnQueue.insert(player, player->getStats()->baseMoveSpeed);
		}
	}

	else if (command == PC_WAIT) {
		Actor* player = actorPool.getPlayer();
		turnQueue.insert(player, FULL_TURN_TIME);
		needToRunTurn = true;
	}

	else if (command == PC_TOGGLE_LOOK) {
		map.switchMovementState();
	}


	if (needToRunTurn) {
		runTurn();
	}
}

void Scene::updateMapDisplay() {
	map.updateMapDisplay();
}



void Scene::runTurn() {
	Actor* currentActor = turnQueue.pop();
	while (currentActor != nullptr && !currentActor->isPlayer()) {
		int timeTaken = performAction(currentActor);
		turnQueue.insert(currentActor, timeTaken);

		currentActor = turnQueue.pop();
	}

	map.flagNeedToUpdateDisplay();
}

void Scene::createPlayerAt(TileCoordinates location) {
	TileDisplay actorDisplay;
	actorDisplay.backColor.set(0, 0, 0);
	actorDisplay.symbolColor.set(255, 255, 255);
	actorDisplay.symbol = ASYM_AT;

	Actor player = Actor(location, actorDisplay, AI(), true);

	actorPool.setPlayer(player);

	map.setPlayerLocation(actorPool.getPlayer(), location);
}

void Scene::createActorAt(TileCoordinates location) {
	Actor newActor = Actor();

	newActor.setLocation(location);

	Actor* newActorPointer = actorPool.insert(newActor);

	map.setActorAt(location, newActorPointer);
	turnQueue.insert(newActorPointer, 0);
}

void Scene::destroyActor(Actor* actor) {
	map.setActorAt(actor->getLocation(), nullptr);

	turnQueue.remove(actor);

	actorPool.kill(actor);
}

void Scene::moveActor(Actor* actor, TileCoordinates newLocation) {
	map.setActorAt(actor->getLocation(), nullptr);

	actor->setLocation(newLocation);

	map.setActorAt(newLocation, actor);
}
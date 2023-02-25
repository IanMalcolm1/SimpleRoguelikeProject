#include "Scene.h"
#include <random>


LocalMap* Scene::getMap() {	return &map; }

PlayerManager* Scene::getPlayerManager() {
	return &playerManager;
}

InputConfirmer* Scene::presentConfirmationSignaller() {
	return &confirmer;
}

void Scene::processCommand(PlayerCommand command, Uint16 modification) {
	if (confirmer.isAwaiting()) {
		int confirmationCode = confirmer.getConfirmation();
		if (confirmationCode == CONF_WAITING) {
			return;
		}
		else if (confirmationCode == CONF_CONFIRMED) {
			command = confirmer.getCommand();
		}
	}

	bool needToRunTurn = false;

	//process player move
	if (command < 9) {
		needToRunTurn = playerManager.processDirectionalCommand(command);

		Actor* player = playerManager.getPlayer();

		if (needToRunTurn) {
			turnQueue.insert(player, player->getStats()->baseMoveSpeed);
		}
	}

	else if (command == PC_WAIT) {
		Actor* player = playerManager.getPlayer();
		turnQueue.insert(player, FULL_TURN_TIME);
		needToRunTurn = true;
	}

	else if (command == PC_TOGGLE_LOOK) {
		playerManager.updateInputState(command);
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

void Scene::setPlayerAt(TileCoordinates location) {
	playerManager.placePlayer(location);
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
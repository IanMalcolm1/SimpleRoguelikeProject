#include "Actor.h"


Item Actor::generateCorpse() {

	return Item();
}

void Actor::setLocation(TileCoordinates coordinates) {
	location = coordinates;
}

void Actor::setDisplay(TileDisplay* newDisplay) {
	display.copy(newDisplay);
}

TileCoordinates Actor::getLocation() { return location; }
void Actor::setPoolIndex(int index) { poolIndex = index; }
int Actor::getPoolIndex() { return poolIndex; }
TileDisplay* Actor::getDisplay() { return &display; }
StatBlock* Actor::getStats() { return &stats; }

std::vector<TileCoordinates>* Actor::getVisibleTiles() { return &visibleTiles; }
std::vector<TileCoordinates>* Actor::getVisibleActorLocations() { return &visibleActorLocations; }

PathfindingRoute* Actor::getCurrentRoute() { return &currentRoute; }

void Actor::clearVisibilityArrays() {
	visibleTiles.clear();
	visibleActorLocations.clear();
}
void Actor::addVisibleTile(TileCoordinates tile) { visibleTiles.push_back(tile); }
void Actor::addVisibleActorLocation(TileCoordinates location) { visibleActorLocations.push_back(location); }

AIStateID Actor::getState() {
	AIStateID newState = ai.getState(currentState);
	if (currentState != newState) {
		currentRoute.clear();
	}
	currentState = newState;
	return newState;
}

void Actor::setState(AIStateID stateID) {
	currentState = stateID;
}

void Actor::makePlayer() {
	player = true;
}

bool Actor::isPlayer() { return player;  }

void Actor::addIdleSubstate(AIState state) {
	ai.addIdleSubstate(state);
}
void Actor::addAttackingSubstate(AIState state) {
	ai.addAttackingSubstate(state);
}

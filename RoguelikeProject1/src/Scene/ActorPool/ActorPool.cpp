#include "ActorPool.h"


void ActorPool::setPlayer(Actor player) {
	player.setPoolIndex(0);
	actors[0] = player;
}

Actor* ActorPool::getPlayer() {
	return &actors[0];
}

Actor* ActorPool::insert(Actor actor) {
	int index;
	if (reusableIndices.size() == 0) {
		index = dirtySlots;
		dirtySlots++;
	}
	else {
		index = reusableIndices.back();
		reusableIndices.pop_back();
	}

	actor.setPoolIndex(index);
	actors[index] = actor;

	return &actors[index];
}


void ActorPool::kill(Actor* actor) {
	int index = actor->getPoolIndex();
	actors[index] = Actor();

	reusableIndices.push_back(index);
}
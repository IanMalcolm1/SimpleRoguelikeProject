#pragma once

#include <memory>
#include <array>
#include "../../GameObjects/Actors/Actor.h"


#define MAX_ACTORS 1024

//Note the player actor always has ID 0
class ActorPool {
private:
	std::array<Actor, MAX_ACTORS> actors;
	int dirtySlots;

	std::vector<int16_t> reusableIndices;
	
public:
	ActorPool() : dirtySlots(1) {};

	void setPlayer(Actor player);
	Actor* getPlayer();
	Actor* insert(Actor actor);
	void kill(Actor* actor);
};
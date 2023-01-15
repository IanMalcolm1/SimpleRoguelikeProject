#pragma once

#include "../Topography/LocalMap/LocalMap.h"
#include "ActorManager/ActorManager.h"
#include "../Enums/PlayerCommands.h"
#include <SDL_stdinc.h>


class Scene {
private:
	std::shared_ptr<LocalMap> map;

	std::shared_ptr<ActorManager> actorManager;

public:
	Scene();

	std::shared_ptr<MapDisplay> getMapDisplay();
	void updateMapDisplay();

	std::shared_ptr<LocalMap> getMap();

	void processCommand(PlayerCommand command, Uint16 modification);

	void setMouseTile(TileCoordinates coordinates);
};
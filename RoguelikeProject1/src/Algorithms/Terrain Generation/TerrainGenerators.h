#pragma once

#include "../../Scene/Scene.h"

struct TerrainGenerators {
	static void rectangleRooms(std::shared_ptr<LocalMap> map, std::shared_ptr<ActorManager> manager, int numRectangles, int maxRectangleSideLength);
};
#pragma once

#include "../../Topography/LocalMap/LocalMap.h"
#include "PathfindingRoute.h"

class Pathfinding {
public:
	static void bresenhamLine(TileCoordinates startTile, TileCoordinates endTile, LocalMap* localMap, PathfindingRoute* route);
};
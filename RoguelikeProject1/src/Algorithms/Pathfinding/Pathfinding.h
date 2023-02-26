#pragma once

#include "../../Topography/LocalMap/LocalMap.h"
#include "PathfindingRoute.h"

struct PathingNode {
	TileCoords tile;
	int distance;

	bool operator < (const PathingNode& node) {
		return distance < node.distance;
	}
	bool operator > (const PathingNode& node) {
		return distance > node.distance;
	}
};



class Pathfinding {
public:
	static void makeLineRoute(TileCoords startTile, TileCoords endTile, LocalMap* map, bool (LocalMap::*traversible)(TileCoords), PathingRoute* route);
	static void makeAStarRoute(TileCoords startTile, TileCoords endTile, LocalMap* map, PathingRoute& route);
};
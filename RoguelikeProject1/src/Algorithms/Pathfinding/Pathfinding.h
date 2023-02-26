#pragma once

#include "../../Topography/LocalMap/LocalMap.h"
#include "PathfindingRoute.h"
#include <vector>

struct PathingNode {
	TileCoords tile;
	int distance;
	int priority;

	PathingNode(TileCoords tile = {-1,-1}, int dist = INT_MAX, int priority = INT_MAX) : tile(tile),
		distance(dist), priority(priority) {};

	bool operator < (const PathingNode& node) const {
		return priority < node.priority;
	}
	bool operator > (const PathingNode& node) const {
		return priority > node.priority;
	}
};

class TileCoordsHash {
public:
	size_t operator()(const TileCoords tile) const {
		return tile.x + tile.y * 256;
	}
};


class Pathfinding {
public:
	static std::vector<TileCoords> getSurroundingTiles(TileCoords center);
	static void calcPlayerPathingRoute(TileCoords startTile, TileCoords endTile, LocalMap* map, PathingRoute* route);
	static void makeLineRoute(TileCoords startTile, TileCoords endTile, LocalMap* map, bool (LocalMap::*traversible)(TileCoords), PathingRoute* route);
	static void makeAStarRoute(TileCoords startTile, TileCoords endTile, LocalMap* map, PathingRoute& route);
};
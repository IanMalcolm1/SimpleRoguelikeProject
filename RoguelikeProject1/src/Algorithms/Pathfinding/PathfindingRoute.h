#pragma once

#include <vector>
#include "../../GameObjects/TileCoordinates.h"

class PathfindingRoute {
private:
	std::vector<TileCoordinates> route;
	int progress;

public:
	PathfindingRoute() : progress(0) {};

	void clear();
	void resetProgress();
	void addTile(TileCoordinates tile);
	TileCoordinates getNextTile();
	int size();
	bool hasNextTile();
};
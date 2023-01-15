#include "PathfindingRoute.h"

void PathfindingRoute::clear() {
	progress = 0;
	route.clear();
}

void PathfindingRoute::resetProgress() { progress = 0; }

void PathfindingRoute::addTile(TileCoordinates tile) { route.push_back(tile); }

TileCoordinates PathfindingRoute::getNextTile() {
	int temp = progress;
	progress++;
	return route[temp];
}

int PathfindingRoute::size() { return route.size(); }

bool PathfindingRoute::hasNextTile() {
	return (route.size() > 0 && progress < route.size());
}
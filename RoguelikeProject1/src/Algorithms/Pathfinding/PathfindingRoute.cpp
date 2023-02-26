#include "PathfindingRoute.h"

void PathingRoute::clear() {
	progress = 0;
	route.clear();
}

void PathingRoute::resetProgress() { progress = 0; }

void PathingRoute::addTile(TileCoords tile) { route.push_back(tile); }

TileCoords PathingRoute::getNextTile() {
	int temp = progress;
	return route[temp];
}

void PathingRoute::incrementProgress() {
	progress++;
}

int PathingRoute::size() { return route.size(); }

bool PathingRoute::hasNextTile() {
	return (route.size() > 0 && progress < route.size());
}

TileCoords PathingRoute::endTile() {
	return route.back();
}

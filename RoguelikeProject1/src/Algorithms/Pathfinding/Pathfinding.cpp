#include "Pathfinding.h"
#include <functional>

void Pathfinding::makeLineRoute(TileCoords startTile, TileCoords endTile, LocalMap* localMap, bool (LocalMap::*traversible)(TileCoords), PathingRoute* route) {
	/*
	Based on the following article: https://www.cs.helsinki.fi/group/goa/mallinnus/lines/bresenh.html
	And this: https://github.com/anushaihalapathirana/Bresenham-line-drawing-algorithm/blob/master/src/index.js
	*/

	route->clear();

	int deltaX = abs(endTile.x - startTile.x);
	int deltaY = abs(endTile.y - startTile.y);

	int modifiedError = 0;

	int loopIncrement, secondaryIncrement;

	if (deltaY <= deltaX) {

		int y = startTile.y;

		loopIncrement = startTile.x < endTile.x ? 1 : -1;
		secondaryIncrement = startTile.y < endTile.y ? 1 : -1;

		//one iteration outside to skip start tile
		if (2 * (modifiedError + deltaY) < deltaX) {
			modifiedError += deltaY;
		}
		else {
			y += secondaryIncrement;
			modifiedError += deltaY - deltaX;
		}
		int x = startTile.x;
		x += loopIncrement;

		for (x; x != endTile.x + loopIncrement; x += loopIncrement) {
			if ( !std::invoke(traversible, localMap, TileCoords(x,y)) ) {
				break;
			}
			route->addTile({ x,y });

			if (2 * (modifiedError + deltaY) < deltaX) {
				modifiedError += deltaY;
			}
			else {
				y += secondaryIncrement;
				modifiedError += deltaY - deltaX;
			}
		}

	}

	else {
		int x = startTile.x;

		loopIncrement = startTile.y < endTile.y ? 1 : -1;
		secondaryIncrement = startTile.x < endTile.x ? 1 : -1;

		//one iteration outside to skip first tile
		if (2 * modifiedError + deltaX < deltaY) {
			modifiedError += deltaX;
		}
		else {
			x += secondaryIncrement;
			modifiedError += deltaX - deltaY;
		}
		int y = startTile.y;
		y += loopIncrement;
		

		for (y; y != endTile.y + loopIncrement; y += loopIncrement) {
			if ( !std::invoke(traversible, localMap, TileCoords(x, y)) ) {
				break;
			}
			route->addTile({ x,y });

			if (2 * modifiedError + deltaX < deltaY) {
				modifiedError += deltaX;
			}
			else {
				x += secondaryIncrement;
				modifiedError += deltaX - deltaY;
			}
		}

	}
}

void Pathfinding::makeAStarRoute(TileCoords startTile, TileCoords endTile, LocalMap* map, PathingRoute& route) {

}

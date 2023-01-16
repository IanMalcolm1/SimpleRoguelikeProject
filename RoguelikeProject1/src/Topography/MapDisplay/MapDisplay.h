#pragma once

#include <memory>
#include "../../GraphicsThings/TileDisplay.h"
#include "../../GameObjects/TileCoordinates.h"


struct MapDisplayTile {
	TileDisplay display;
	bool isVisible;
	bool hasBeenSeen;
	bool hasReticle;
};


/* Struct for buffer arrays */
class MapDisplay {
private:
	int width, height;

	TileCoordinates focusTile;

	std::unique_ptr<MapDisplayTile[]> tiles;

public:
	MapDisplay(int width, int height);
	~MapDisplay();

	int getWidth();
	int getHeight();

	TileDisplay* getDisplay(int index);
	void setDisplayAt(int index, TileDisplay* display);

	TileCoordinates getFocusTile();
	void setFocusTile(TileCoordinates tile);

	//returns whether the tile at the given coordinates is visible to the player
	bool isVisible(int index);
	bool hasBeenSeen(int index);
	bool hasReticle(int index);

	void setVisibility(int index, bool value);
	void setSeen(int index, bool value);
	void setHasReticle(int index, bool value);
};
#include "MapDisplay.h"

MapDisplay::MapDisplay(int width, int height) {
	this->width = width;
	this->height = height;

	focusTile = { 0,0 };

	tiles = std::make_unique<MapDisplayTile[]>(width * height);

	for (int i = 0; i < width * height; i++) {
		tiles[i].isVisible = false;
		tiles[i].hasBeenSeen = false;
		tiles[i].hasReticle = false;
	}
}

MapDisplay::~MapDisplay() {
	printf("Buffer map destructor called\n");
}

int MapDisplay::getWidth() { return width; }
int MapDisplay::getHeight() { return height; }

TileDisplay* MapDisplay::getDisplay(int index) { return &tiles[index].display; }
void MapDisplay::setDisplayAt(int index, TileDisplay* display) { tiles[index].display.copy(display); }

TileCoordinates MapDisplay::getFocusTile() { return focusTile; }
void MapDisplay::setFocusTile(TileCoordinates tile) { focusTile = tile; }

bool MapDisplay::isVisible(int index) { return tiles[index].isVisible; }
bool MapDisplay::hasBeenSeen(int index) { return tiles[index].hasBeenSeen; }
bool MapDisplay::hasReticle(int index) { return tiles[index].hasReticle; }

void MapDisplay::setVisibility(int index, bool value) { tiles[index].isVisible = value; }
void MapDisplay::setSeen(int index, bool value) { tiles[index].hasBeenSeen = value; }
void MapDisplay::setHasReticle(int index, bool value) { tiles[index].hasReticle = value; }

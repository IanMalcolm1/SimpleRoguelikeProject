#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SDL.h>
#include <SDL_image.h>
#include "../../Topography/LocalMap/LocalMap.h"
#include "../DisplayInformation/DisplayedTilesMap.h"
#include "../../GameLog/GameLog.h"
#include "../../DebugLogger/DebugLogger.h"


enum GameWindowState {
	WINDOW_STATE_MAP,
	WINDOW_STATE_MAIN_MENU,
	WINDOW_STATE_INVENTORY,
	WINDOW_STATE_CHARACTER_SHEET
};

struct MapViewports {
	SDL_Rect map;
	SDL_Rect messages;
	SDL_Rect playerInfo;
};

class GameWindow {
private:
	DebugLogger debugLogger;

	GameWindowState state = WINDOW_STATE_MAP;

	SDL_Rect screenDimensions;
	MapViewports viewports;

	int mapScale;

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* spriteSheet;

	std::unique_ptr<DisplayedTilesMap> displayedTilesMap;

	//Renders a buffer map. Current return type is temporary.
	void renderMap(std::shared_ptr<MapDisplay> mapDisplay);
	MapRenderingData calculateMapRenderingDimensions(SDL_Rect viewport, int mapWidthTiles, int mapHeightTiles, TileCoordinates focusTile);

	void renderRecentMessages();
	std::string makeFormattedMessage(int maxLettersPerLine, std::string message);

	void renderPlayerInfo();

	void updateMapViewports();
	void resetRendererAndDrawBorder(SDL_Rect& currentViewport);

public:
	GameWindow(int scale = 10, int windowWidth = 1600, int windowHeight = 1200);
	~GameWindow();

	GameWindowState getState();
	void setState(GameWindowState state);

	bool initialize();
	void update(std::shared_ptr<MapDisplay> mapDisplay);

	void changeMapScale(int offset);
	void updateWindowDimensions(int width, int height);

	inline TileCoordinates findMapTileFromScreenCoordinates(int x, int y) {
		return displayedTilesMap->findTileFromScreenCoordinates(x, y);
	}
};

#endif
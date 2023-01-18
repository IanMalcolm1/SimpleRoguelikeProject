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


struct TextRenderingSpecifications {
	int fontSize;
	int fontSizePixels;
	int margin;
	int lineSpacing;
	int messageSpacing;
	int maxLettersPerLine;
	int scrollOffset;

	TextRenderingSpecifications() : fontSize(0), fontSizePixels(0), margin(0), lineSpacing(0),
		messageSpacing(0), maxLettersPerLine(0), scrollOffset(0) {};

	TextRenderingSpecifications(int fontSize, int margin = 16) : fontSize(fontSize), 
		fontSizePixels(8*fontSize), margin(margin), lineSpacing(fontSizePixels/4),
		messageSpacing(fontSizePixels/2), scrollOffset(0), maxLettersPerLine(0) {};
	
	void changeFontSize(int fontSize);
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

	std::shared_ptr<GameLog> messageLog;

	//TODO: remove this
	TextRenderingSpecifications textSpecs;



	void renderMap(std::shared_ptr<MapDisplay> mapDisplay);
	MapRenderingData calculateMapRenderingDimensions(SDL_Rect viewport, int mapWidthTiles, int mapHeightTiles, TileCoordinates focusTile);

	void renderRecentMessages();
	std::pair<std::string, int> makeFormattedMessage(int maxLettersPerLine, std::string message);
	void renderIndividualMessage(GameMessage message, SDL_Rect& destinationRect);

	void renderPlayerInfo();

	void updateMapViewports();
	void resetRendererAndDrawBorder(SDL_Rect& currentViewport);

	void changeMessagesScrollOffset(int offset);
	void changeMapScale(int offset);

public:
	GameWindow(std::shared_ptr<GameLog> messageLog, int scale = 10, int windowWidth = 1600, int windowHeight = 1200);
	~GameWindow();

	GameWindowState getState();
	void setState(GameWindowState state);

	bool initialize();
	void update(std::shared_ptr<MapDisplay> mapDisplay);

	void processMouseScroll(int x, int y, int scrollOffset);
	void updateWindowDimensions(int width, int height);

	inline TileCoordinates findMapTileFromScreenCoordinates(int x, int y) {
		return displayedTilesMap->findTileFromScreenCoordinates(x, y);
	}
};

#endif
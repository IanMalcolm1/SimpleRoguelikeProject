#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SDL.h>
#include <SDL_image.h>
#include "../../Topography/LocalMap/LocalMap.h"
#include "../../GameLog/GameLog.h"
#include "../../DebugLogger/DebugLogger.h"
#include "../../UIScreens/MessagesUI/MessagesUI.h"
#include "../../UIScreens/MapUI/MapUI.h"


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

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* spritesheet;

	MapUI mapUI;
	MessagesUI messagesUI;


	void renderMap();
	void renderRecentMessages();
	void renderPlayerInfo();

	void updateMapViewports();
	void resetRendererAndDrawBorder(SDL_Rect& currentViewport);

public:
	GameWindow(LocalMap* map, std::shared_ptr<GameLog> log,
		int windowWidth = 1600, int windowHeight = 1200);
	~GameWindow();

	GameWindowState getState();
	void setState(GameWindowState state);

	bool initialize();
	void update();

	void processScroll(int x, int y, int scrollOffset, bool ctrlDown);
	void updateWindowDimensions(int width, int height);

	void processCursorLocation(int x, int y);
};

#endif
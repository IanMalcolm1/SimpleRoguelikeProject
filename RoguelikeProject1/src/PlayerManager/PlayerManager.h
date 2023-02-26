#pragma once

#include "../GameObjects/Actors/Actor.h"
#include "../Enums/PlayerCommands.h"
#include "../Topography/LocalMap/LocalMap.h"
#include "../UIScreens/ConfirmerUI/InputConfirmer.h"
#include "../Scene/ActorManagement/TurnQueue/TurnQueue.h"


enum PlayerInputState {
	PLAYER_INPUT_MOVE,
	PLAYER_INPUT_LOOK,
	PLAYER_INPUT_SELECT
};

class PlayerManager {
private:
	Actor player;

	PlayerInputState inputState;

	LocalMap* map;

	InputConfirmer* sceneConfirmer;

	PathingRoute autoMoveRoute;


public:
	bool autoActing;

	PlayerManager() : player(Actor()), inputState(PLAYER_INPUT_MOVE), map(nullptr),
		sceneConfirmer(nullptr), autoActing(false) {};

	void initialize(LocalMap* map, InputConfirmer* sceneConfirmer, TurnQueue* queue);

	//returns whether the turn needs to be run
	bool processDirectionalCommand(PlayerCommand command);

	Actor* getPlayer();
	void placePlayer(TileCoords location);

	void updateInputState(PlayerCommand command);
	int doAutoAct();
	void clearAutoAct();
	void startAutoMove();
};
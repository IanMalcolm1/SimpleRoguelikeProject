#pragma once

#include "../GameObjects/Actors/Actor.h"
#include "../Enums/PlayerCommands.h"
#include "../Topography/LocalMap/LocalMap.h"
#include "../UIScreens/ConfirmationUI/InputConfirmer.h"
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


public:
	PlayerManager() : player(Actor()),
		inputState(PLAYER_INPUT_MOVE), map(nullptr), sceneConfirmer(nullptr) {};
	void initialize(LocalMap* map, InputConfirmer* sceneConfirmer, TurnQueue* queue);

	//returns whether the turn needs to be run
	bool processDirectionalCommand(PlayerCommand command);

	Actor* getPlayer();
	void placePlayer(TileCoordinates location);

	void updateInputState(PlayerCommand command);
};
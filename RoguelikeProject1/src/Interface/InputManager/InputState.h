#pragma once

#include "../../Enums/PlayerCommands.h"


struct InputState {
private:
	bool awaitingConfirmation; //Awaiting a popup
	bool repeatingCommand; //For things like player auto-move

public:
	int confirmation; //-1 is null or not received, 0 is false, 1 is true
	PlayerCommand command;

	InputState() : awaitingConfirmation(false), confirmation(-1), repeatingCommand(false), command(PC_NULL) {};

	void awaitConfirmation(PlayerCommand command);
	bool isAwaiting();

	void repeatCommand(PlayerCommand command);
	bool isRepeating();

	void clear();
};
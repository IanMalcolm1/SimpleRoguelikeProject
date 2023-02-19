#include "InputState.h"

void InputState::awaitConfirmation(PlayerCommand command) {
	awaitingConfirmation = true;
	repeatingCommand = false;
}

bool InputState::isAwaiting() {
	return awaitingConfirmation;
}

void InputState::repeatCommand(PlayerCommand command) {
	repeatingCommand = true;
	awaitingConfirmation = false;
}

bool InputState::isRepeating() {
	return repeatingCommand;
}

void InputState::clear() {
	awaitingConfirmation = false;
	confirmation = -1;
	repeatingCommand = false;
	command = PC_NULL;
}
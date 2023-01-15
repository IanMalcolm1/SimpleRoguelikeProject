#include "MessageLog.h"

std::string GameMessage::toString() {
	return text + "\t" + std::to_string(color.r) + std::to_string(color.g) + std::to_string(color.b);
}

MessageLog::MessageLog() {
	logFile.open("game_files/message_log.txt", std::ios::in | std::ios::out | std::ios::app);

	if ( !logFile.is_open() ) {
		printf("Failed to open log file.");
	}
}


MessageLog::~MessageLog() {
	logFile.close();
}


void MessageLog::sendNormalMessage(std::string message) {
	GameMessage gameMessage = GameMessage(message, { 255,255,255 });
	logFile << gameMessage.toString() << "\n";

	recentMessages.push_back(gameMessage);
}
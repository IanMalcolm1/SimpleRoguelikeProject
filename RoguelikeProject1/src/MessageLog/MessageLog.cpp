#include "MessageLog.h"


MessageLog::MessageLog() {
	logFile.open("game_files/message_log.txt", std::ios::in | std::ios::out | std::ios::app);

	if ( !logFile.is_open() ) {
		printf("Failed to open log file.");
	}
}

MessageLog::~MessageLog() {
	logFile.close();
}


void MessageLog::logGameplayMessage(std::string message) {
	//TODO: add color formatting (use <colorname> or <255255255> to start color section and </>
	//		to end color section. convert <colorname> to rgb values and use char 255 to indicate
	//		start/end of colored section

	logFile << message << "\n";

	recentMessages.push_back(message);
}
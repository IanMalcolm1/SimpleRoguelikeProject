#pragma once

#include <vector>
#include <string>
#include <fstream>
#include "../GameObjects/TileGraphics.h"


class MessageLog {
private:
	std::fstream logFile;

	std::vector<std::string> recentMessages;

public:
	MessageLog();
	~MessageLog();

	void logErrorMessage(std::string message);
	void logGameplayMessage(std::string message);
};
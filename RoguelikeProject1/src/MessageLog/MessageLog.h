#pragma once

#include <vector>
#include <string>
#include <fstream>
#include "../GameObjects/TileGraphics.h"

enum MessageType {
	MESSAGE_TYPE_MISC,
	MESSAGE_TYPE_COMBAT
};

struct GameMessage {
	std::string text;
	MyColor color;

	GameMessage(std::string text, MyColor color) : text(text), color(color) {};
	std::string toString();
};

class MessageLog {
private:
	std::fstream logFile;

	std::vector<GameMessage> recentMessages;

public:
	MessageLog();
	~MessageLog();

	void sendNormalMessage(std::string message);
};
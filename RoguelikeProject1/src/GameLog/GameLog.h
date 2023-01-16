#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include "../GraphicsThings/MyColor.h"
#include "../DebugLogger/DebugLogger.h"
#include <unordered_map>
#include "../GraphicsThings/ColorMap.h"


struct MessageColorNode {
	MyColor color;
	uint16_t endIndex;

	MessageColorNode(MyColor color, uint16_t endIndex) : color(color), endIndex(endIndex) {};
};


struct GameMessage {
private:
	std::vector<MessageColorNode> colorNodes;

public:
	std::string text;

	GameMessage(std::string text, std::vector<MessageColorNode> colorNodes) : text(text), colorNodes(colorNodes) {};

	MyColor getColorAtIndex(int index);
};


class GameLog {
private:
	std::unique_ptr<DebugLogger> debugLogger;

	std::unique_ptr<std::fstream> messageFile;

	std::vector<GameMessage> recentMessages;

	std::shared_ptr<ColorMap> colorMap;

	GameMessage makeGameMessage(std::string rawText);
	MyColor readColor(int& index, std::string& text);

	MyColor readColorByRGB(int& index, std::string& text);
	int readColorRGBValue(int& index, std::string& text);

	MyColor readColorByColorName(int& index, std::string& text);

public:
	GameLog(std::shared_ptr<ColorMap> colorMap);
	~GameLog();

	/*
	By default text is white.

	To start a colored section of text, use </colorName: or </rrrgggbbb:
	Examples: </red: or </255000000:

	To end a colored section of text, use />

	Try not to have extraneous < characters.
	*/
	void sendMessage(std::string text);

	std::vector<GameMessage>* getRecentMessages();
};
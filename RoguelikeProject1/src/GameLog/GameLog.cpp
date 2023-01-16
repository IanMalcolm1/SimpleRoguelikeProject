#include "GameLog.h"
#include <stack>


MyColor GameMessage::getColorAtIndex(int index) {
	for (MessageColorNode node : colorNodes) {
		if (index <= node.endIndex) {
			return node.color;
		}
	}
	

	printf("Message display error: Reached end of color nodes");
	return MyColor(255, 255, 255);
}


GameLog::GameLog() {
	initializeColorMap();

	debugLogger = std::make_unique<DebugLogger>();

	messageFile = std::make_unique<std::fstream>();
	messageFile->open("game_files/message_log.txt", std::ios::in | std::ios::out | std::ios::app);

	if ( !messageFile->is_open() ) {
		printf("Failed to open message log file.");
	}
}

GameLog::~GameLog() {
	messageFile->close();
}

void GameLog::initializeColorMap() {
	colorMap.insert({ "red", MyColor(255, 0, 0) });
	colorMap.insert({ "green", MyColor(0, 255, 0) });
	colorMap.insert({ "blue", MyColor(0, 0, 255) });
}

MyColor GameLog::getColorByName(std::string name) {
	try {
		return colorMap.at(name);
	}
	catch (std::out_of_range e) {
		debugLogger->log("Game message format error: Color name '"+name+"' is unrecognized");
		return MyColor(255, 255, 255);
	}
}

int GameLog::readColorRGBValue(int& index, std::string& text) {
	std::string value;

	for (int counter = 0; counter < 3; counter++) {
		if (index >= text.size()) {
			debugLogger->log("Game message format error: Reading RGB values exceeded length of string\n" + text);
			return 0;
		}
		else {
			value.push_back(text[index]);
			index++;
		}
	}
	return std::stoi(value);
}


MyColor GameLog::readColorByRGB(int& index, std::string& text) {
	MyColor color;

	color.r = readColorRGBValue(index, text);
	color.g = readColorRGBValue(index, text);
	color.b = readColorRGBValue(index, text);

	return color;
}

MyColor GameLog::readColorByColorName(int& index, std::string& text) {
	std::string colorName;

	while (index < text.size()) {
		if (text[index] == ':') {
			return getColorByName(colorName);
		}
		else {
			colorName.push_back(text[index]);
		}

		index++;
	}

	debugLogger->log("Game message format error: End of string reached while trying to parse color name\n" + text);
	return MyColor(255, 255, 255);
}


MyColor GameLog::readColor(int& index, std::string& text) {
	index += 2;

	if (text[index] >= '0' && text[index] <= '9') {
		return readColorByRGB(index, text);
	}

	else {
		return readColorByColorName(index, text);
	}
}

GameMessage GameLog::makeGameMessage(std::string rawText) {
	std::vector<MessageColorNode> colorNodes;

	std::stack<MyColor> colorStack;
	colorStack.push(MyColor(255, 255, 255));

	std::string newText;
	int newTextIndex = -1;

	int i = 0;

	for (i; i < rawText.size(); i++) {
		if (rawText[i] == '<' && rawText[i + 1] == '/') {
			if (newTextIndex != 0) {
				colorNodes.push_back(MessageColorNode(colorStack.top(), newTextIndex));
			}

			colorStack.push(readColor(i, rawText));
		}

		else if (rawText[i] == '/' && rawText[i + 1] == '>') {
			colorNodes.push_back(MessageColorNode(colorStack.top(), newTextIndex));
			colorStack.pop();

			i++;
		}

		else {
			newText.push_back(rawText[i]);
			newTextIndex++;
		}
	}

	//the final color, if it's needed
	if (colorNodes.size() > 0) {
		if (colorNodes.back().endIndex != newTextIndex) {
			colorNodes.push_back(MessageColorNode(colorStack.top(), newTextIndex));
		}
	}
	else {
		colorNodes.push_back(MessageColorNode(colorStack.top(), newTextIndex));
	}

	return GameMessage(newText, colorNodes);
}


void GameLog::sendMessage(std::string text) {
	messageFile->write(text.c_str(), text.size());
	messageFile->write("\n", 1);

	recentMessages.push_back( makeGameMessage(text) );
}

std::vector<GameMessage>* GameLog::getRecentMessages() { return &recentMessages; }
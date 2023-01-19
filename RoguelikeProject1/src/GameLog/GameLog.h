#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include "../GraphicsThings/MyColor.h"
#include "../DebugLogger/DebugLogger.h"
#include <unordered_map>
#include "../GraphicsThings/ColorMap.h"
#include "../GraphicsThings/GameText/GameText.h"

class GameLog {
private:
	std::unique_ptr<DebugLogger> debugLogger;

	std::unique_ptr<std::fstream> messageFile;

	std::vector<GameText> recentMessages;

	std::unique_ptr<GameTextMaker> gameTextMaker;

public:
	GameLog();
	~GameLog();

	void sendMessage(std::string text);

	std::vector<GameText>* getRecentMessages();
};
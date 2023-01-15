#pragma once

#include <fstream>

//TODO: make this not shit
class ErrorLogger {
public:
	static void logError(std::string text) {
		std::fstream errorFile;

		errorFile.open("game_files/error_log.txt", std::ios::out | std::ios::app);

		if (!errorFile.is_open()) {
			printf("Failed to open error log file.");
			return;
		}

		errorFile << text << "\n\n";

		errorFile.close();
	}
};
#pragma once
#include <fstream>
#include <iostream>
#include <string>

class Logging
{
public:
	static Logging& GetInstance(const std::string& filename = "debugLog.log");
	~Logging();

	void Log(const char* format, ...);
	void LogLine(const char* format, ...);

private:
	Logging(const std::string& filename);
	Logging(const Logging&) = delete;
	Logging& operator=(const Logging&) = delete;

	std::ofstream logFile;
};
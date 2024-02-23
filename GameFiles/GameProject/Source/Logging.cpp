//------------------------------------------------------------------------------
//
// File Name:	Logging.cpp
// Author(s):	TayLee Young
// Purpose:		Prints to both console and a log file for debugging purposes
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "Logging.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdarg>
#include <sstream>

// constructor
Logging::Logging(const std::string& filename)
{
#ifdef _DEBUG

	logFile.open(filename, std::ios::app);
	if (!logFile.is_open())
	{
		std::cerr << "Error: Can't open log file." << std::endl;
	}
#endif // DEBUG
}

// destructor
Logging::~Logging()
{
#ifdef _DEBUG

	if (logFile.is_open())
	{
		logFile.close();
	}
#endif // DEBUG
}

//instances babeeyyy, pass in "debuglog.log" by default
Logging& Logging::GetInstance(const std::string& filename)
{
#ifdef _DEBUG

#endif // DEBUG
	static Logging instance(filename);
	return instance;
}

/***************************************************************************************
*    Title: Game Engine Architecture (second edition)
*    Author: Jason Gregory
*    pg. 411 - 413
*
***************************************************************************************/

// logs WITHOUT a newline to BOTH console and file
void Logging::Log(const char* format, ...)
{
#ifdef _DEBUG

	if (!logFile.is_open())
	{
		return;
	}

	va_list args;
	va_start(args, format);

	char buffer[4096];
	vsnprintf(buffer, sizeof(buffer), format, args);

	LogToConsole(buffer);
	LogToFile(buffer);

	va_end(args);
#endif // DEBUG
}

// logs WITH a newline to BOTH console and file
void Logging::LogLine(const char* format, ...)
{
#ifdef _DEBUG

	if (!logFile.is_open())
	{
		return;
	}

	va_list args;
	va_start(args, format);

	char buffer[4096];
	vsnprintf(buffer, sizeof(buffer), format, args);

	LogToConsole(buffer);
	LogToFile(buffer);
	LogToFile("\n");

	va_end(args);
#endif // DEBUG
}

// just console
void Logging::LogToConsole(const std::string& message)
{
#ifdef _DEBUG
	std::cout << message;
#endif // DEBUG
}

// just file
void Logging::LogToFile(const std::string& message)
{
#ifdef _DEBUG

	if (logFile.is_open())
	{
		logFile << message;
		logFile.flush();
	}
#endif // DEBUG
}

// timestamp so you know roughly when it was called, namely only useful long term (like my engine bizz)
void Logging::LogTimestamp()
{
#ifdef _DEBUG

	time_t now = std::time(0);
	tm timeinfo;
	if (localtime_s(&timeinfo, &now) == 0)
	{
		char buffer[128]; // use a larger buffer to be safe
		if (strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S] ", &timeinfo) > 0)
		{
			Log(buffer);
		}
		else
		{
			Log("Error formatting timestamp");
		}
	}
	else
	{
		Log("Error getting local time");
	}
#endif // DEBUG
}

// Logs everything for ez of access
void Logging::LogToAll(const char* format, ...)
{
#ifdef _DEBUG

	if (!logFile.is_open())
	{
		return;
	}

	va_list args;
	va_start(args, format);

	char buffer[4096];
	vsnprintf(buffer, sizeof(buffer), format, args);

	LogTimestamp();
	Log(buffer);
	LogLine("");

	va_end(args);
#endif // DEBUG
}
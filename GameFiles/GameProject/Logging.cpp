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

Logging::Logging(const std::string& filename)
{
	logFile.open(filename);
	if (!logFile.is_open())
	{
		std::cerr << "Error: Can't open log file." << std::endl;
	}
}

Logging::~Logging()
{
	if (logFile.is_open())
	{
		logFile.close();
	}
}

Logging& Logging::GetInstance(const std::string& filename)
{
	static Logging instance(filename);
	return instance;
}


/***************************************************************************************
*    Title: Game Engine Architecture (second edition)
*    Author: Jason Gregory
*    pg. 411 - 413
*
***************************************************************************************/
 
void Logging::Log(const char* format, ...)
{
	if (!logFile.is_open())
	{
		return;
	}

	va_list args;
	va_start(args, format);

	char buffer[4096];
	vsnprintf(buffer, sizeof(buffer), format, args);

	logFile << buffer;
	logFile.flush();

	va_end(args);

	OutputDebugStringA(buffer);

}

void Logging::LogLine(const char* format, ...)
{
	if (!logFile.is_open())
	{
		return;
	}

	va_list args;
	va_start(args, format);

	char buffer[4096];
	vsnprintf(buffer, sizeof(buffer), format, args);

	logFile << buffer << std::endl;
	logFile.flush();

	va_end(args);

	OutputDebugStringA(buffer);
	OutputDebugStringA("\n");
}
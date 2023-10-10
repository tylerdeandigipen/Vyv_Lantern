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

// The following code, namely any use of va_(anything) is not entirely mine. Credit goes to IBM:
// https://www.ibm.com/docs/en/i/7.1?topic=lf-va-arg-va-end-va-start-access-function-arguments
// 
// For examples on how to use.
// 
// VA_(anything) is used to allow for arguments to be passed in when logging. Previously, just allowed strings.
 
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
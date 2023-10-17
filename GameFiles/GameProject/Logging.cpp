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

Logging::Logging(const std::string& filename)
{
	logFile.open(filename, std::ios::app);
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

    LogToConsole(buffer);
    LogToFile(buffer);

    va_end(args);
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

    LogToConsole(buffer);
    LogToFile(buffer);
    LogToFile("\n");

    va_end(args);
}

void Logging::LogToConsole(const std::string& message)
{
    std::cout << message;
}

void Logging::LogToFile(const std::string& message)
{
    if (logFile.is_open())
    {
        logFile << message;
        logFile.flush();
    }
}

void Logging::LogTimestamp()
{
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
}


void Logging::LogToAll(const char* format, ...)
{
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
}
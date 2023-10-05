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
void Logging::Log(const std::string& message)
{
	if (logFile.is_open())
	{
		logFile << message;
		logFile.flush();
	}

	OutputDebugStringA(message.c_str());
}

void Logging::LogLine(const std::string& message)
{
	if (logFile.is_open())
	{
		logFile << message << std::endl;
		logFile.flush();
	}

	OutputDebugStringA((message + "\n").c_str());
}